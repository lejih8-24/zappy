from bt_core import Node, NodeStatus
from ..CommandModel import LookCommand


class IsHungry(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.inventory.get("food", 0) < 15:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class HasEnoughFood(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.inventory.get("food", 0) > 35:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class CanElevate(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.can_elevate():
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class ActionSearchFood(Node):
    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if not ai.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        return NodeStatus.RUNNING


class ActionFarmStones(Node):
    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if not ai.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        print("Recherche de pierres précieuses...")
        return NodeStatus.RUNNING