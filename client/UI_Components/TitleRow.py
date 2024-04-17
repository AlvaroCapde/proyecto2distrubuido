import tkinter as tk


class TitleRow(tk.Frame):

    def __init__(self, parent):
        super().__init__(
            parent
        )

        # Top Bar
        self.title_label = tk.Label(
            self,
            text="Super Secret Chat",
            bg="#222831", fg="white",
            height=2
        )
        self.title_label.pack(
            fill="both",
            expand=True
        )

        self.pack(
            fill="x",
            expand=True,
            side="top",
            anchor="n"
        )

    def send_message(self):
        pass
