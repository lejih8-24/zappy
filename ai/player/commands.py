##
## EPITECH PROJECT, 2026
## zappy
## File description:
## commands
##

HELP = """
  Arrow Up / Z   Forward
  Arrow Left / Q Turn Left
  Arrow Right    Turn Right

  u  Incantation     i  Inventory   l   Look
  b  Broadcast       t  Take        s   Set
  f  Fork            e  Eject       n   Connect_nbr

  ?  Help            Ctrl+C / Esc   Quit
"""

OBJECTS = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

KEY_MAP: dict[str, str] = {
    "\x1b[A": "Forward",  "z": "Forward",
    "\x1b[D": "Left",     "q": "Left",
    "\x1b[C": "Right",
    "l": "Look",
    "i": "Inventory",
    "u": "Incantation",
    "f": "Fork",
    "e": "Eject",
    "n": "Connect_nbr",
}

PROMPT_CMDS: dict[str, tuple[str, str]] = {
    "b": ("Broadcast", "text"),
    "t": ("Take",      "object"),
    "s": ("Set",       "object"),
}
