##
## EPITECH PROJECT, 2026
## zappy
## File description:
## display
##

import sys


def rprint(text: str = "") -> None:
    sys.stdout.write(text.replace("\n", "\r\n") + "\r\n")
    sys.stdout.flush()


def fmt_look(raw: str) -> str:
    raw = raw.strip().strip("[]")
    tiles = [t.strip() or "empty" for t in raw.split(",")]
    lines = [f"  [{'here' if i == 0 else f'tile {i}'}] {t}" for i, t in enumerate(tiles)]
    return "\n".join(lines)


def fmt_inventory(raw: str) -> str:
    raw = raw.strip().strip("[]")
    return "  " + "  |  ".join(p.strip() for p in raw.split(","))


def handle_event(msg: str) -> bool:
    if msg.startswith("message "):
        rprint(f"[broadcast] {msg[8:]}")
        return True
    if msg.startswith("eject:"):
        rprint(f"[ejected from dir {msg[6:].strip()}]")
        return True
    if msg.startswith("Current level:"):
        rprint(f"[elevation] Level {msg.split(':')[1].strip()}!")
        return True
    if msg == "dead":
        rprint("[dead] You starved.")
        return True
    return False


def handle_response(cmd: str, msg: str) -> None:
    if cmd == "Look":
        rprint(f"Look:\n{fmt_look(msg)}")
    elif cmd == "Inventory":
        rprint(f"Inventory:\n{fmt_inventory(msg)}")
    else:
        rprint(f"{cmd}: {msg}")
