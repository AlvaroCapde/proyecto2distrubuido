import tkinter as tk

from Requester import Requester
from UI_Components.Title import Title
from UI_Components.StatusBar import StatusBar
from UI_Components.ChatFrame import ChatFrame
from UI_Components.BottomBar import BottomBar
from tkinter import messagebox


requester = Requester()


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
    login_button = tk.Button(root,
                             text="Login",
                             command=lambda: login(
                                 root,
                                 username_entry.get(),
                                 password_entry.get()
                                )
                             )

    login_button.grid(row=2, column=0, columnspan=2, pady=5)

    # Run the main event loop
    root.mainloop()


def main_window(auth_username):
    main_window = tk.Tk()
    main_window.title("Super Chat")
    main_window.geometry("350x600")

    title = Title(main_window)
    status_bar = StatusBar(main_window)
    chat_frame = ChatFrame(main_window, requester, auth_username)
    bottom_bar = BottomBar(main_window, chat_frame)

    main_window.mainloop()


def login(active_window, username, password):
    print(username)
    print(password)
    auth_response = requester.login(username, password)

    if auth_response:
        active_window.destroy()
        main_window(username)




auth_window()
