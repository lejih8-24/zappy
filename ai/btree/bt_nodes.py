from bt_core import Node, NodeStatus
from ..CommandModel import LookCommand, ForwardCommand, TurnLeftCommand, TurnRightCommand, SetCommand
from ..pathfinding import find_path_to_closest


class IsHungry(Node):
    """Vérifie si le drone est en danger de mort par famine."""
    def tick(self, ai) -> NodeStatus:
        if ai.drone_state.inventory.get("food", 0) < 15:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class CanElevate(Node):
    """Vérifie si le drone possède toutes les ressources pour passer au niveau supérieur."""
    def tick(self, ai) -> NodeStatus:
        if ai.drone_state.can_elevate():
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class HasMasterCall(Node):
    """Vérifie si le drone a entendu un appel valide d'un Master."""
    def tick(self, ai) -> NodeStatus:
        if ai.drone_state.master_direction is not None or ai.drone_state.ready_for_incantation:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class ActionSearchFood(Node):
    """Cherche et ramasse de la nourriture en priorité."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if not ai.drone_state.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.drone_state.vision_grid, "food")

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

        target_mat = ai.drone_state.get_needed_material()

        if not target_mat:
            return NodeStatus.SUCCESS

        if not ai.drone_state.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.drone_state.vision_grid, target_mat)

        if path is not None:
            ai.navigator.move_instructions(path, target_mat)
        else:
            ai.navigator.move_instructions(None, None)

        return NodeStatus.RUNNING


class ActionJoinMaster(Node):
    """Fait avancer le drone vers la source du signal du Master."""

    def tick(self, ai) -> NodeStatus:
        if ai.drone_state.ready_for_incantation:
            return NodeStatus.SUCCESS

        if ai.drone_state.master_direction is None:
            return NodeStatus.FAILURE

        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        k = ai.drone_state.master_direction
        ai.drone_state.master_direction = None

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
        for item, qty in list(ai.drone_state.inventory.items()):
            if item != "food" and qty > 0:
                ai.queue_command(SetCommand(item))
                ai.drone_state.inventory[item] -= 1
                dropped_something = True
                break

        if dropped_something:
            return NodeStatus.RUNNING

        ai.logger.Info("[BT] Contribution terminée. En attente de l'incantation...")
        return NodeStatus.RUNNING
