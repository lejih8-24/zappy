#!/usr/bin/env python3
"""Manual Zappy player. Usage: python3 ai/play.py -n TeamA -p 4242 [-h localhost]"""

import argparse
import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from network import NetworkClient
from player.loop import run
from player.terminal import RawTerminal


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Manual Zappy player", add_help=False)
    p.add_argument("-n", dest="name", required=True)
    p.add_argument("-p", dest="port", required=True, type=int)
    p.add_argument("-h", dest="host", default="localhost")
    p.add_argument("--help", action="help")
    return p.parse_args()


def handshake(client: NetworkClient, team: str) -> None:
    step = 0
    while step < 3:
        for msg in client.get_responses():
            if step == 0 and msg == "WELCOME":
                client.send_command(team)
                step = 1
            elif step == 1:
                if msg == "ko":
                    print("Server rejected connection (no slots or unknown team).")
                    sys.exit(1)
                try:
                    slots = int(msg)
                    print(f"Slots remaining: {slots}")
                    step = 2
                except ValueError:
                    pass
            elif step == 2 and len(msg.split()) >= 2:
                w, h = msg.split()[:2]
                print(f"Map: {w}x{h}")
                step = 3


def main() -> None:
    args = parse_args()
    client = NetworkClient(args.host, args.port)
    client.connect()
    handshake(client, args.name)
    try:
        with RawTerminal() as term:
            run(client, term)
    finally:
        client.close()
        print("Disconnected.")


if __name__ == "__main__":
    main()
