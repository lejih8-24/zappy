from constants import State, Role
from CommandModel import *
from pathfinding import find_path_to_closest
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from ZappyAIModel import ZappyAI

class AIStatesMixin:
    """Contient uniquement la logique des états de l'IA."""

    def _state_survival(self: 'ZappyAI'):
        if not self.vision_grid:
            if not self._is_command_pending(LookCommand):
                self._queue_command(LookCommand())
            return
        path = find_path_to_closest(self.vision_grid, "food")
        self._move_instructions(path, "food")
        if not self._is_command_pending(InventoryCommand):
            self._queue_command(InventoryCommand())

    def _state_farming(self: 'ZappyAI'):
        if not self.vision_grid:
            if not self._is_command_pending(LookCommand):
                self._queue_command(LookCommand())
            return

        target_mat = self._get_needed_material()

        if not target_mat:
            print("Toutes les ressources sont réunies. Passage en mode GROUPING.")
            self.state = State.GROUPING
            return

        path_to_mats = find_path_to_closest(self.vision_grid, target_mat)

        if path_to_mats is not None:
            self._move_instructions(path_to_mats, target_mat)
        else:
            path_to_food = find_path_to_closest(self.vision_grid, "food")

            if path_to_food is not None and self.inventory.get("food", 0) < 40:
                self._move_instructions(path_to_food, "food")
            else:
                self._queue_command(ForwardCommand())
                self._queue_command(LookCommand())
                if not self._is_command_pending(InventoryCommand):
                    self._queue_command(InventoryCommand())
                self.vision_grid = None

    def _state_grouping(self: 'ZappyAI'):
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

    def _state_contributing(self: 'ZappyAI'):
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

    def _state_incantation(self: 'ZappyAI'):
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

    def _state_forking(self: 'ZappyAI'):
        """L'IA s'immobilise pour pondre un œuf."""
        if self._is_command_pending(ForkCommand):
            return

        print("[DEBUG] Envoi de la commande Fork au serveur (blocage de 42 ticks)...")
        self._queue_command(ForkCommand())