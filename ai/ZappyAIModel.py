#!/usr/local/bin/python

import time
import uuid
import random

from CommandModel import *
from BroadcastManager import BroadcastManager
from btree.bt_nodes import ShouldReproduce, ActionFork, IsHungry, ActionSearchFood, ActionFarmStones, CanElevate, \
    ActionGroupAndIncant, \
    HasMasterCall, ActionJoinMaster, ActionContributeStones
from ai_movement import AINavigator
from ai_states import AIState
from dashboard import Display
from logger import AILogger
from btree.bt_core import Selector, Sequence
from constants import Role, State
from network import ServerEvent, ProtocolParser


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

        self.cycle_count = 0
        self.last_inventory_cycle = 0
        self.last_interaction_cycle = 0

    def _build_behavior_tree(self):
        return Selector([
            Sequence([IsHungry(), ActionSearchFood()]),
            Sequence([HasMasterCall(), ActionJoinMaster(), ActionContributeStones()]),
            Sequence([CanElevate(), ActionGroupAndIncant()]),
            ActionFarmStones()
        ])

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

                if self.id > sender_id:
                    self.logger.Warn(f"[COMMS] Conflit résolu ! {sender_id} est prioritaire. J'abandonne mon rituel.")
                    self.states.is_master = False
                    self.states.master_direction = direction
                    self.states.last_master_id = sender_id
                else:
                    self.logger.Warn(
                        f"[COMMS] Conflit avec {sender_id}. Mon ID ({self.id}) est prioritaire, je maintiens.")
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
        """Flux FIFO strictement respecté : On POP toujours la commande en premier !"""
        self.last_interaction_time = time.time()

        if not self.pending_commands:
            self.logger.Error(f"Réponse inattendue (aucune commande en attente) : {raw_message}")
            return

        current_command = self.pending_commands.pop(0)

        if isinstance(current_command, IncantationCommand) and raw_message == "ko":
            self.logger.Warn("L'incantation a échoué (un joueur a bougé ou une pierre manque).")
            self.states.ready_for_incantation = False
            self.states.is_master = False
            self.states.clear_master_call()
            return

        result = current_command.parse_response(raw_message)
        self._update_state_from_result(current_command, result)

    def _process_server_event(self, event: ServerEvent):
        if event.type == "DEAD":
            self.logger.Error("L'IA est morte de faim.")
            self.is_alive = False

        elif event.type == "EJECT":
            self.logger.Warn(f"Je me suis fait éjecter depuis la direction {event.data['direction']} !")
            self.pending_commands.clear()
            self.states.vision_grid = None

        elif event.type == "ELEVATION_START":
            self.logger.Good("Début de l'incantation, je suis immobilisé !")
            self.states.ready_for_incantation = True

        elif event.type == "ELEVATION_SUCCESS":
            self.states.level = event.data["level"]
            self.states.is_master = False
            self.states.ready_for_incantation = False
            self.states.clear_master_call()
            self.logger.Good(f"Succès ! Je suis niveau {self.states.level}")

            if self.pending_commands and isinstance(self.pending_commands[0], IncantationCommand):
                self.pending_commands.pop(0)

            self.queue_command(InventoryCommand())

        elif event.type == "BROADCAST":
            self._handle_broadcast(event.data["direction"], event.data["decoded"])

        elif event.type == "RESPONSE":
            self._handle_command_response(event.data["raw"])

    def _update_state_from_result(self, command, result):
        if isinstance(command, InventoryCommand) and isinstance(result, dict):
            self.states.inventory = result
            self.last_inventory_cycle = self.cycle_count

        elif isinstance(command, LookCommand) and isinstance(result, list):
            self.states.vision_grid = result

        elif isinstance(command, TakeCommand) and result is True:
            self.states.inventory[command.obj_name] = self.states.inventory.get(command.obj_name, 0) + 1
            self.states.vision_grid = None

        elif isinstance(command, SetCommand) and result is True:
            self.states.inventory[command.obj_name] = self.states.inventory.get(command.obj_name, 1) - 1
            self.states.vision_grid = None

        elif isinstance(command, ForkCommand) and result is True:
            self.logger.Good("L'oeuf a été pondu avec succès ! Retour au travail.")

        elif isinstance(command, TurnRightCommand) and result is True:
            self.states.orientation = (self.states.orientation + 1) % 4
            self.states.vision_grid = None

        elif isinstance(command, TurnLeftCommand) and result is True:
            self.states.orientation = (self.states.orientation - 1) % 4
            self.states.vision_grid = None

        elif isinstance(command, ForwardCommand) and result is True:
            if self.states.orientation == 0:
                self.states.pos_y = (self.states.pos_y + 1) % self.map_y
            elif self.states.orientation == 1:
                self.states.pos_x = (self.states.pos_x + 1) % self.map_x
            elif self.states.orientation == 2:
                self.states.pos_y = (self.states.pos_y - 1) % self.map_y
            elif self.states.orientation == 3:
                self.states.pos_x = (self.states.pos_x - 1) % self.map_x
            self.states.vision_grid = None

    def _decide_next_action(self):
        self.cycle_count += 1
        if getattr(self.states, 'ready_for_incantation', False):
            return

        if len(self.pending_commands) > 0 and (self.cycle_count - self.last_interaction_cycle) > 700:
            self.logger.Warn(f"[TIMEOUT] {self.id} : Le serveur ne répond plus. Déconnexion.")
            self.is_alive = False
            return

        if (self.cycle_count - self.last_inventory_cycle) > 25 and not self.is_command_pending(InventoryCommand):
            self.queue_command(InventoryCommand())
            return

        if len(self.pending_commands) >= 9:
            return

        self.bt.tick(self)

        if time.time() % 1 < 0.05:
            self.display.save_dashboard_state(self.states.name, Role.Explorer, State.FARMING, self.states.level,
                                              [self.states.pos_x, self.states.pos_y], self.states.inventory)

    def queue_command(self, command):
        if len(self.pending_commands) < 9:
            self.pending_commands.append(command)
            self.last_interaction_cycle = self.cycle_count
            self.network.send_command(command.command_string)

    def is_command_pending(self, command_class) -> bool:
        return any(isinstance(cmd, command_class) for cmd in self.pending_commands)