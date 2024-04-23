import socket


class Requester:
    def __init__(self):
        self.server_host = '127.0.0.1'
        self.server_port = 8080
        self.client_socket = None
        self.connect_to_server()

    def connect_to_server(self):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.client_socket.connect((self.server_host, self.server_port))
            print("Connected to the server.")

            message = "1Hello from the client!"
            self.client_socket.sendall(message.encode())

            data = self.client_socket.recv(1024)
            print("Received from server: " + data.decode())

        except Exception as e:
            print("Error:", e)

    def login(self, username, password):
        try:
            self.client_socket.sendall(f"2\n{username}\n{password}".encode())
            data = self.client_socket.recv(1024)
            print("Received from server: " + data.decode())

            if data.decode() == "Auth Success":
                return True
            else:
                return False

        except Exception as e:
            print("Error:", e)

    def send_message(self, message):
        try:
            self.client_socket.sendall(f"3\n{message}".encode())
            data = self.client_socket.recv(1024)
            print("Received from server: " + data.decode())

        except Exception as e:
            print("Error:", e)

    def get_messages(self):
        try:
            self.client_socket.sendall(f"4".encode())
            data = self.client_socket.recv(1024)
            print("Messages received: " + data.decode())
            return data.decode()

        except Exception as e:
            print("Error:", e)