##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## mock_server.py
##

import socket
import time

HOST = "localhost"
PORT = 4242

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((HOST, PORT))
server.listen(1)
print(f"Mock server listening on {HOST}:{PORT}")

conn, addr = server.accept()
print(f"GUI connected: {addr}")

conn.send(b"WELCOME\n")
data = conn.recv(1024)
print(f"GUI sent: {data.decode().strip()}")

conn.send(b"msz 10 10\n")
conn.send(b"tna TeamA\n")
conn.send(b"tna TeamB\n")
conn.send(b"pnw #1 0 0 1 1 TeamA\n")
# conn.send(b"enw #1 #1 5 5\n")

time.sleep(1)
for x in range(1, 10):
    conn.send(f"ppo #1 {x} 0 1\n".encode())
    time.sleep(1)

conn.close()
server.close()
