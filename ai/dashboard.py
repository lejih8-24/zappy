import os
import time
import json
import curses


def draw_dashboard(stdscr):
    # Configuration de curses
    curses.curs_set(0)
    stdscr.nodelay(True)
    stdscr.timeout(100)

    if curses.has_colors():
        curses.start_color()
        curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
        curses.init_pair(2, curses.COLOR_CYAN, curses.COLOR_BLACK)
        curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)
        curses.init_pair(4, curses.COLOR_YELLOW, curses.COLOR_BLACK)

    stats_dir = ".zappy_stats"

    while True:
        stdscr.clear()
        stdscr.addstr(0, 0, "╔══════════════════════════════════════════════════════════════════════════════╗",
                      curses.A_BOLD)
        stdscr.addstr(1, 0, "║                      ZAPPY SWARM REAL-TIME DASHBOARD                         ║",
                      curses.A_BOLD)
        stdscr.addstr(2, 0, "╚══════════════════════════════════════════════════════════════════════════════╝",
                      curses.A_BOLD)
        stdscr.addstr(3, 0, "  [Appuyez sur 'q' pour quitter]  |  Dossier cible : .zappy_stats/\n")

        if not os.path.exists(stats_dir):
            stdscr.addstr(5, 2, "En attente du déploiement des premiers drones...")
        else:
            files = [f for f in os.listdir(stats_dir) if f.endswith(".json")]

            if not files:
                stdscr.addstr(5, 2, "Aucun drone actif détecté.")

            row = 5
            for filename in sorted(files):
                path = os.path.join(stats_dir, filename)
                try:
                    with open(path, "r") as f:
                        data = json.load(f)

                    d_id = data["id"]
                    role = data["role"]
                    state = data["state"]
                    level = data["level"]
                    pos = data["position"]
                    inv = data["inventory"]

                    color = curses.color_pair(4)
                    if "SURVIVAL" in state:
                        color = curses.color_pair(3)
                    elif "Master" in role:
                        color = curses.color_pair(1)
                    elif "Slave" in role:
                        color = curses.color_pair(2)

                    stdscr.addstr(row, 2, f"🤖 Drone [{d_id}]", curses.A_BOLD)
                    stdscr.addstr(row, 18, f"Niveau: {level}  |  Pos estimée: ({pos[0]},{pos[1]})  |  ")
                    stdscr.addstr(row, 48, f"Rôle: {role:<8}  État: {state:<15}", color)

                    inv_str = (f"Nourriture: {inv.get('food', 0):<2} │ "
                               f"L: {inv.get('linemate', 0)} │ D: {inv.get('deraumere', 0)} │ "
                               f"S: {inv.get('sibur', 0)} │ M: {inv.get('mendiane', 0)} │ "
                               f"P: {inv.get('phiras', 0)} │ T: {inv.get('thystame', 0)}")
                    stdscr.addstr(row + 1, 4, f"🎒 Pocket ➔ {inv_str}")
                    stdscr.addstr(row + 2, 2, "╶" * 76, curses.A_DIM)

                    row += 3
                except (json.JSONDecodeError, KeyError, IOError):
                    continue

        stdscr.refresh()

        try:
            key = stdscr.getch()
            if key == ord('q'):
                if os.path.exists(stats_dir):
                    for f in os.listdir(stats_dir):
                        os.remove(os.path.join(stats_dir, f))
                break
        except KeyboardInterrupt:
            break


if __name__ == "__main__":
    curses.wrapper(draw_dashboard)