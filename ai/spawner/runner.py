##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Process lifecycle: spawn, monitor, shutdown
##

import argparse
import signal
import sys
import time
import threading
from pathlib import Path
from subprocess import Popen, PIPE, STDOUT

from spawner.colors import RESET, ai_color
from spawner.output import pipe_output


def _build_cmd(args: argparse.Namespace) -> list[str]:
    script = Path(__file__).parent.parent / "main.py"
    return [sys.executable, str(script), "-n", args.name, "-p", str(args.port), "-h", args.host]


def run(args: argparse.Namespace) -> None:
    cmd   = _build_cmd(args)
    procs: list[Popen] = []

    def shutdown(sig=None, frame=None):
        print("\n[spawn] Stopping all AIs...")
        for p in procs:
            p.terminate()
        sys.exit(0)

    signal.signal(signal.SIGINT, shutdown)
    signal.signal(signal.SIGTERM, shutdown)

    threads: list[threading.Thread] = []
    for i in range(args.count):
        proc = Popen(cmd, stdout=PIPE, stderr=STDOUT, text=True)
        procs.append(proc)
        t = threading.Thread(target=pipe_output, args=(proc, i + 1, args.quiet), daemon=True)
        t.start()
        threads.append(t)
        print(f"{ai_color(i + 1)}[spawn]{RESET} AI {i + 1}/{args.count} started (PID {proc.pid})")
        if args.delay > 0 and i < args.count - 1:
            time.sleep(args.delay)

    for proc in procs:
        proc.wait()

    for t in threads:
        t.join(timeout=1.0)
