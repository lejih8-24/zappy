##
## EPITECH PROJECT, 2026
## zappy
## File description:
## display
##


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
        print(f"\r[broadcast] {msg[8:]}")
        return True
    if msg.startswith("eject:"):
        print(f"\r[ejected from dir {msg[6:].strip()}]")
        return True
    if msg.startswith("Current level:"):
        print(f"\r[elevation] Level {msg.split(':')[1].strip()}!")
        return True
    if msg == "dead":
        print("\r[dead] You starved.")
        return True
    return False


def handle_response(cmd: str, msg: str) -> None:
    if cmd == "Look":
        print(f"\rLook:\n{fmt_look(msg)}")
    elif cmd == "Inventory":
        print(f"\rInventory:\n{fmt_inventory(msg)}")
    else:
        print(f"\r{cmd}: {msg}")
