import tkinter as tk
from UI_Components.TitleRow import TitleRow
from UI_Components.Title import Title
from UI_Components.StatusBar import StatusBar
from UI_Components.ChatFrame import ChatFrame
from UI_Components.BottomBar import BottomBar
from Message import Message
import socket
from tkinter import messagebox


def auth_window():
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
    login_button = tk.Button(root, text="Login", command=lambda: login(root, username_entry.get(), password_entry.get()))
    login_button.grid(row=2, column=0, columnspan=2, pady=5)

    # Signup button
    signup_button = tk.Button(root, text="Signup", command=lambda main_window=root: signup(main_window))
    signup_button.grid(row=3, column=0, columnspan=2, pady=5)

    # Run the main event loop
    root.mainloop()

def main_window():
    main_window = tk.Tk()
    main_window.title("Super Chat")
    main_window.geometry("600x600")

    title = Title(main_window)
    status_bar = StatusBar(main_window)
    chat_frame = ChatFrame(main_window)
    bottom_bar = BottomBar(main_window)

    main_window.mainloop()


def login(active_window, username, password):
    client_socket.sendall(username.encode())
    client_socket.sendall(password.encode())
    auth_response = client_socket.recv(1024)
    print(auth_response)
    if auth_response.decode('utf-8') == "Authentication successful":
        active_window.destroy()
        main_window()
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
client_socket = connect_to_server(SERVER_HOST, SERVER_PORT)

auth_window()

