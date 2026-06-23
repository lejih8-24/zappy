##
## EPITECH PROJECT, 2026
## zappy
## File description:
## terminal
##

import select
import sys
import termios
import tty


class RawTerminal:
    def __enter__(self):
        self._fd = sys.stdin.fileno()
        self._saved = termios.tcgetattr(self._fd)
        tty.setraw(self._fd)
        return self

    def __exit__(self, *_):
        termios.tcsetattr(self._fd, termios.TCSADRAIN, self._saved)

    def read_key(self) -> str:
        ch = sys.stdin.read(1)
        if ch == "\x1b":
            r, _, _ = select.select([sys.stdin], [], [], 0.1)
            if r:
                return ch + sys.stdin.read(2)
        return ch

    def prompt(self, msg: str) -> str:
        termios.tcsetattr(self._fd, termios.TCSADRAIN, self._saved)
        try:
            return input(msg)
        finally:
            tty.setraw(self._fd)
