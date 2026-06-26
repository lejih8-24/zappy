import os
import time
import json
import curses

from constants import State, Role


class Display:
    def __init__(self, folder: str = '.zappy_stats'):
        self.folder = folder
        if not any(f.endswith('.json') for f in os.listdir('.') if f.startswith('.zappy_stats')):
            os.makedirs(".zappy_stats", exist_ok=True)
            os.system("rm -rf .zappy_stats/*")

    def save_dashboard_state(self, id, role, state, level, position, inventory):
        """Sauvegarde l'état actuel dans un fichier JSON partagé."""
        state_data = {
            "id": id,
            "role": role.value,
            "state": state.value,
            "level": level,
            "position": position,
            "inventory": inventory
        }

        os.makedirs(".zappy_stats", exist_ok=True)
        filename = f".zappy_stats/drone_{id}.json"

        temp_filename = f"{filename}.tmp"
        with open(temp_filename, "w") as f:
            json.dump(state_data, f)
        os.replace(temp_filename, filename)


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
        # 1. On récupère la taille maximale du terminal à cet instant (y = hauteur, x = largeur)
        max_y, max_x = stdscr.getmaxyx()

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
                # 2. SÉCURITÉ : Si la prochaine écriture dépasse le bas du terminal, on arrête d'afficher
                if row + 3 >= max_y:
                    stdscr.addstr(max_y - 1, 2, "... [Agrandissez le terminal pour voir les autres drones] ...",
                                  curses.A_DIM)
                    break

                path = os.path.join(stats_dir, filename)
                try:
                    with open(path, "r") as f:
                        data = json.load(f)

                    d_id = data["id"]
                    # Extraction robuste en cas de changement de format enum
                    role = data.get("role", "Unknown")
                    state = data.get("state", "Unknown")
                    level = data.get("level", 1)
                    pos = data.get("position", [0, 0])
                    inv = data.get("inventory", {})

                    color = curses.color_pair(4)
                    if state == State.SURVIVAL.value or state == "SURVIVAL":
                        color = curses.color_pair(3)
                    elif role == Role.Master.value or role == "Master":
                        color = curses.color_pair(1)
                    elif role == Role.Slave.value or role == "Slave":
                        color = curses.color_pair(2)

                    stdscr.addstr(row, 2, f"🤖 Drone [{d_id}]", curses.A_BOLD)
                    stdscr.addstr(row, 18, f"Niveau: {level}  |  Pos estimée: ({pos[0]},{pos[1]})  |  ")
                    stdscr.addstr(row, 48, f"Rôle: {str(role):<8}  État: {str(state):<15}", color)

                    inv_str = (f"Nourriture: {inv.get('food', 0):<2} │ "
                               f"L: {inv.get('linemate', 0)} │ D: {inv.get('deraumere', 0)} │ "
                               f"S: {inv.get('sibur', 0)} │ M: {inv.get('mendiane', 0)} │ "
                               f"P: {inv.get('phiras', 0)} │ T: {inv.get('thystame', 0)}")

                    safe_inv_str = inv_str[:max_x - 15]
                    stdscr.addstr(row + 1, 4, f"🎒 Pocket ➔ {safe_inv_str}")

                    line_len = min(76, max_x - 3)
                    if line_len > 0:
                        stdscr.addstr(row + 2, 2, "╶" * line_len, curses.A_DIM)

                    row += 3
                except (json.JSONDecodeError, KeyError, IOError):
                    continue

        stdscr.refresh()

        try:
            key = stdscr.getch()
            if key == ord('q'):
                if os.path.exists(stats_dir):
                    for f in os.listdir(stats_dir):
                        try:
                            os.remove(os.path.join(stats_dir, f))
                        except OSError:
                            pass
                break
        except KeyboardInterrupt:
            break


if __name__ == "__main__":
    curses.wrapper(draw_dashboard)