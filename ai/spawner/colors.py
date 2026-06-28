##
## EPITECH PROJECT, 2026
## zappy
## File description:
## ANSI color helpers for spawn labels
##

_PALETTE = ["\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[91m"]
RESET    = "\033[0m"


def ai_color(index: int) -> str:
    return _PALETTE[(index - 1) % len(_PALETTE)]
