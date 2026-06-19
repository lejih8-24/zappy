#!/usr/local/bin/python

import time
from CommandModel import *
from BroadcastManager import BroadcastManager
from ai.logger import AILogger
from constants import Role, State, ELEVATION_RULES
from pathfinding import find_path_to_closest
import os
import json


class ZappyAI:
    def __init__(self, network, team_name, map_x, map_y):
        self.network = network
        self.team_name = team_name
        self.map_x = map_x
        self.map_y = map_y
        self.logger = AILogger("./ai.logs")

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

        self.pos_x = 0
        self.pos_y = 0
        self.orientation = 0

        self.world_map = {}

        if not any(f.endswith('.json') for f in os.listdir('.') if f.startswith('.zappy_stats')):
            os.makedirs(".zappy_stats", exist_ok=True)

    def _save_dashboard_state(self):
        """Sauvegarde l'état actuel dans un fichier JSON partagé."""
        state_data = {
            "id": self.comms.my_id,
            "role": self.role.name,
            "state": self.state.name,
            "level": self.level,
            "position": [self.pos_x, self.pos_y],
            "inventory": self.inventory
        }

        os.makedirs(".zappy_stats", exist_ok=True)
        filename = f".zappy_stats/drone_{self.comms.my_id}.json"

        temp_filename = f"{filename}.tmp"
        with open(temp_filename, "w") as f:
            json.dump(state_data, f)
        os.replace(temp_filename, filename)

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
                        if self.role != Role.Master:
                            self.role = Role.Slave
                            self.state = State.GROUPING
                            self.target_direction = direction
                            print(
                                f"[COMMS] Entendu l'appel du Master {decoded['sender_id']} à la direction {direction}. Je passe Slave.")
                        else:
                            print(
                                f"[COMMS] J'entends un appel ({decoded['sender_id']}) mais je suis déjà MASTER. J'ignore.")

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

                elif decoded["request"] == "DATA_SHARE":
                    try:
                        raw_data = decoded.get("data", "")
                        parts = raw_data.split("|")
                        if len(parts) == 4:
                            share_x = int(parts[0])
                            share_y = int(parts[1])
                            resource = parts[2]
                            qty = int(parts[3])

                            target_pos = (share_x, share_y)

                            if target_pos not in self.world_map:
                                self.world_map[target_pos] = {}

                            self.world_map[target_pos][resource] = qty
                    except Exception as e:
                        print(f"Erreur lors du parsing de la synchronisation cartographique : {e}")
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

    def _update_state_from_result(self, command, result):
        if isinstance(command, InventoryCommand) and isinstance(result, dict):
            self.inventory = result

        elif isinstance(command, LookCommand) and isinstance(result, list):
            self.vision_grid = result
            self._integrate_vision_to_map(result)

        elif isinstance(command, TakeCommand) and result is True:
            self.inventory[command.obj_name] += 1
            current_pos = (self.pos_x, self.pos_y)
            if current_pos in self.world_map and command.obj_name in self.world_map[current_pos]:
                if self.world_map[current_pos][command.obj_name] > 0:
                    self.world_map[current_pos][command.obj_name] -= 1

        elif isinstance(command, SetCommand) and result is True:
            self.inventory[command.obj_name] = self.inventory.get(command.obj_name, 1) - 1

        elif isinstance(command, ForkCommand) and result is True:
            print("[DEBUG] L'oeuf a été pondu avec succès ! Retour au travail.")
            self.state = State.FARMING

        elif isinstance(command, TurnRightCommand) and result is True:
            self.orientation = (self.orientation + 1) % 4

        elif isinstance(command, TurnLeftCommand) and result is True:
            self.orientation = (self.orientation - 1) % 4

        elif isinstance(command, ForwardCommand) and result is True:
            if self.orientation == 0:
                self.pos_y = (self.pos_y + 1) % self.map_y
            elif self.orientation == 1:
                self.pos_x = (self.pos_x + 1) % self.map_x
            elif self.orientation == 2:
                self.pos_y = (self.pos_y - 1) % self.map_y
            elif self.orientation == 3:
                self.pos_x = (self.pos_x - 1) % self.map_x

    def _decide_next_action(self):
        if len(self.pending_commands) >= 9:
            debug = f"[DEBUG] File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau."
            if debug != self.previous_debug:
                self.previous_debug = f"[DEBUG] File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau."
                print(self.previous_debug)
            return

        self._save_dashboard_state()

    def _queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.network.send_command(command.command_string)

    def _is_command_pending(self, command_class) -> bool:
        """Vérifie si un type de commande précis est déjà dans la file d'attente."""
        return any(isinstance(cmd, command_class) for cmd in self.pending_commands)

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


    def _get_needed_material(self) -> str | None:
        if self.level >= 8:
            return None
        rule = self.elevation_rules[self.level]["stones"]
        for stone, required_qty in rule.items():
            if self.inventory.get(stone, 0) < required_qty:
                return stone
        return None