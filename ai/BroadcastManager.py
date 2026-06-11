import uuid
from ZappyAIModel import State, Role

class BroadcastManager:
    """
    Gestionnaire centralisé du protocole de communication de l'équipe.
    Format strict : Token|Sender_ID|Target_ID|Level|Hierarchy|State|Request
    """

    def __init__(self, token: str = "ZappyStras2026"):
        self.token = token

        self.my_id = str(uuid.uuid4())[:4]

    def format_message(self, target_id: str, level: int, hierarchy: Role, state: State, request: str) -> str:
        """
        Assemble les données en une chaîne formatée prête à être envoyée dans un BroadcastCommand.

        :param target_id: L'ID du drone visé (ou 'ALL' pour tout le monde)
        :param level: Le niveau actuel de l'IA (1-8)
        :param hierarchy: 'EXPLORER', 'MASTER', ou 'SLAVE'
        :param state: 'FARMING', 'GROUPING', ou 'INCANTING'
        :param request: L'action (ex: 'INCANTATION_CALL', 'OMW', 'ABORT')
        """
        safe_request = request.replace('|', '')
        return f"{self.token}|{self.my_id}|{target_id}|{level}|{hierarchy}|{state}|{safe_request}"

    def parse_message(self, raw_text: str) -> dict | None:
        """
        Déchiffre un message reçu depuis le serveur.
        Retourne un dictionnaire exploitable si le message est légitime et destiné
        à notre équipe, sinon retourne None.
        """
        clean_text = raw_text.strip()
        parts = clean_text.split('|')

        if len(parts) != 7:
            return None

        if parts[0] != self.token:
            return None

        # 3. Vérification de la cible (est-ce que ça m'est adressé ?)
        # On accepte si c'est 'ALL' ou si c'est exactement mon ID personnel
        target_id = parts[2]
        if target_id != "ALL" and target_id != self.my_id:
            return None

        try:
            return {
                "sender_id": parts[1],
                "target_id": target_id,
                "level": int(parts[3]),
                "hierarchy": parts[4],
                "state": parts[5],
                "request": parts[6]
            }
        except ValueError:
            return None