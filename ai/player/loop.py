##
## EPITECH PROJECT, 2026
## zappy
## File description:
## loop
##

import select
import sys

from network import NetworkClient
from player.commands import HELP, KEY_MAP, PROMPT_CMDS
from player.display import handle_event, handle_response, rprint
from player.terminal import RawTerminal


def _drain_server(client: NetworkClient, pending: list[str]) -> bool:
    try:
        chunk = client.socket.recv(4096).decode("utf-8")
    except BlockingIOError:
        return True
    if not chunk:
        rprint("[server closed]")
        return False

    client._read_buffer += chunk
    messages, client._read_buffer = client._read_buffer.rsplit("\n", 1) if "\n" in client._read_buffer else ("", client._read_buffer)

    for msg in messages.split("\n"):
        if not msg:
            continue
        if handle_event(msg):
            if msg == "dead":
                return False
            continue
        if pending:
            client.max_requests = max(0, client.max_requests - 1)
            handle_response(pending.pop(0), msg)
        else:
            rprint(f"[server] {msg}")
    return True


def _handle_key(client: NetworkClient, pending: list[str], term: RawTerminal) -> bool:
    key = term.read_key()

    if key in ("\x03", "\x1b"):
        return False

    if key in ("?", "h"):
        rprint(HELP)
        return True

    cmd = KEY_MAP.get(key)

    if cmd is None and key in PROMPT_CMDS:
        base, label = PROMPT_CMDS[key]
        value = term.prompt(f"\r{base} {label}> ")
        if value:
            cmd = f"{base} {value}"

    if cmd:
        client.send_command(cmd)
        pending.append(cmd.split()[0])
        rprint(f"> {cmd}")

    return True


def run(client: NetworkClient, term: RawTerminal) -> None:
    pending: list[str] = []
    stdin_fd = sys.stdin.fileno()
    rprint(HELP)

    while True:
        r, _, _ = select.select([stdin_fd, client.socket], [], [], 0.05)

        if client.socket in r and not _drain_server(client, pending):
            break
        if stdin_fd in r and not _handle_key(client, pending, term):
            break
