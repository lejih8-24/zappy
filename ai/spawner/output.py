##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Per-process output piping and config display
##

import argparse
from subprocess import Popen

from spawner.colors import RESET, ai_color


def pipe_output(proc: Popen, index: int, quiet: bool = False) -> None:
    prefix = f"{ai_color(index)}[AI-{index}]{RESET}"
    for line in proc.stdout:
        if quiet:
            continue
        try:
            print(f"{prefix} {line}", end="", flush=True)
        except Exception:
            break
    code = proc.wait()
    try:
        print(f"{prefix} exited (code {code})", flush=True)
    except Exception:
        pass


def print_header(args: argparse.Namespace) -> None:
    print(f"[spawn] {args.host}:{args.port}  team={args.name}  count={args.count}  delay={args.delay}s")
