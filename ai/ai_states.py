from constants import State, Role
from CommandModel import *
from pathfinding import find_path_to_closest
from typing import TYPE_CHECKING

# drone_state.py
from constants import Role, State, ELEVATION_RULES

class AIState:
    def __init__(self, name, team_name):
        self.name = name
        self.team_name = team_name
        self.level = 1
        self.inventory = {"food": 10, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0,
                          "thystame": 0}
        self.elevation_rules = ELEVATION_RULES

        self.role = Role.Explorer
        self.state = State.FARMING
        self.vision_grid = None
        self.target_direction = None

        self.is_master = False
        self.master_direction = None
        self.last_master_id = None
        self.ready_for_incantation = False

    def can_elevate(self) -> bool:
        rules = self.elevation_rules[self.level]
        for stone, required_qty in rules["stones"].items():
            if self.inventory.get(stone, 0) < required_qty:
                return False
        return True

    def get_needed_material(self) -> str | None:
        if self.level >= 8: return None
        rule = self.elevation_rules[self.level]["stones"]
        for stone, required_qty in rule.items():
            if self.inventory.get(stone, 0) < required_qty:
                return stone
        return None

    def count_player_case(self) -> int:
        if not self.vision_grid: return 0
        return self.vision_grid[0].get('player', 0)

    def is_floor_perfect(self) -> bool:
        if not self.vision_grid: return False
        floor = self.vision_grid[0]
        rules = self.elevation_rules[self.level]

        if floor.get("player", 0) != rules["players"]: return False
        for item, qty in floor.items():
            if item == "player" or qty == 0: continue
            if qty != rules["stones"].get(item, 0): return False
        for stone, required_qty in rules["stones"].items():
            if floor.get(stone, 0) != required_qty: return False
        return True

    def clear_master_call(self):
        """Réinitialise la mémoire des appels réseau pour revenir à l'état normal."""
        self.master_direction = None
        self.last_master_id = None
        self.ready_for_incantation = False
