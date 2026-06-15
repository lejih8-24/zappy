import uuid
from abc import ABC, abstractmethod
from typing import Union


class Command(ABC):
    """Classe de base pour toutes les commandes Zappy."""

    @property
    @abstractmethod
    def command_string(self) -> str:
        """La chaîne exacte à envoyer au serveur."""
        pass

    @abstractmethod
    def time_limit(self) -> int:
        """Le temps d'exécution de la commande en ticks (f)."""
        pass

    def parse_response(self, response: str) -> Union[bool, list, dict, int, str]:
        """Parse la réponse standard. Peut être surchargée par les classes enfants."""
        return response == "ok"


class ForwardCommand(Command):
    @property
    def command_string(self) -> str:
        return "Forward"

    def time_limit(self) -> int:
        return 7


class TurnRightCommand(Command):
    @property
    def command_string(self) -> str:
        return "Right"

    def time_limit(self) -> int:
        return 7


class TurnLeftCommand(Command):
    @property
    def command_string(self) -> str:
        return "Left"

    def time_limit(self) -> int:
        return 7


class LookCommand(Command):
    @property
    def command_string(self) -> str:
        return "Look"

    def time_limit(self) -> int:
        return 7

    def parse_response(self, response: str) -> list[dict]:
        if response == "ko":
            return []

        clean_str = response.strip("[] \n")
        raw_tiles = clean_str.split(',')
        parsed_tiles = []

        for tile_content in raw_tiles:
            tile_inventory = {}
            items = tile_content.strip().split()

            for item in items:
                tile_inventory[item] = tile_inventory.get(item, 0) + 1

            parsed_tiles.append(tile_inventory)

        return parsed_tiles


class InventoryCommand(Command):
    @property
    def command_string(self) -> str:
        return "Inventory"

    def time_limit(self) -> int:
        return 1

    def parse_response(self, response: str) -> dict:
        if response == "ko":
            return {}

        inventory = {}
        clean_str = response.strip("[] \n")
        items = clean_str.split(",")

        for item in items:
            parts = item.strip().split()
            if len(parts) == 2:
                name, qty = parts[0], int(parts[1])
                inventory[name] = qty
        return inventory


class BroadcastCommand(Command):
    def __init__(self, token):
        self.token = token
        self.my_id = str(uuid.uuid4())[:4]
        self.text = ""

    def build_message(self, target_id, level, hierarchy, state, request) -> str:
        """Génère la chaîne de caractères à envoyer au serveur."""
        text = f"{self.token}|{self.my_id}|{target_id}|{level}|{hierarchy}|{state}|{request}"
        return text

    @property
    def command_string(self) -> str:
        return f"Broadcast {self.text}"

    def time_limit(self) -> int:
        return 7

    def parse_response(self, response: str) -> Union[bool, list, dict, int, str]:
        elements = response.split('|')
        return elements


class ConnectNbrCommand(Command):
    @property
    def command_string(self) -> str:
        return "Connect_nbr"

    def time_limit(self) -> int:
        return 0

    def parse_response(self, response: str) -> int:
        try:
            return int(response)
        except ValueError:
            return -1


class ForkCommand(Command):
    @property
    def command_string(self) -> str:
        return "Fork"

    def time_limit(self) -> int:
        return 42


class EjectCommand(Command):
    @property
    def command_string(self) -> str:
        return "Eject"

    def time_limit(self) -> int:
        return 7


class TakeCommand(Command):
    def __init__(self, obj_name: str):
        self.obj_name = obj_name

    @property
    def command_string(self) -> str:
        return f"Take {self.obj_name}"

    def time_limit(self) -> int:
        return 7


class SetCommand(Command):
    def __init__(self, obj_name: str):
        self.obj_name = obj_name

    @property
    def command_string(self) -> str:
        return f"Set {self.obj_name}"

    def time_limit(self) -> int:
        return 7


class IncantationCommand(Command):
    @property
    def command_string(self) -> str:
        return "Incantation"

    def time_limit(self) -> int:
        return 300

    def parse_response(self, response: str) -> str:
        return response
