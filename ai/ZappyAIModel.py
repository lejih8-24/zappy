from CommandModel import *
import time
import sys
import math
from enum import Enum
from BroadcastManager import BroadcastManager

class Role(Enum):
    Explorer = 0
    Master = 1
    Incanting = 2
    Slave = 3

class State(Enum):
    FARMING = 0
    INCANTATION = 1
    GROUPING = 2
    SURVIVAL = 3
    WAITING_ELEVATION = 4
    CONTRIBUTING = 5


class ZappyAI:
    def __init__(self, network, team_name, map_x, map_y):
        self.network = network
        self.team_name = team_name
        self.map_x = map_x
        self.map_y = map_y

        self.level = 1
        self.inventory = {"food": 10, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0,
                          "thystame": 0}

        self.elevation_rules = {
            1: {"players": 1,
                "stones": {"linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0}},
            2: {"players": 2,
                "stones": {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0}},
            3: {"players": 2,
                "stones": {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0}},
            4: {"players": 4,
                "stones": {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0}},
            5: {"players": 4,
                "stones": {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0}},
            6: {"players": 6,
                "stones": {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0}},
            7: {"players": 6,
                "stones": {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0}},
        }

        self.is_alive = True
        self.pending_commands = []
        self.role = Role.Explorer
        self.comms = BroadcastManager(token="AlphaNor_Zappy_26")

    def run(self):
        while self.is_alive:
            responses = self.network.get_responses()

            for response in responses:
                self._process_server_message(response)

            if self.is_alive:
                self._decide_next_action()

            # Jveux garder mon pc en vie stp
            time.sleep(0.01)

    def _process_server_message(self, message: str):
        """Routeur principal : Sépare les événements asynchrones des réponses synchrones."""

        if message == "dead":
            print("L'IA est morte de faim.")
            self.is_alive = False
            return

        if message.startswith("message"):
            parts = message.split(",", 1)
            if len(parts) == 2:
                direction = int(parts[0].replace("message", "").strip())
                raw_text = parts[1].strip()

                decoded = self.comms.parse_message(raw_text)

                if decoded:
                    if decoded["request"] == "INCANTATION_CALL" and decoded["level"] == self.level:
                        self.role = Role.Slave
                        self.state = State.GROUPING
                        self.target_direction = direction
                        print(f"Entendu l'appel du Master {decoded['sender_id']} à la direction {direction}")

                    elif decoded["request"] == "ABORT" and self.role == Role.Slave:
                        self.role = Role.Explorer
                        self.state = State.FARMING
                        print("Le Master a annulé l'appel. Retour au farming.")
            return

        if message.startswith("eject"):
            direction = int(message.split(":")[1].strip())
            print(f"Je me suis fait éjecter depuis la direction {direction} !")

            self.pending_commands.clear()
            self.vision_grid = None
            return

        if message == "Elevation underway":
            print("Début de l'incantation, je suis immobilisé !")
            if self.pending_commands and isinstance(self.pending_commands[0], IncantationCommand):
                self.pending_commands.pop(0)
            return

        if message.startswith("Current level:"):
            new_level = int(message.split(":")[1].strip())
            self.level = new_level

            self.state = State.FARMING
            self.role = Role.Explorer

            print(f"Succès ! Je suis niveau {self.level}")
            return

        if message == "ko" and self.state in [State.INCANTATION, State.WAITING_ELEVATION]:
            print("L'incantation a échoué (un joueur a bougé ou une pierre manque).")
            self.state = State.FARMING
            self.role = Role.Explorer
            return

        if self.pending_commands:
            current_command = self.pending_commands.pop(0)
            result = current_command.parse_response(message)

            print(f"Commande {current_command.command_string} terminée. Résultat: {result}")
            self._update_state_from_result(current_command, result)
        else:
            print(f"❓ Réponse inattendue (aucune commande en attente) : {message}")

    def _update_state_from_result(self, command, result):

        if isinstance(command, InventoryCommand) and isinstance(result, dict):
            self.inventory = result

        elif isinstance(command, LookCommand) and isinstance(result, list):
            self.vision_grid = result

        elif isinstance(command, TakeCommand) and result is True:
            self.inventory[command.obj_name] += 1
            pass

    def _queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.network.send_command(command.command_string)
        else:
            pass

    def can_elevate(self) -> bool:
        rules = self.elevation_rules[self.level]

        for stone, required_qty in rules["stones"].items():
            if self.inventory.get(stone, 0) < required_qty:
                return False

        return True

    def _count_player_case(self):
        list_vision = self.vision_grid[0]
        self_case = list_vision.split(' ')
        number_players = self_case.count('player')
        return number_players

    def _state_grouping(self):
        """Gère le regroupement avant incantation."""

        if self.role == "master":
            players_on_tile = self._count_player_case()
            required_players = self.elevation_rules[self.level]["players"]

            if players_on_tile >= required_players:
                print(f"Assez de trantoriens ({players_on_tile}/{required_players}).")

                msg = self.comms.format_message("ALL", self.level, Role.Master, State.INCANTATION, "ABORT")
                self._queue_command(BroadcastCommand(msg))

                self.state = State.INCANTATION
            else:
                print(f"En attente de trantoriens ({players_on_tile}/{required_players}).")

                msg = self.comms.format_message("ALL", self.level, Role.Master, State.GROUPING, "INCANTATION_CALL")
                self._queue_command(BroadcastCommand(msg))

    def _decide_next_action(self):
        if len(self.pending_commands) >= 9 or self.state == State.WAITING_ELEVATION:
            return

        if self.inventory.get("food", 0) < 15 and self.state != State.INCANTATION:
            self.state = State.SURVIVAL
        elif self.can_elevate():
            self.state = State.GROUPING

        if self.state == State.SURVIVAL:
            self._state_survival()

        elif self.state == State.FARMING:
            self._state_farming()

        elif self.state == State.GROUPING:
            self._state_grouping()

        elif self.state == State.INCANTATION:
            self._state_incantation()

    def _move_instructions(self, path: list, target_item: str):
        """Exécute un chemin et ramasse l'objet à la fin."""

        if path is not None:
            for action in path:
                if action == "Forward":
                    self._queue_command(ForwardCommand())
                elif action == "Right":
                    self._queue_command(TurnRightCommand())
                elif action == "Left":
                    self._queue_command(TurnLeftCommand())

            self._queue_command(TakeCommand(target_item))
            self.vision_grid = None
        else:
            self._queue_command(ForwardCommand())
            self._queue_command(LookCommand())

    def _generate_path_to_index(self, index: int) -> list[str]:
        """Convertit un index du tableau Look en séquence d'actions Zappy."""
        if index == 0:
            return []

        import math
        y = math.isqrt(index)
        x = index - (y * y + y)

        path = []

        for _ in range(y):
            path.append("Forward")

        if x < 0:
            path.append("Left")
            for _ in range(abs(x)):
                path.append("Forward")
        elif x > 0:
            path.append("Right")
            for _ in range(x):
                path.append("Forward")

        return path

    def _find_path_to_closest(self, target: str) -> list[str] | None:
        """
        Trouve la ressource demandée la plus proche (en nombre d'actions Zappy)
        et retourne le chemin pour y aller. Retourne None si introuvable.
        """
        if not self.vision_grid:
            return None

        best_index = -1
        best_cost = 999

        for i, tile in enumerate(self.vision_grid):
            if tile.get(target, 0) > 0:
                y = math.isqrt(i)
                x = i - (y * y + y)

                cost = y + abs(x) + (1 if x != 0 else 0)

                if cost < best_cost:
                    best_cost = cost
                    best_index = i

        if best_index == -1:
            return None

        return self._generate_path_to_index(best_index)

    def _get_needed_material(self) -> str | None:
        """Regarde l'inventaire et retourne le nom de la première pierre manquante."""
        if self.level >= 8:
            return None

        rule = self.elevation_rules[self.level]["stones"]
        for stone, required_qty in rule.items():
            if self.inventory.get(stone, 0) < required_qty:
                return stone
        return None

    def _state_survival(self):
        """État de survie : cherche la nourriture la plus proche."""
        if not self.vision_grid:
            self._queue_command(LookCommand())
            return

        path_to_food = self._find_path_to_closest("food")
        self._move_instructions(path_to_food, "food")

    def _state_farming(self):
        """État de farming : cherche les pierres manquantes."""
        if not self.vision_grid:
            self._queue_command(LookCommand())
            return

        target_mat = self._get_needed_material()

        if not target_mat:
            print("Toutes les ressources sont réunies. Passage en mode GROUPING.")
            self.state = State.GROUPING
            return

        path_to_mats = self._find_path_to_closest(target_mat)
        self._move_instructions(path_to_mats, target_mat)

    def _is_floor_perfect(self) -> bool:
        """Vérifie si la case contient EXACTEMENT les bonnes pierres et les bons joueurs."""
        floor = self.vision_grid[0]  # Ex: {'player': 2, 'linemate': 1}
        rules = self.elevation_rules[self.level]

        # 1. Vérifier le nombre de joueurs
        if floor.get("player", 0) != rules["players"]:
            return False

        # 2. Vérifier qu'il n'y a pas d'intrus et que les quantités sont exactes
        for item, qty in floor.items():
            if item == "player" or qty == 0:
                continue  # On ignore les joueurs et les objets à 0

            required_qty = rules["stones"].get(item, 0)
            if qty != required_qty:
                return False  # Trop de pierres, ou objet interdit (ex: food)

        # 3. Vérifier qu'il ne manque aucune pierre (au cas où elle ne serait pas dans le dict)
        for stone, required_qty in rules["stones"].items():
            if floor.get(stone, 0) != required_qty:
                return False

        return True

    def _clean_floor(self) -> bool:
        """Ramasse absolument tout sur la case (sauf les joueurs). Retourne True si on a nettoyé."""
        floor = self.vision_grid[0]
        cleaned_something = False

        for item, qty in floor.items():
            if item != "player" and qty > 0:
                for _ in range(qty):
                    self._queue_command(TakeCommand(item))
                    cleaned_something = True

        return cleaned_something

    def _state_incantation(self):
        """Gère la préparation chirurgicale et le lancement de l'incantation."""

        if not self.vision_grid:
            self._queue_command(LookCommand())
            return

        if self._is_floor_perfect():
            print(f"La case est prête ! Lancement de l'Incantation (Niveau {self.level} -> {self.level + 1}).")
            self._queue_command(IncantationCommand())
            self.state = "WAITING_ELEVATION"
            return

        if not self.can_elevate():
            print(f"Inventaire insuffisant pour préparer le rituel du niveau {self.level + 1}.")
            msg = self.comms.format_message("ALL", self.level, Role.Master, State.FARMING, "ABORT")
            self._queue_command(BroadcastCommand(msg))
            self.state = "FARMING"
            return

        if self._clean_floor():
            print("Nettoyage de la case en cours...")
            self.vision_grid = None
            self._queue_command(LookCommand())
            return

        print(f"Dépôt chirurgical des pierres pour le niveau {self.level + 1}...")
        rules = self.elevation_rules[self.level]
        for stone, required_qty in rules["stones"].items():
            for _ in range(required_qty):
                self._queue_command(SetCommand(stone))
                self.inventory[stone] = self.inventory.get(stone, 0) - 1

        self.vision_grid = None
        self._queue_command(LookCommand())
