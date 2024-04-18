import tkinter as tk
from Message import Message


class ChatFrame(tk.Frame):
    def __init__(self, parent, client_socket, auth_username):
        self.client_socket = client_socket
        self.auth_username = auth_username
        super().__init__(
            parent,
            bg="#31363F",
            width=1,
            height=490
        )
        self.grid(row=2, column=0, sticky="nsew")
        self.messages = []
        self.display_messages()

    def send_message(self, text_field):
        msg = text_field.get()
        print("Sending message \"" + str(msg.encode()) + "\" to server...")
        self.client_socket.sendall(msg.encode())
        print("Message sent.")

    def add_message_to_chat(self, message):
        self.messages.append(Message(message, self.auth_username))
        self.display_messages()

    def display_messages(self):
        for child in self.winfo_children():
            child.destroy()
        for i in range(10):
            try:
                msg = self.messages[i]
                msg_label = tk.Label(
                    self,
                    text=msg.message,
                    anchor="w" if self.auth_username == msg.username else "e"
                )
                msg_label.pack(
                    fill="x",
                    expand=True
                )
            except:
                default_msg = tk.Label(
                    self,
                    text="",
                    anchor="w" if i % 2 == 0 else "e"
                )
                default_msg.pack(
                    fill="x",
                    expand=True
                )


