import tkinter as tk


class BottomBar(tk.Frame):
    def __init__(self, parent, chat_frame):
        self.chat_frame = chat_frame
        super().__init__(
            parent,
            bg="#222831",
            width=1,
            height=50
        )
        self.grid(row=3, column=0, sticky="nsew")

        self.text_field = tk.Entry(self)
        self.text_field.pack(side="left", fill="both", expand=True)

        # Button
        self.send_button = tk.Button(
            self,
            text="Whisper",
            command=lambda: self.chat_frame.send_message(self.text_field)
        )
        self.send_button.pack(side="right")

        # Button
        self.refresh_button = tk.Button(
            self,
            text="Listen",
            command=self.chat_frame.get_messages
        )
        self.refresh_button.pack(side="right")