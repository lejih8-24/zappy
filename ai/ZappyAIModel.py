#!/usr/local/bin/python

import time
from CommandModel import *
from BroadcastManager import BroadcastManager
from constants import Role, State, ELEVATION_RULES
from pathfinding import find_path_to_closest


class ZappyAI:
    def __init__(self, network, team_name, map_x, map_y):
        self.network = network
        self.team_name = team_name
        self.map_x = map_x
        self.map_y = map_y

        self.level = 1
        self.inventory = {"food": 10, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0,
                          "thystame": 0}
        self.elevation_rules = ELEVATION_RULES

        self.is_alive = True
        self.pending_commands = []
        self.vision_grid = None
        self.role = Role.Explorer
        self.state = State.FARMING
        self.comms = BroadcastManager(token="AlphaNor_Zappy_26")
        self.previous_debug = ""

    def run(self):
        while self.is_alive:
            responses = self.network.get_responses()
            for response in responses:
                self._process_server_message(response)

            if self.is_alive:
                self._decide_next_action()

            time.sleep(0.01)

    def _process_server_message(self, message: str):
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
                        print(f"🏃 Entendu l'appel du Master {decoded['sender_id']} à la direction {direction}")

                    elif decoded["request"] == "INCANTATION_STARTING" and self.role == Role.Slave:
                        if direction == 0:
                            print("Je suis sur la case de l'incantation ! Je reste pour le rituel.")
                            self.state = State.CONTRIBUTING
                        else:
                            print("Le groupe est déjà complet et je suis trop loin. Retour au farming.")
                            self.role = Role.Explorer
                            self.state = State.FARMING

                    elif decoded["request"] == "ABORT" and self.role == Role.Slave:
                        self.role = Role.Explorer
                        self.state = State.FARMING
                        print("Le Master a annulé l'appel (ressources manquantes). Retour au farming.")
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
            self.level = int(message.split(":")[1].strip())
            self.state = State.FARMING
            self.role = Role.Explorer
            print(f"Succès ! Je suis niveau {self.level}")
            return

        if message == "ko" and self.state == State.WAITING_ELEVATION:
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
            print(f"Réponse inattendue (aucune commande en attente) : {message}")

    def _is_command_pending(self, command_class) -> bool:
        """Vérifie si un type de commande précis est déjà dans la file d'attente."""
        return any(isinstance(cmd, command_class) for cmd in self.pending_commands)

    def _update_state_from_result(self, command, result):
        if isinstance(command, InventoryCommand) and isinstance(result, dict):
            self.inventory = result
        elif isinstance(command, LookCommand) and isinstance(result, list):
            self.vision_grid = result
        elif isinstance(command, TakeCommand) and result is True:
            self.inventory[command.obj_name] += 1
        elif isinstance(command, SetCommand) and result is True:
            self.inventory[command.obj_name] = self.inventory.get(command.obj_name, 1) - 1

    def _queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.network.send_command(command.command_string)

    def can_elevate(self) -> bool:
        rules = self.elevation_rules[self.level]
        for stone, required_qty in rules["stones"].items():
            if self.inventory.get(stone, 0) < required_qty:
                return False
        return True

    def _count_player_case(self):
        if not self.vision_grid:
            return 0
        return self.vision_grid[0].get('player', 0)

    def _decide_next_action(self):
        if len(self.pending_commands) >= 9:
            debug = f"[DEBUG] File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau."
            if debug != self.previous_debug:
                self.previous_debug = f"[DEBUG] File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau."
                print(self.previous_debug)
            return

        if self.state == State.WAITING_ELEVATION:
            debug = f"[DEBUG] En transe (WAITING_ELEVATION). L'IA est bloquée en attendant la fin de l'incantation."
            if debug != self.previous_debug:
                self.previous_debug = f"[DEBUG] En transe (WAITING_ELEVATION). L'IA est bloquée en attendant la fin de l'incantation."
                print(self.previous_debug)
            return

        current_food = self.inventory.get("food", 0)

        if current_food < 15 and self.state not in [State.INCANTATION, State.CONTRIBUTING]:
            if self.state != State.SURVIVAL:
                debug = f"[DEBUG] URGENCE VITALE : Nourriture critique ({current_food}/15). Abandon de l'état {self.state.name}, passage en SURVIVAL !"
                if debug != self.previous_debug:
                    self.previous_debug = f"[DEBUG] URGENCE VITALE : Nourriture critique ({current_food}/15). Abandon de l'état {self.state.name}, passage en SURVIVAL !"
                    print(self.previous_debug)
            self.state = State.SURVIVAL

        elif current_food >= 20 and self.state == State.SURVIVAL:
            debug = f"[DEBUG] Réserves reconstituées ({current_food}/25). Fin de l'urgence, retour au FARMING !"
            if debug != self.previous_debug:
                self.previous_debug = f"[DEBUG] Réserves reconstituées ({current_food}/25). Fin de l'urgence, retour au FARMING !"
                print(self.previous_debug)
            self.state = State.FARMING

        elif self.can_elevate() and self.state == State.FARMING:
            debug = f"[DEBUG] J'ai toutes les pierres requises pour le niveau {self.level + 1} ! Je deviens MASTER et passe en GROUPING."
            if debug != self.previous_debug:
                self.previous_debug = f"[DEBUG] J'ai toutes les pierres requises pour le niveau {self.level + 1} ! Je deviens MASTER et passe en GROUPING."
                print(self.previous_debug)
            self.state = State.GROUPING
            self.role = Role.Master

        debug = f"[DEBUG] Réflexion -> État: {self.state.name} | Rôle: {self.role.name} | Nourriture: {current_food}"
        if debug != self.previous_debug:
            self.previous_debug = f"[DEBUG] Réflexion -> État: {self.state.name} | Rôle: {self.role.name} | Nourriture: {current_food}"
            print(self.previous_debug)

        if self.state == State.SURVIVAL:
            self._state_survival()
        elif self.state == State.FARMING:
            self._state_farming()
        elif self.state == State.GROUPING:
            self._state_grouping()
        elif self.state == State.CONTRIBUTING:
            self._state_contributing()
        elif self.state == State.INCANTATION:
            self._state_incantation()

    def _move_instructions(self, path: list, target_item: str):
        if path is not None:
            for action in path:
                if action == "Forward":
                    self._queue_command(ForwardCommand())
                elif action == "Right":
                    self._queue_command(TurnRightCommand())
                elif action == "Left":
                    self._queue_command(TurnLeftCommand())

            if target_item:
                self._queue_command(TakeCommand(target_item))
            self.vision_grid = None
        else:
            self._queue_command(ForwardCommand())
            self._queue_command(LookCommand())
            self.vision_grid = None

    def _get_needed_material(self) -> str | None:
        if self.level >= 8:
            return None
        rule = self.elevation_rules[self.level]["stones"]
        for stone, required_qty in rule.items():
            if self.inventory.get(stone, 0) < required_qty:
                return stone
        return None

    def _state_survival(self):
        if not self.vision_grid:
            if not self._is_command_pending(LookCommand):
                self._queue_command(LookCommand())
            return
        path = find_path_to_closest(self.vision_grid, "food")
        self._move_instructions(path, "food")

    def _state_farming(self):
        if not self.vision_grid:
            if not self._is_command_pending(LookCommand):
                self._queue_command(LookCommand())
            return

        target_mat = self._get_needed_material()
        if not target_mat:
            print("Toutes les ressources sont réunies. Passage en mode GROUPING.")
            self.state = State.GROUPING
            return

        path = find_path_to_closest(self.vision_grid, target_mat)
        self._move_instructions(path, target_mat)

    def _state_grouping(self):
        if self.role == Role.Master:
            if not self.vision_grid:
                if not self._is_command_pending(LookCommand):
                    self._queue_command(LookCommand())
                return

            players_on_tile = self._count_player_case()
            required_players = self.elevation_rules[self.level]["players"]

            if players_on_tile >= required_players:
                print(f"Assez de trantoriens ({players_on_tile}/{required_players}).")

                if not self._is_command_pending(BroadcastCommand):
                    msg = self.comms.format_message("ALL", self.level, Role.Master, State.INCANTATION,
                                                    "INCANTATION_STARTING")
                    self._queue_command(BroadcastCommand(self.comms.token, msg))

                self.state = State.INCANTATION
            else:
                print(f"En attente de trantoriens ({players_on_tile}/{required_players}).")

                if not self._is_command_pending(BroadcastCommand):
                    msg = self.comms.format_message("ALL", self.level, Role.Master, State.GROUPING,
                                                    "INCANTATION_CALL")
                    self._queue_command(BroadcastCommand(self.comms.token, msg))

                self.vision_grid = None
        elif self.role == Role.Slave:
            if getattr(self, "target_direction", None) is None:
                return

            k = self.target_direction

            self.target_direction = None

            if k == 0:
                print("Arrivé sur la case du Master ! Je vide mes poches.")
                self.state = State.CONTRIBUTING
            elif k in [1, 2, 8]:
                self._queue_command(ForwardCommand())
            elif k in [3, 4]:
                self._queue_command(TurnLeftCommand())
            elif k in [5, 6, 7]:
                self._queue_command(TurnRightCommand())

    def _state_contributing(self):
        """Le Slave est sur la case du Master et donne toutes ses pierres."""
        dropped_something = False

        for item, qty in list(self.inventory.items()):
            if item != "food" and qty > 0:
                for _ in range(qty):
                    self._queue_command(SetCommand(item))
                    self.inventory[item] -= 1
                    dropped_something = True

        if dropped_something:
            print("Don de mes pierres en cours...")
        else:
            print("Je n'ai plus rien. J'attends le rituel.")
            self.state = State.WAITING_ELEVATION

    def _is_floor_perfect(self) -> bool:
        if not self.vision_grid: return False
        floor = self.vision_grid[0]
        rules = self.elevation_rules[self.level]

        if floor.get("player", 0) != rules["players"]:
            return False

        for item, qty in floor.items():
            if item == "player" or qty == 0:
                continue
            if qty != rules["stones"].get(item, 0):
                return False

        for stone, required_qty in rules["stones"].items():
            if floor.get(stone, 0) != required_qty:
                return False
        return True

    def _clean_floor(self) -> bool:
        if not self.vision_grid: return False
        floor = self.vision_grid[0]
        cleaned_something = False

        for item, qty in floor.items():
            if item != "player" and qty > 0:
                for _ in range(qty):
                    self._queue_command(TakeCommand(item))
                    cleaned_something = True
        return cleaned_something

    def _state_incantation(self):
        if self._is_command_pending(SetCommand) or self._is_command_pending(TakeCommand):
            return

        if not self.vision_grid:
            if not self._is_command_pending(LookCommand):
                self._queue_command(LookCommand())
            return

        if self._is_floor_perfect():
            print(f"La case est prête ! Lancement de l'Incantation (Niveau {self.level} -> {self.level + 1}).")
            self._queue_command(IncantationCommand())
            self.state = State.WAITING_ELEVATION
            return

        if not self.can_elevate():
            print(f"Inventaire insuffisant pour préparer le rituel du niveau {self.level + 1}.")
            msg = self.comms.format_message("ALL", self.level, Role.Master, State.FARMING, "ABORT")
            self._queue_command(BroadcastCommand(self.comms.token, msg))
            self.state = State.FARMING
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

        self.vision_grid = None
        self._queue_command(LookCommand())