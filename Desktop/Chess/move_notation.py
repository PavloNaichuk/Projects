PIECE_LETTER = {
    "P": "",
    "N": "N",
    "B": "B",
    "R": "R",
    "Q": "Q",
    "K": "K",
}


def square_name(square):
    row, col = square
    return f"{chr(col + 97)}{8 - row}"


def format_move(move):
    start, end, piece, captured = move[:4]
    promotion = move[4] if len(move) > 4 else None

    piece_letter = PIECE_LETTER.get(piece[1].upper(), "")
    move_text = f"{piece_letter}{square_name(start)} - {square_name(end)}"

    if promotion:
        move_text += f"={promotion}"

    if captured is not None and captured != "":
        move_text += " +"

    return move_text
