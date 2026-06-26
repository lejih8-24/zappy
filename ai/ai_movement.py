from constants import State, Role
from CommandModel import *
from pathfinding import find_path_to_closest
from typing import TYPE_CHECKING
from CommandModel import ForwardCommand, TurnRightCommand, TurnLeftCommand, TakeCommand, LookCommand

if TYPE_CHECKING:
    from ZappyAIModel import ZappyAI

class AINavigator:
    def __init__(self, ai_controller: 'ZappyAI'):
        self.ai = ai_controller

    def move_instructions(self, path: list, target_item: str):
        if path is not None:
            for action in path:
                if action == "Forward":
                    self.ai.queue_command(ForwardCommand())
                elif action == "Right":
                    self.ai.queue_command(TurnRightCommand())
                elif action == "Left":
                    self.ai.queue_command(TurnLeftCommand())

            if target_item:
                self.ai.queue_command(TakeCommand(target_item))
            self.ai.states.vision_grid = None
        else:
            self.ai.queue_command(ForwardCommand())
            self.ai.queue_command(LookCommand())
            self.ai.states.vision_grid = None