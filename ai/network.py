import socket
import select
import sys

class NetworkClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.socket = None
        self._read_buffer = ""

    def connect(self):
        """
        Permet de se connecter au réseau du zappy
        """
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.socket.setblocking(False)
            print(f"Connecté au serveur {self.host}:{self.port}")
        except ConnectionRefusedError:
            print(f"Impossible de se connecter à {self.host}:{self.port}", file=sys.stderr)
            sys.exit(84)

    def send_command(self, command: str):
        if not self.socket:
            return

        if not command.endswith("\n"):
            command += "\n"

        try:
            self.socket.sendall(command.encode('utf-8'))
        except BrokenPipeError:
            print("❌ Erreur : Connexion au serveur perdue lors de l'envoi.", file=sys.stderr)
            sys.exit(84)

    def get_responses(self) -> list[str]:
        """
        Écoute la socket sans bloquer le programme.
        Retourne une liste de messages complets envoyés par le serveur.
        """
        if not self.socket:
            return []

        ready_to_read, _, _ = select.select([self.socket], [], [], 0.05)

        if ready_to_read:
            try:
                chunk = self.socket.recv(4096).decode('utf-8')

                if not chunk:
                    print("Le serveur a fermé la connexion (Mort ou fin de partie).")
                    sys.exit(0)

                self._read_buffer += chunk
            except BlockingIOError:
                pass
            except ConnectionResetError:
                print("Erreur : Connexion réinitialisée par le serveur.", file=sys.stderr)
                sys.exit(84)

        messages = self._read_buffer.split('\n')

        self._read_buffer = messages.pop()

        return messages

    def close(self):
        """Ferme proprement la socket."""
        if self.socket:
            self.socket.close()
            self.socket = None
