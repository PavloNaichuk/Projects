from move_notation import format_move, square_name


def test_square_name():
    assert square_name((7, 0)) == "a1"
    assert square_name((0, 7)) == "h8"
    assert square_name((6, 4)) == "e2"


def test_format_pawn_move():
    move = ((6, 4), (4, 4), "wP", None, None)

    assert format_move(move) == "e2 - e4"


def test_format_knight_move():
    move = ((7, 6), (5, 5), "wN", None, None)

    assert format_move(move) == "Ng1 - f3"


def test_format_capture():
    move = ((7, 3), (3, 7), "wQ", "bP", None)

    assert format_move(move) == "Qd1 - h5 +"


def test_format_promotion():
    move = ((1, 0), (0, 0), "wP", None, "Q")

    assert format_move(move) == "a7 - a8=Q"
