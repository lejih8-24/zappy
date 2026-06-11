import sys
import argparse
import time
from network import NetworkClient
from ZappyAIModel import ZappyAI

def parse_arguments():
    """
    Parse les arguments de la ligne de commande selon le sujet Zappy.
    """
    parser = argparse.ArgumentParser(add_help=False, description="Client IA Zappy")

    parser.add_argument("-p", dest="port", type=int, required=True, help="Port du serveur")
    parser.add_argument("-n", dest="name", type=str, required=True, help="Nom de l'équipe")
    parser.add_argument("-h", dest="host", type=str, default="localhost", help="Hôte (localhost par défaut)")
    parser.add_argument("--help", action="help", help="Affiche ce message d'aide")

    try:
        return parser.parse_args()
    except SystemExit:
        sys.exit(84)


def handle_handshake(client: NetworkClient, team_name: str) -> tuple[int, int, int]:
    """
    Gère la séquence de connexion initiale (Handshake).
    Retourne un tuple: (client_num, map_x, map_y)
    """
    step = 0
    client_num = 0
    map_x = 0
    map_y = 0

    print("Attente du handshake...")

    while step < 3:
        responses = client.get_responses()
        for response in responses:
            print(f"Serveur: {response}")

            if step == 0 and response == "WELCOME":
                print(f"Envoi du nom d'équipe : {team_name}")
                client.send_command(team_name)
                step = 1

            elif step == 1:
                try:
                    client_num = int(response)
                    print(f"Slots restants : {client_num}")

                    if client_num < 1:
                        print("Erreur : Plus de place pour cette équipe.")
                        sys.exit(84)

                    step = 2
                except ValueError:
                    print(f"Erreur de parsing du client_num: {response}")
                    sys.exit(84)

            elif step == 2:
                try:
                    parts = response.split()
                    if len(parts) >= 2:
                        map_x = int(parts[0])
                        map_y = int(parts[1])
                        print(f"Dimensions de la map : {map_x}x{map_y}")
                        step = 3
                except ValueError:
                    print(f"Erreur de parsing de la map: {response}")
                    sys.exit(84)

    return client_num, map_x, map_y

def main():
    args = parse_arguments()
    print(f"Démarrage de l'IA '{args.name}' sur {args.host}:{args.port}")

    client = NetworkClient(args.host, args.port)
    client.connect()

    try:
        client_num, map_x, map_y = handle_handshake(client, args.name)
        print("Handshake terminé ! Transfert du contrôle au cerveau de l'IA.")

        ai = ZappyAI(client, args.name, map_x, map_y)

        ai.run()

    except KeyboardInterrupt:
        print("\nInterruption par l'utilisateur.")
    finally:
        client.close()
        print("Connexion fermée.")

if __name__ == "__main__":
    main()


if __name__ == "__main__":
    main()