import tkinter as tk


class StatusBar(tk.Label):
    online_color = "#5cb85c"
    offline_color = "#cc3300"

    def __init__(self, parent):
        super().__init__(
            parent,
            text="Connected",
            bg=self.online_color,
            fg="white",
            height=1)
        self.status = True

        self.grid(row=1, column=0, sticky="ew")
