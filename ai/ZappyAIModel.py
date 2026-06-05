from CommandModel import *
import time
import sys
import math

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

        if message == "dead":
            print("L'IA est morte de faim.")
            self.is_alive = False
            return

        if message.startswith("message"):
            parts = message.split(",", 1)
            if len(parts) == 2:
                direction = int(parts[0].replace("message", "").strip())
                text = parts[1].strip()
                self._handle_incoming_broadcast(direction, text)
            return

        if message.startswith("eject"):
            # TODO: Gérer l'éjection (recalculer son pathfinding, etc.)
            print(f"Je me suis fait éjecter ! ({message})")
            return

        if message == "Elevation underway" or message.startswith("Current level:"):
            if message == "Elevation underway":
                print("Début de l'incantation, je dois être immobile !")
                return

            if message.startswith("Current level:"):
                new_level = int(message.split(":")[1].strip())
                self.level = new_level
                print(f"Succès ! Je suis niveau {self.level}")
                return

        if self.pending_commands:
            current_command = self.pending_commands.pop(0)

            result = current_command.parse_response(message)
            print(f"Commande {current_command.command_string} terminée. Résultat: {result}")

            self._update_state_from_result(current_command, result)
        else:
            print(f"Réponse inattendue (aucune commande en attente) : {message}")

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

    def _decide_next_action(self):
        if len(self.pending_commands) >= 9:
            return

        if self.inventory.get("food", 0) < 15 and self.state != "INCANTATION":
            self.state = "SURVIVAL"
        elif self.can_elevate():
            self.state = "GROUPING"

        if self.state == "SURVIVAL":
            self._state_survival()

        elif self.state == "FARMING":
            self._state_farming()

        elif self.state == "GROUPING":
            self._state_grouping()

        elif self.state == "INCANTATION":
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

    def _state_survival(self):
        if not self.vision_grid:
            self._queue_command(LookCommand())
            return

        # TODO: faire la fonction qui cherche la bouffe la plus proche
        path_to_food = self._find_path_to_closest("food")
        self._move_instructions(path_to_food)



    def _state_farming(self):
        if not self.vision_grid:
            self._queue_command(LookCommand())
            return

        path_to_mats = self._find_path_to_closest_mat()

        self._move_instructions(path_to_mats)

    def _state_incantation(self):
        """Objectif : S'élever."""
        # TODO: verif d'avoir les bons objets
        self._queue_command(IncantationCommand())
