import math

def generate_path_to_index(index: int) -> list[str]:
    """Convertit un index du tableau Look en séquence d'actions Zappy."""
    if index == 0:
        return []

    y = math.isqrt(index)
    x = index - (y * y + y)

    path = []

    for _ in range(y):
        path.append("Forward")

    if x < 0:
        path.append("Left")
        for _ in range(abs(x)):
            path.append("Forward")
    elif x > 0:
        path.append("Right")
        for _ in range(x):
            path.append("Forward")

    return path

def find_path_to_closest(vision_grid: list[dict], target: str) -> list[str] | None:
    """
    Trouve la ressource demandée la plus proche (en nombre d'actions Zappy)
    et retourne le chemin pour y aller. Retourne None si introuvable.
    """
    if not vision_grid:
        return None

    best_index = -1
    best_cost = 999

    for i, tile in enumerate(vision_grid):
        if tile.get(target, 0) > 0:
            y = math.isqrt(i)
            x = i - (y * y + y)

            cost = y + abs(x) + (1 if x != 0 else 0)

            if cost < best_cost:
                best_cost = cost
                best_index = i

    if best_index == -1:
        return None

    return generate_path_to_index(best_index)