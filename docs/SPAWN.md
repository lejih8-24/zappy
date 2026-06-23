# Multi-AI Spawner - ai/spawn.py

Launches multiple AI instances in parallel against a running Zappy server.
Each AI runs as a separate process with its output color-coded by index.

## Usage

```bash
python3 ai/spawn.py -n <team> -p <port> [options]
```

**Arguments:**

| Flag | Required | Default | Description |
|------|----------|---------|-------------|
| `-n` | yes | - | Team name |
| `-p` | yes | - | Server port |
| `-h` | no | localhost | Server host |
| `-c` | no | 1 | Number of AIs to spawn |
| `-d` | no | 0.0 | Delay in seconds between each spawn |
| `-q` | no | false | Quiet mode - suppress AI output |

**Examples:**

```bash
# Spawn 3 AIs on the default team
python3 ai/spawn.py -n TeamA -p 4242 -c 3

# Spawn 6 AIs with a 0.5s stagger and suppress their output
python3 ai/spawn.py -n TeamA -p 4242 -c 6 -d 0.5 -q

# Full server + spawner setup
./build/zappy_server -p 4242 -x 10 -y 10 -n TeamA -c 10 -f 100
python3 ai/spawn.py -n TeamA -p 4242 -c 6 -d 0.3
```

## Output

Each AI process is prefixed with a color-coded tag so you can tell them apart:

```
[spawn] localhost:4242  team=TeamA  count=3  delay=0.0s
[AI-1] Connected to server...
[AI-2] Connected to server...
[AI-3] Connected to server...
[AI-1] State: FARMING
...
[AI-2] exited (code 0)
```

`Ctrl+C` sends `SIGTERM` to all child processes and exits cleanly.

## Architecture

```
ai/spawn.py             - entry point: parse args, call run()
ai/spawner/
    runner.py           - spawn loop: Popen per AI, SIGINT/SIGTERM handler, wait
    output.py           - pipe_output() thread per process, print_header()
    colors.py           - ANSI color helpers per AI index
```

### Runner

`run()` launches `count` subprocesses each running `ai/main.py` with the given args.
A background thread per process calls `pipe_output()` to forward stdout line-by-line.
The optional `-d` delay staggers spawns so the server is not hit with simultaneous connections.
`SIGINT` and `SIGTERM` are caught to terminate all children before exiting.

### Quiet mode

With `-q`, `pipe_output` discards all lines but still waits for the process to finish and prints the exit code. Useful for load testing without flooding the terminal.
