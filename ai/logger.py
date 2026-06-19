from colorama import Fore, Back, Style
from enum import Enum

class Level(Enum):
    GOOD = 0
    WARN = 1
    ERROR = 2
    CHECK = 3


class AILogger:
    def __init__(self, write_mode: str | None):
        self.last_log = ""
        self.can_write = True if write_mode is not None else False
        if self.can_write:
            self.file = open(write_mode, "a")

    def __del__(self):
        self.file.close()

    def Good(self, text: str):
        if self.last_log == text:
            return
        print(Back.GREEN + f"[GOOD]: {text}")
        self.last_log = text
        if self.can_write:
            self.file.write(text)

    def Warn(self, text: str):
        if self.last_log == text:
            return
        print(Back.YELLOW + f"[WARN]: {text}")
        self.last_log = text
        if self.can_write:
            self.file.write(text)

    def Error(self, text: str):
        if self.last_log == text:
            return
        print(Back.RED + f"[ERROR]: {text}")
        self.last_log = text
        if self.can_write:
            self.file.write(text)

    def Info(self, text: str):
        if self.last_log == text:
            return
        print(Back.MAGENTA + f"[INFO]: {text}")
        self.last_log = text
        if self.can_write:
            self.file.write(text)
