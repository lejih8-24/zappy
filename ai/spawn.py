#!/usr/bin/env python3
"""
Spawn multiple Zappy AI instances with a single command.
Usage: python3 ai/spawn.py -n TeamA -p 4242 [-h localhost] [-c 3]
"""

import argparse
import signal
import sys
import threading
from pathlib import Path
from subprocess import Popen, PIPE, STDOUT


def pipe_output(proc: Popen, index: int) -> None:
    prefix = f"[AI-{index}]"
    for line in proc.stdout:
        try:
            print(f"{prefix} {line}", end="", flush=True)
        except Exception:
            break


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Spawn multiple Zappy AIs", add_help=False)
    parser.add_argument("-n", dest="name", required=True, help="Team name")
    parser.add_argument("-p", dest="port", required=True, type=int, help="Server port")
    parser.add_argument("-h", dest="host", default="localhost", help="Server host")
    parser.add_argument("-c", dest="count", default=1, type=int, help="Number of AIs to spawn")
    parser.add_argument("--help", action="help")
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    script = Path(__file__).parent / "main.py"
    cmd = [sys.executable, str(script), "-n", args.name, "-p", str(args.port), "-h", args.host]

    procs: list[Popen] = []

    def shutdown(sig=None, frame=None):
        print("\n[spawn] Stopping all AIs...")
        for p in procs:
            p.terminate()
        sys.exit(0)

    signal.signal(signal.SIGINT, shutdown)
    signal.signal(signal.SIGTERM, shutdown)

    for i in range(args.count):
        proc = Popen(cmd, stdout=PIPE, stderr=STDOUT, text=True)
        procs.append(proc)
        threading.Thread(target=pipe_output, args=(proc, i + 1), daemon=True).start()
        print(f"[spawn] AI {i + 1}/{args.count} started (PID {proc.pid})")

    for proc in procs:
        proc.wait()


if __name__ == "__main__":
    main()
