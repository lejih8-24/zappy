from enum import Enum

class NodeStatus(Enum):
    SUCCESS = 1
    FAILURE = 2
    RUNNING = 3

class Node:
    """Classe de base pour tous les nœuds de l'arbre."""
    def tick(self, ai) -> NodeStatus:
        raise NotImplementedError

class Selector(Node):
    """Exécute les enfants jusqu'à ce que l'un d'eux réussisse ou soit en mode RUNNING."""
    def __init__(self, children: list[Node]):
        self.children = children

    def tick(self, ai) -> NodeStatus:
        for child in self.children:
            status = child.tick(ai)
            if status != NodeStatus.FAILURE:
                return status
        return NodeStatus.FAILURE

class Sequence(Node):
    """Exécute les enfants jusqu'à ce que l'un d'eux échoue."""
    def __init__(self, children: list[Node]):
        self.children = children

    def tick(self, ai) -> NodeStatus:
        for child in self.children:
            status = child.tick(ai)
            if status != NodeStatus.SUCCESS:
                return status
        return NodeStatus.SUCCESS