import tkinter as tk
from UI_Components.TitleRow import TitleRow
from UI_Components.Title import Title
from UI_Components.StatusBar import StatusBar
from UI_Components.ChatFrame import ChatFrame
from UI_Components.BottomBar import BottomBar
from Message import Message
import socket
from tkinter import messagebox
import threading


def auth_window(client_socket):
    root = tk.Tk()
    root.title("Authentication")
    username_label = tk.Label(root, text="Username:")
    username_label.grid(row=0, column=0, padx=10, pady=5)
    username_entry = tk.Entry(root)
    username_entry.grid(row=0, column=1, padx=10, pady=5)

    # Password label and entry
    password_label = tk.Label(root, text="Password:")
    password_label.grid(row=1, column=0, padx=10, pady=5)
    password_entry = tk.Entry(root, show="*")
    password_entry.grid(row=1, column=1, padx=10, pady=5)

    # Login button
    login_button = tk.Button(root,
                             text="Login",
                             command=lambda: login(
                                 root,
                                 username_entry.get(),
                                 password_entry.get(),
                                 client_socket
                                )
                             )

    login_button.grid(row=2, column=0, columnspan=2, pady=5)

    # Signup button
    signup_button = tk.Button(root, text="Signup", command=lambda main_window=root: signup(main_window))
    signup_button.grid(row=3, column=0, columnspan=2, pady=5)

    # Run the main event loop
    root.mainloop()


def receive_messages(client_socket, chat_frame):
    while True:
        try:
            print("Listening to incoming messages for " + chat_frame.auth_username)
            message = client_socket.recv(1024).decode()
            if not message:
                print("Server disconnected")
                break
            print("Message received: " + str(message))
            chat_frame.add_message_to_chat(message)
        except Exception as e:
            print("Error receiving message:", e)
            break


def main_window(client_socket, auth_username):
    main_window = tk.Tk()
    main_window.title("Super Chat")
    main_window.geometry("600x600")

    title = Title(main_window)
    status_bar = StatusBar(main_window)
    chat_frame = ChatFrame(main_window, client_socket, auth_username)
    bottom_bar = BottomBar(main_window, chat_frame)

    # Receive messages from a thread
    receive_thread = threading.Thread(target=receive_messages, args=(client_socket, chat_frame))
    receive_thread.start()

    main_window.mainloop()


def login(active_window, username, password, client_socket):
    print(username)
    print(password)
    # Send username and password together with a delimiter
    client_socket.sendall(f"{username}\n{password}".encode())

    # Receive and process the authentication response
    auth_response = b''
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        auth_response += data
        if b'\n' in data:
            break

    print(auth_response)
    auth_response = auth_response.strip()  # Remove leading/trailing whitespace
    if auth_response.decode('utf-8') == "Authentication successful":
        active_window.destroy()
        main_window(client_socket, username)
    else:
        messagebox.showinfo("Authentication Error", "Invalid username or/and password")


def signup(active_window):
    active_window.destroy()
    main_window()


def connect_to_server(SERVER_HOST, SERVER_PORT):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((SERVER_HOST, SERVER_PORT))
        print("Connected to the server.")

        message = "Hello from the client!"
        client_socket.sendall(message.encode())

        data = client_socket.recv(1024)
        print("Received from server:", data.decode())

        return client_socket

    except Exception as e:
        print("Error:", e)


SERVER_HOST = '127.0.0.1'
SERVER_PORT = 8080

auth_window(connect_to_server(SERVER_HOST, SERVER_PORT))
