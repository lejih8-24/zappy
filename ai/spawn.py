#!/usr/bin/env python3
"""Spawn multiple Zappy AIs. Usage: python3 ai/spawn.py -n TeamA -p 4242 [-h localhost] [-c 3] [-d 0.5] [-q]"""

import argparse
import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from spawner.output import print_header
from spawner.runner import run


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Spawn multiple Zappy AIs", add_help=False)
    p.add_argument("-n", dest="name",  required=True,              help="Team name")
    p.add_argument("-p", dest="port",  required=True,  type=int,   help="Server port")
    p.add_argument("-h", dest="host",  default="localhost",         help="Server host")
    p.add_argument("-c", dest="count", default=1,      type=int,   help="Number of AIs")
    p.add_argument("-d", dest="delay", default=0.0,    type=float, help="Delay between spawns (s)")
    p.add_argument("-q", dest="quiet", action="store_true",         help="Suppress AI output")
    p.add_argument("--help", action="help")
    return p.parse_args()


def main() -> None:
    args = parse_args()
    print_header(args)
    run(args)


if __name__ == "__main__":
    main()
