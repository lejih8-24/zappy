import subprocess
import time
import argparse
import sys


def main():
    parser = argparse.ArgumentParser(description="Lanceur automatique pour Zappy AI", add_help=False)
    parser.add_argument("-p", "--port", type=str, required=True, help="Port du serveur")
    parser.add_argument("-n", "--name", type=str, required=True, help="Nom de l'équipe")
    parser.add_argument("-h", "--host", type=str, default="127.0.0.1", help="Adresse du serveur")
    parser.add_argument("-c", "--clients", type=int, default=6, help="Nombre d'IA à lancer (défaut: 6)")

    args = parser.parse_args()

    processes = []

    print(f"Lancement de l'essaim : {args.clients} IA pour l'équipe '{args.name}'...")
    print("Appuyez sur CTRL+C à tout moment pour arrêter tous les drones.\n")

    for i in range(args.clients):
        print(f"Déploiement du drone #{i + 1}...")

        p = subprocess.Popen([
            sys.executable, "ai/main.py",
            "-p", args.port,
            "-n", args.name,
            "-h", args.host
        ])
        processes.append(p)

        time.sleep(0.5)

    print("\nTous les drones sont en ligne !")

    try:
        while True:
            for i, p in enumerate(processes):
                if p.poll() is not None:
                    print(f"Alerte : Le drone #{i + 1} est mort ou s'est déconnecté.")
            time.sleep(2)

    except KeyboardInterrupt:
        print("\nInterruption demandée. Arrêt d'urgence de tous les drones...")
        for p in processes:
            if p.poll() is None:
                p.terminate()
        print("Toutes les IA ont été éteintes. Fin de la simulation.")


if __name__ == "__main__":
    main()