# Manual Player - ai/play.py

Interactive terminal controller to play as a trantorian manually instead of using the AI.

## Usage

```bash
python3 ai/play.py -n <team> -p <port> [-h <host>]
```

**Arguments:**

| Flag | Required | Default | Description |
|------|----------|---------|-------------|
| `-n` | yes | - | Team name |
| `-p` | yes | - | Server port |
| `-h` | no | localhost | Server host |

**Example:**

```bash
./build/zappy_server -p 4242 -x 10 -y 10 -n TeamA -c 5 -f 100
python3 ai/play.py -n TeamA -p 4242
```

## Controls

| Key | Action |
|-----|--------|
| Arrow Up / `w` / `z` | Move forward |
| Arrow Left / `a` / `q` | Turn left |
| Arrow Right / `d` | Turn right |
| `l` | Look |
| `i` | Inventory |
| `u` | Incantation |
| `b` | Broadcast (prompts for text) |
| `t` | Take object (prompts for object name) |
| `s` | Set object (prompts for object name) |
| `f` | Fork |
| `e` | Eject |
| `n` | Connect_nbr |
| `?` or `h` | Show help |
| `Ctrl+C` / `Esc` | Quit |

Objects: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`

## Architecture

```
ai/play.py              - entry point: parse args, connect, handshake, run loop
ai/player/
    terminal.py         - RawTerminal context manager (termios raw mode, key reading)
    commands.py         - KEY_MAP, PROMPT_CMDS, OBJECTS, HELP string (pure data)
    display.py          - response formatters: fmt_look(), fmt_inventory(), handle_event()
    loop.py             - select loop on stdin + socket, dispatches input and server messages
```

### RawTerminal

Puts the terminal in raw mode for single-keypress input. Restored automatically on exit (even on crash). Handles 1-3 byte escape sequences for arrow keys, with macOS (`\x1bO*`) and Linux (`\x1b[*`) variants.

`prompt(msg)` temporarily restores cooked mode so the user can type a full line (used for Broadcast text, Take/Set object).

### Loop

`select` on both stdin and the server socket with a 50ms timeout so neither blocks the other. Incoming server messages are matched against the pending command queue to display formatted responses. Unmatched messages (broadcasts, ejects, level-up, dead) are handled as events.
