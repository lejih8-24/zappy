##
## EPITECH PROJECT, 2026
## zappy
## File description:
## terminal
##

import os
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

    def _read1(self) -> str:
        return os.read(self._fd, 1).decode("utf-8", errors="replace")

    def read_key(self) -> str:
        ch = self._read1()
        if ch != "\x1b":
            return ch
        r, _, _ = select.select([self._fd], [], [], 0.1)
        if not r:
            return ch
        ch2 = self._read1()
        if ch2 not in ("[", "O"):
            return ch + ch2
        r, _, _ = select.select([self._fd], [], [], 0.1)
        if not r:
            return ch + ch2
        return ch + ch2 + self._read1()

    def prompt(self, msg: str) -> str:
        termios.tcsetattr(self._fd, termios.TCSADRAIN, self._saved)
        try:
            return input(msg)
        finally:
            tty.setraw(self._fd)
