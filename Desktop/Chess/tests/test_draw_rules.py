from board import Board


def test_stalemate_is_draw():
    board = Board("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1")

    assert board.is_check("b") is False
    assert board.legal_moves("b") == []
    assert board.is_stalemate("b") is True
    assert board.is_draw("b") is True


def test_king_vs_king_is_insufficient_material():
    board = Board("8/8/8/8/8/8/8/4k2K w - - 0 1")

    assert board.is_insufficient_material() is True
    assert board.is_draw("w") is True


def test_king_and_bishop_vs_king_is_insufficient_material():
    board = Board("8/8/8/8/8/8/8/4k1BK w - - 0 1")

    assert board.is_insufficient_material() is True
    assert board.is_draw("w") is True


def test_king_and_knight_vs_king_is_insufficient_material():
    board = Board("8/8/8/8/8/8/8/4k1NK w - - 0 1")

    assert board.is_insufficient_material() is True
    assert board.is_draw("w") is True


def test_fifty_move_rule_is_draw():
    board = Board("8/8/8/8/8/8/8/4k2K w - - 100 1")

    assert board.is_fifty_move_rule() is True
    assert board.is_draw("w") is True


def test_fifty_move_counter_resets_after_pawn_move():
    board = Board("8/8/8/8/8/8/4P3/4k2K w - - 99 1")

    board.push(((6, 4), (5, 4), None))

    assert board.halfmove_clock == 0
    assert board.is_fifty_move_rule() is False


def test_fifty_move_counter_resets_after_capture():
    board = Board("8/8/8/8/8/8/4Q3/4k2K w - - 99 1")

    board.push(((6, 4), (7, 4), None))

    assert board.halfmove_clock == 0
    assert board.is_fifty_move_rule() is False
