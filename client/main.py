import tkinter as tk
chat_frame = None

def login():
    username = username_entry.get()
    password = password_entry.get()
    # TODO
    root.destroy()
    open_main_window()


def signup():
    username = username_entry.get()
    password = password_entry.get()
    # TODO
    root.destroy()
    open_main_window()


def get_active_chats(parent, main_window):
    active_chats_placeholder = ["samuellangarica", "alvarocapde"]
    active_chats = []
    for ac in active_chats_placeholder:
        button = tk.Button(parent, text=ac, command=lambda username=ac, main_win=main_window: show_chat(username, main_win))
        active_chats.append(button)
    return active_chats


def show_chat(username, main_window):
    print(username)
    new_chat_frame = tk.LabelFrame(main_window, borderwidth=0, bg="#222831")
    chat_label = tk.Label(new_chat_frame, text=username, bg="#222831")
    chat_label.pack()
    new_chat_frame.place(relx=0.3, rely=0, relwidth=0.7, relheight=1)

    global chat_frame
    chat_frame.destroy()  # Destroy the existing chat frame
    chat_frame = new_chat_frame  # Assign the new chat frame
    chat_frame.place(relx=0.3, rely=0, relwidth=0.7, relheight=1)


def open_main_window():
    main_window = tk.Tk()
    main_window.title("Super Chat")
    main_window.geometry("600x600")

    # SIDE PANEL -> Active Chats
    frame_width = int(main_window.winfo_width() * 0.3)
    active_chats_frame = tk.Frame(main_window, width=frame_width, height=100)
    active_chats_frame.place(relx=0, rely=0, relwidth=0.3, relheight=1)

    top_label = tk.Label(active_chats_frame, text="Active Chats", bg="gray", fg="white")
    top_label.pack(fill="x", padx=0, pady=0, anchor="n")

    active_chats = get_active_chats(active_chats_frame, main_window)
    for active_chat in active_chats:
        active_chat.pack(fill="x", padx=10, pady=5)

    #  CHAT FRAME
    global chat_frame
    chat_frame = tk.LabelFrame(main_window, borderwidth=0, bg="#222831")
    chat_label = tk.Label(chat_frame, text="No chat selected", bg="#222831")
    chat_label.pack()
    chat_frame.place(relx=0.3, rely=0, relwidth=0.7, relheight=1)

    main_window.mainloop()


# Login/Signup
root = tk.Tk()
root.title("Login/Signup")
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
login_button = tk.Button(root, text="Login", command=login)
login_button.grid(row=2, column=0, columnspan=2, pady=5)

# Signup button
signup_button = tk.Button(root, text="Signup", command=signup)
signup_button.grid(row=3, column=0, columnspan=2, pady=5)

# Run the main event loop
root.mainloop()
