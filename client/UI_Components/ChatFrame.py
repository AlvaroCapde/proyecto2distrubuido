import tkinter as tk
from Message import Message


class ChatFrame(tk.Frame):
    def __init__(self, parent):
        super().__init__(
            parent,
            bg="#31363F",
            width=1,
            height=490
        )
        self.grid(row=2, column=0, sticky="nsew")
        self.display_messages([], "samuellangarica")

    def display_messages(self, messages, auth_username):
        for i in range(10):
            try:
                msg = messages[i]
            except:
                default_msg = tk.Label(
                    self,
                    text="Mensaje #"+str(i),
                    anchor="w" if i % 2 == 0 else "e"
                )
                default_msg.pack(
                    fill="x",
                    expand=True
                )


