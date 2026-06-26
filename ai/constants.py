from enum import Enum

class Role(Enum):
    Explorer = 0
    Master = 1
    Incanting = 2
    Slave = 3

    def __str__(self):
        return self.name

class State(Enum):
    FARMING = 0
    INCANTATION = 1
    GROUPING = 2
    SURVIVAL = 3
    WAITING_ELEVATION = 4
    CONTRIBUTING = 5
    FORKING = 6

    def __str__(self):
        return self.name


ELEVATION_RULES = {
    1: {"players": 1, "stones": {"linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0}},
    2: {"players": 2, "stones": {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0}},
    3: {"players": 2, "stones": {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0}},
    4: {"players": 4, "stones": {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0}},
    5: {"players": 4, "stones": {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0}},
    6: {"players": 6, "stones": {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0}},
    7: {"players": 6, "stones": {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0}},
}