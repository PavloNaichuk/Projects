import tkinter as tk
from tkinter import filedialog


def ask_save_filename():
    root = tk.Tk()
    root.withdraw()

    filename = filedialog.asksaveasfilename(
        defaultextension=".json",
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
    )

    root.destroy()
    return filename


def ask_load_filename():
    root = tk.Tk()
    root.withdraw()

    filename = filedialog.askopenfilename(
        defaultextension=".json",
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
    )

    root.destroy()
    return filename
