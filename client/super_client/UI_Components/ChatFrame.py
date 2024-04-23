import tkinter as tk
import threading
import time

class ChatFrame(tk.Frame):
    def __init__(self, parent, requester, auth_username):
        self.auth_username = auth_username
        self.requester = requester
        super().__init__(
            parent,
            bg="#31363F",
            width=1,
            height=490
        )
        self.grid(row=2, column=0, sticky="nsew")


    def send_message(self, text_field):
        self.requester.send_message(text_field.get())
        text_field.delete(0, tk.END)

    def get_messages(self):
            data = self.requester.get_messages()
            self.display_messages(data)

    def display_messages(self, data):
        for child in self.winfo_children():
            child.destroy()
        for line in data.split("\n"):
            msg_label = tk.Label(
                self,
                text=line,
                anchor="e" if self.auth_username == line.split(":")[0] else "w"
            )
            msg_label.pack(
                fill="x",
                expand=True
            )


