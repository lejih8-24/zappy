from .bt_core import Node, NodeStatus
from CommandModel import LookCommand, ForwardCommand, TurnLeftCommand, TurnRightCommand, SetCommand, BroadcastCommand, \
    IncantationCommand, TakeCommand
from pathfinding import find_path_to_closest


class IsHungry(Node):
    """Vérifie si le drone est en danger de mort par famine."""
    def tick(self, ai) -> NodeStatus:
        if ai.states.inventory.get("food", 0) < 15:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class CanElevate(Node):
    """Vérifie si le drone possède toutes les ressources pour passer au niveau supérieur."""
    def tick(self, ai) -> NodeStatus:
        if ai.states.can_elevate():
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class HasMasterCall(Node):
    """Vérifie si le drone a entendu un appel valide d'un Master."""
    def tick(self, ai) -> NodeStatus:
        if ai.states.master_direction is not None or ai.states.ready_for_incantation:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class ActionSearchFood(Node):
    """Cherche et ramasse de la nourriture en priorité."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.states.vision_grid, "food")

        if path is not None:
            ai.navigator.move_instructions(path, "food")
        else:
            ai.navigator.move_instructions(None, None)

        return NodeStatus.RUNNING


class ActionFarmStones(Node):
    """Cherche et ramasse la pierre manquante pour l'élévation."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        target_mat = ai.states.get_needed_material()

        if not target_mat:
            return NodeStatus.SUCCESS

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.states.vision_grid, target_mat)

        if path is not None:
            ai.navigator.move_instructions(path, target_mat)
        else:
            ai.navigator.move_instructions(None, None)

        return NodeStatus.RUNNING


class ActionJoinMaster(Node):
    """Fait avancer le drone vers la source du signal du Master."""

    def tick(self, ai) -> NodeStatus:
        if ai.states.ready_for_incantation:
            return NodeStatus.SUCCESS

        if ai.states.master_direction is None:
            return NodeStatus.FAILURE

        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        k = ai.states.master_direction
        ai.states.master_direction = None

        if k == 0:
            ai.logger.Info("[BT] Arrivé sur la case du Master.")
            return NodeStatus.SUCCESS

        if k in [1, 2, 8]:
            ai.queue_command(ForwardCommand())
        elif k in [3, 4]:
            ai.queue_command(TurnLeftCommand())
        elif k in [5, 6, 7]:
            ai.queue_command(TurnRightCommand())

        return NodeStatus.RUNNING


class ActionContributeStones(Node):
    """Vide l'inventaire des pierres sur la case du Master."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        dropped_something = False
        for item, qty in list(ai.states.inventory.items()):
            if item != "food" and qty > 0:
                ai.queue_command(SetCommand(item))
                ai.states.inventory[item] -= 1
                dropped_something = True
                break

        if dropped_something:
            return NodeStatus.RUNNING

        ai.logger.Info("[BT] Contribution terminée. En attente de l'incantation...")
        return NodeStatus.RUNNING


class ActionGroupAndIncant(Node):
    """Le chef d'orchestre : Regroupe l'équipe, prépare la case et lance l'incantation."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        ai.states.is_master = True

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        floor = ai.states.vision_grid[0]
        players_on_tile = ai.states.count_player_case()
        rules = ai.states.elevation_rules[ai.states.level]
        required_players = rules["players"]

        if required_players > 1:
            if players_on_tile < required_players:
                if not ai.is_command_pending(BroadcastCommand):
                    msg = ai.comms.format_message("ALL", ai.states.level, "Master", "GROUPING", "INCANTATION_CALL")
                    ai.queue_command(BroadcastCommand(ai.id, msg))

                ai.states.vision_grid = None
                return NodeStatus.RUNNING

        msg = ai.comms.format_message("ALL", ai.states.level, "Master", "INCANTATION", "INCANTATION_STARTING")
        ai.queue_command(BroadcastCommand(ai.id, msg))

        if ai.states.is_floor_perfect():
            ai.logger.Good(f"[MASTER] Case parfaite ! Lancement du rituel (Niveau {ai.states.level} -> {ai.states.level + 1}).")
            ai.queue_command(IncantationCommand())
            return NodeStatus.RUNNING

        for item, qty in floor.items():
            if item == "player":
                continue
            required_qty = rules["stones"].get(item, 0)

            if qty > required_qty:
                ai.queue_command(TakeCommand(item))
                ai.states.vision_grid = None
                return NodeStatus.RUNNING

        for stone, required_qty in rules["stones"].items():
            current_on_floor = floor.get(stone, 0)
            if current_on_floor < required_qty:
                ai.queue_command(SetCommand(stone))
                ai.states.inventory[stone] -= 1
                ai.states.vision_grid = None
                return NodeStatus.RUNNING

        return NodeStatus.RUNNING
