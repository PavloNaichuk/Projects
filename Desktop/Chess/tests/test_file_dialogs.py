from unittest.mock import MagicMock, patch

from file_dialogs import ask_load_filename, ask_save_filename


@patch("file_dialogs.filedialog.asksaveasfilename")
@patch("file_dialogs.tk.Tk")
def test_ask_save_filename_returns_selected_path(mock_tk, mock_asksaveasfilename):
    root = MagicMock()
    mock_tk.return_value = root
    mock_asksaveasfilename.return_value = "save.json"

    assert ask_save_filename() == "save.json"

    root.withdraw.assert_called_once()
    root.destroy.assert_called_once()
    mock_asksaveasfilename.assert_called_once_with(
        defaultextension=".json",
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
    )


@patch("file_dialogs.filedialog.askopenfilename")
@patch("file_dialogs.tk.Tk")
def test_ask_load_filename_returns_selected_path(mock_tk, mock_askopenfilename):
    root = MagicMock()
    mock_tk.return_value = root
    mock_askopenfilename.return_value = "save.json"

    assert ask_load_filename() == "save.json"

    root.withdraw.assert_called_once()
    root.destroy.assert_called_once()
    mock_askopenfilename.assert_called_once_with(
        defaultextension=".json",
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
    )
