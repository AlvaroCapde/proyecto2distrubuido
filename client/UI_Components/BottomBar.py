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
        self.button = tk.Button(
            self,
            text="Enviar",
            command=lambda: self.chat_frame.send_message(self.text_field)
        )
        self.button.pack(side="right")