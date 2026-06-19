#!/usr/local/bin/python

import time
from CommandModel import *
from BroadcastManager import BroadcastManager
from ai.ai_movement import AINavigator
from ai.ai_states import AIState
from ai.dashboard import Display
from ai.logger import AILogger
from ai.btree.bt_core import Selector, Sequence, NodeStatus
from ai.btree.bt_nodes import IsHungry, ActionSearchFood, ActionFarmStones
from constants import Role, State, ELEVATION_RULES
from pathfinding import find_path_to_closest
import os
import json
from ai.network import ServerEvent


class ZappyAI:
    def __init__(self, network, team_name, map_x, map_y):
        self.id = str(uuid.uuid4())[:4]
        self.network = network
        self.map_x = map_x
        self.map_y = map_y
        self.logger = AILogger("./ai.logs")
        self.comms = BroadcastManager(self.id, token="AlphaNor_Zappy_26")
        self.display = Display()
        self.states = AIState(self.id, team_name)
        self.Navigation = AINavigator(self)
        self.bt = self._build_behavior_tree()

        self.is_alive = True
        self.pending_commands = []

    def _build_behavior_tree(self):
        """Construit l'arbre d'intelligence de bas en haut."""

        # Branche 1 : Survie
        # SI je suis affamé ET que je cherche de la food
        survival_branch = Sequence([IsHungry(), ActionSearchFood()])

        # Branche 2 : Fork
        # SI je suis riche en food ET que je lance le Fork
        # fork_branch = Sequence([HasEnoughFood(), ActionFork()])

        # Branche 3 : Élévation
        # elevation_branch = Sequence([CanElevate(), ActionGroupAndIncant()])

        # Branche 4 : Farming par défaut
        farming_branch = ActionFarmStones()

        # Le Noeud Racine : Un Sélecteur qui lit de haut en bas
        root = Selector([
            survival_branch,
            # fork_branch,
            # elevation_branch,
            farming_branch
        ])

        return root

    def run(self):
        while self.is_alive:
            responses = self.network.get_responses()
            for response in responses:
                self._process_server_event(response)

            if self.is_alive:
                self._decide_next_action()

            time.sleep(0.01)

    def _handle_broadcast(self, direction: int, decoded: dict):
        """Gère la réception des messages et met à jour exclusivement le Blackboard."""
        req = decoded.get("request")
        level = decoded.get("level")
        sender_id = decoded.get("sender_id")

        if req != "DATA_SHARE" and level != self.states.level:
            return

        if req == "INCANTATION_CALL":
            if self.states.is_master:
                self.logger.Info(f"[COMMS] Appel de {sender_id} entendu, mais je suis déjà MASTER. J'ignore.")
                return

            self.states.last_master_id = sender_id
            self.states.master_direction = direction
            self.logger.Info(f"[COMMS] Master {sender_id} appelle dans la direction {direction}. Enregistré.")

        elif req == "INCANTATION_STARTING":
            if direction == 0:
                self.logger.Info("[COMMS] Rituel démarré. Je suis sur la case, verrouillage de ma position.")
                self.states.ready_for_incantation = True
            else:
                self.logger.Info("[COMMS] Le groupe est complet et je suis en retard. Annulation du suivi.")
                self.states.clear_master_call()

        elif req == "ABORT":
            self.logger.Info(f"[COMMS] Le Master {sender_id} a annulé l'appel. Retour au travail.")
            self.states.clear_master_call()

    def _handle_command_response(self, raw_message: str):
        """Associe la réponse à la commande en attente."""
        if raw_message == "ko" and self.states.state == State.WAITING_ELEVATION:
            self.logger.Warn("L'incantation a échoué (un joueur a bougé ou une pierre manque).")
            self.states.state = State.FARMING
            self.states.role = Role.Explorer
            return

        if self.pending_commands:
            current_command = self.pending_commands.pop(0)
            result = current_command.parse_response(raw_message)
            self._update_state_from_result(current_command, result)
        else:
            self.logger.Error(f"Réponse inattendue (aucune commande en attente) : {raw_message}")


    def _process_server_event(self, event: ServerEvent):
        """Route les événements propres vers les bons organes de l'IA."""
        if event.type == "DEAD":
            self.logger.Error("L'IA est morte de faim.")
            self.is_alive = False

        elif event.type == "EJECT":
            self.logger.Warn(f"Je me suis fait éjecter depuis la direction {event.data['direction']} !")
            self.pending_commands.clear()
            self.states.vision_grid = None

        elif event.type == "ELEVATION_START":
            self.logger.Good("Début de l'incantation, je suis immobilisé !")
            if self.pending_commands and isinstance(self.pending_commands[0], IncantationCommand):
                self.pending_commands.pop(0)

        elif event.type == "ELEVATION_SUCCESS":
            self.states.level = event.data["level"]
            self.states.state = State.FARMING
            self.states.role = Role.Explorer
            self.logger.Good(f"Succès ! Je suis niveau {self.states.level}")

        elif event.type == "BROADCAST":
            self._handle_broadcast(event.data["direction"], event.data["decoded"])

        elif event.type == "RESPONSE":
            self._handle_command_response(event.data["raw"])

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
            self.logger.Good("[DEBUG] L'oeuf a été pondu avec succès ! Retour au travail.")
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
            self.logger.Info(f"[DEBUG] File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau.")

        self.bt.tick(self)

        self.display.save_dashboard_state(self.states.name, self.states.role, self.states.state, self.states.level, [0, 0], self.states.inventory)

    def queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.network.send_command(command.command_string)

    def _is_command_pending(self, command_class) -> bool:
        """Vérifie si un type de commande précis est déjà dans la file d'attente."""
        return any(isinstance(cmd, command_class) for cmd in self.pending_commands)

    def _clean_floor(self) -> bool:
        if not self.vision_grid: return False
        floor = self.vision_grid[0]
        cleaned_something = False

        for item, qty in floor.items():
            if item != "player" and qty > 0:
                for _ in range(qty):
                    self.queue_command(TakeCommand(item))
                    cleaned_something = True
        return cleaned_something
