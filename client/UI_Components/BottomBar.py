import tkinter as tk


class BottomBar(tk.Frame):
    def __init__(self, parent):
        super().__init__(
            parent,
            bg="#222831",
            width=1,
            height=50
        )
        self.grid(row=3, column=0, sticky="nsew")

        text_field = tk.Entry(self)
        text_field.pack()