import tkinter as tk

class Title(tk.Label):

    def __init__(self, parent):
        super().__init__(
            parent,
            text="Super Secret Chat",
            bg="#222831", fg="white",
            height=2
        )
        parent.grid_columnconfigure(0, weight=1)
        self.grid(row=0, column=0, sticky="ew")
