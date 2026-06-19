from constants import State, Role
from CommandModel import *
from pathfinding import find_path_to_closest
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from ZappyAIModel import ZappyAI

class AIMovementMixin:
    def _move_instructions(self: 'ZappyAI', path: list, target_item: str):
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

    def _integrate_vision_to_map(self: 'ZappyAI', vision_list: list):
        """Transforme le tableau Look en coordonnées absolues et met à jour la carte mentale."""
        dx_abs = 0
        dy_abs = 0
        import math

        for i, tile_content in enumerate(vision_list):
            dy_rel = math.isqrt(i)
            dx_rel = i - (dy_rel * dy_rel + dy_rel)

            if self.orientation == 0:
                dx_abs, dy_abs = dx_rel, dy_rel
            elif self.orientation == 1:
                dx_abs, dy_abs = dy_rel, -dx_rel
            elif self.orientation == 2:
                dx_abs, dy_abs = -dx_rel, -dy_rel
            elif self.orientation == 3:
                dx_abs, dy_abs = -dy_rel, dx_rel

            target_x = (self.pos_x + dx_abs) % self.map_x
            target_y = (self.pos_y + dy_abs) % self.map_y
            target_pos = (target_x, target_y)

            old_tile = self.world_map.get(target_pos, {})
            self.world_map[(target_x, target_y)] = tile_content

            for resource, qty in tile_content.items():
                if resource != "player" and resource != "food" and qty > old_tile.get(resource, 0):
                    # On ne spamme pas le réseau si on a déjà un Broadcast en attente
                    if not self._is_command_pending(BroadcastCommand):
                        # Formatage du paquet : "DATA_SHARE", data="X|Y|resource|qty"
                        data_payload = f"{target_x}|{target_y}|{resource}|{qty}"
                        msg = self.comms.format_message("ALL", self.level, self.role.name, self.state.name, f"DATA_SHARE({data_payload})")
                        self._queue_command(BroadcastCommand(self.comms.token, msg))
