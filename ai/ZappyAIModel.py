#!/usr/local/bin/python

import time
from CommandModel import *
from BroadcastManager import BroadcastManager
from ai.btree.bt_nodes import ShouldReproduce, ActionFork
from ai_movement import AINavigator
from ai_states import AIState
from dashboard import Display
from logger import AILogger
from btree.bt_core import Selector, Sequence
from btree.bt_nodes import IsHungry, ActionSearchFood, ActionFarmStones, CanElevate, ActionGroupAndIncant, \
    HasMasterCall, ActionJoinMaster, ActionContributeStones
from constants import Role, State
from network import ServerEvent, ProtocolParser
import random


class ZappyAI:
    def __init__(self, network, team_name, map_x, map_y):
        self.id = str(uuid.uuid4())[:4]
        self.network = network
        self.map_x = map_x
        self.map_y = map_y
        self.logger = AILogger("./ai.logs", self.id)
        self.comms = BroadcastManager(self.id, token="AlphaNor_Zappy_26")
        self.display = Display()
        self.states = AIState(self.id, team_name)
        self.navigator = AINavigator(self)
        self.bt = self._build_behavior_tree()
        self.parser = ProtocolParser(self.comms)

        self.is_alive = True
        self.pending_commands = []

    def _build_behavior_tree(self):
        """Construit l'arbre d'intelligence de bas en haut."""

        survival_branch = Sequence([IsHungry(), ActionSearchFood()])

        fork_branch = Sequence([ShouldReproduce(), ActionFork()])

        elevation_branch = Sequence([CanElevate(), ActionGroupAndIncant()])

        farming_branch = ActionFarmStones()

        root = Selector([
            Sequence([IsHungry(), ActionSearchFood()]),
            Sequence([HasMasterCall(), ActionJoinMaster(), ActionContributeStones()]),
            Sequence([CanElevate(), ActionGroupAndIncant()]),
            ActionFarmStones()
        ])

        return root

    def run(self):
        while self.is_alive:
            responses = self.network.get_responses()
            for raw_response in responses:
                event = self.parser.parse(raw_response)

                if event.type != "IGNORE":
                    self._process_server_event(event)

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
                required_players = self.states.elevation_rules[self.states.level]["players"]
                if self.states.count_player_case() >= required_players:
                    return

                my_id = self.id

                if random.random() > 0.5:
                    self.logger.Warn(f"[COMMS] Conflit de Masters ! J'abandonne mon rituel pour rejoindre {sender_id}.")
                    self.states.is_master = False
                    self.states.master_direction = direction
                    self.states.last_master_id = sender_id
                else:
                    self.logger.Warn(f"[COMMS] Conflit avec {sender_id}, je maintiens mon rituel, c'est lui qui viendra.")
                return

            self.states.last_master_id = sender_id
            self.states.master_direction = direction

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
            self.states.is_master = False
            self.logger.Good(f"Succès ! Je suis niveau {self.states.level}")

        elif event.type == "BROADCAST":
            self._handle_broadcast(event.data["direction"], event.data["decoded"])

        elif event.type == "RESPONSE":
            self._handle_command_response(event.data["raw"])

    def _update_state_from_result(self, command, result):
        if isinstance(command, InventoryCommand) and isinstance(result, dict):
            self.states.inventory = result

        elif isinstance(command, LookCommand) and isinstance(result, list):
            self.states.vision_grid = result
            self._integrate_vision_to_map(result)

        elif isinstance(command, TakeCommand) and result is True:
            self.states.inventory[command.obj_name] = self.states.inventory.get(command.obj_name, 0) + 1

            current_pos = (self.states.pos_x, self.states.pos_y)
            if current_pos in self.states.world_map and command.obj_name in self.states.world_map[
                current_pos]:
                if self.states.world_map[current_pos][command.obj_name] > 0:
                    self.states.world_map[current_pos][command.obj_name] -= 1

        elif isinstance(command, SetCommand) and result is True:
            self.states.inventory[command.obj_name] = self.states.inventory.get(command.obj_name, 1) - 1

            current_pos = (self.states.pos_x, self.states.pos_y)
            if current_pos not in self.states.world_map:
                self.states.world_map[current_pos] = {}
            self.states.world_map[current_pos][command.obj_name] = self.states.world_map[current_pos].get(
                command.obj_name, 0) + 1

        elif isinstance(command, ForkCommand) and result is True:
            self.logger.Good("L'oeuf a été pondu avec succès ! Retour au travail.")

        elif isinstance(command, TurnRightCommand) and result is True:
            self.states.orientation = (self.states.orientation + 1) % 4

        elif isinstance(command, TurnLeftCommand) and result is True:
            self.states.orientation = (self.states.orientation - 1) % 4

        elif isinstance(command, ForwardCommand) and result is True:
            if self.states.orientation == 0:
                self.states.pos_y = (self.states.pos_y + 1) % self.map_y
            elif self.states.orientation == 1:
                self.states.pos_x = (self.states.pos_x + 1) % self.map_x
            elif self.states.orientation == 2:
                self.states.pos_y = (self.states.pos_y - 1) % self.map_y
            elif self.states.orientation == 3:
                self.states.pos_x = (self.states.pos_x - 1) % self.map_x

    def _decide_next_action(self):
        if len(self.pending_commands) >= 9:
            self.logger.Info(f"File d'attente pleine ({len(self.pending_commands)}/9). Attente des réponses réseau.")

        self.bt.tick(self)

        self.display.save_dashboard_state(self.states.name, self.states.role, self.states.state, self.states.level, [0, 0], self.states.inventory)

    def queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.network.send_command(command.command_string)

    def is_command_pending(self, command_class) -> bool:
        """Vérifie si un type de commande précis est déjà dans la file d'attente."""
        return any(isinstance(cmd, command_class) for cmd in self.pending_commands)

    def _clean_floor(self) -> bool:
        if not self.states.vision_grid: return False
        floor = self.states.vision_grid[0]
        cleaned_something = False

        for item, qty in floor.items():
            if item != "player" and qty > 0:
                for _ in range(qty):
                    self.queue_command(TakeCommand(item))
                    cleaned_something = True
        return cleaned_something

    def _integrate_vision_to_map(self, vision_list: list):
        """Transforme le tableau Look en coordonnées absolues et met à jour la carte mentale."""
        dx_abs = 0
        dy_abs = 0
        import math

        for i, tile_content in enumerate(vision_list):
            dy_rel = math.isqrt(i)
            dx_rel = i - (dy_rel * dy_rel + dy_rel)

            if self.states.orientation == 0:
                dx_abs, dy_abs = dx_rel, dy_rel
            elif self.states.orientation == 1:
                dx_abs, dy_abs = dy_rel, -dx_rel
            elif self.states.orientation == 2:
                dx_abs, dy_abs = -dx_rel, -dy_rel
            elif self.states.orientation == 3:
                dx_abs, dy_abs = -dy_rel, dx_rel

            target_x = (self.states.pos_x + dx_abs) % self.map_x
            target_y = (self.states.pos_y + dy_abs) % self.map_y

            self.states.world_map[(target_x, target_y)] = tile_content
