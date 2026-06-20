from board import Board


def test_white_pawn_attacks_diagonally_up_the_board():
    board = Board("7k/8/8/8/4P3/8/8/7K w - - 0 1")

    assert board.is_attacked(3, 3, "w") is True  # d5
    assert board.is_attacked(3, 5, "w") is True  # f5
    assert board.is_attacked(3, 4, "w") is False  # e5
    assert board.is_attacked(5, 3, "w") is False  # d3
    assert board.is_attacked(5, 5, "w") is False  # f3


def test_black_pawn_attacks_diagonally_down_the_board():
    board = Board("7k/8/8/4p3/8/8/8/7K b - - 0 1")

    assert board.is_attacked(4, 3, "b") is True  # d4
    assert board.is_attacked(4, 5, "b") is True  # f4
    assert board.is_attacked(4, 4, "b") is False  # e4
    assert board.is_attacked(2, 3, "b") is False  # d6
    assert board.is_attacked(2, 5, "b") is False  # f6


def test_knight_attacks_l_shaped_squares():
    board = Board("7k/8/8/8/4N3/8/8/7K w - - 0 1")

    expected_attacks = {
        (2, 3), (2, 5),
        (3, 2), (3, 6),
        (5, 2), (5, 6),
        (6, 3), (6, 5),
    }

    for square in expected_attacks:
        assert board.is_attacked(*square, "w") is True

    assert board.is_attacked(3, 4, "w") is False
    assert board.is_attacked(4, 5, "w") is False


def test_bishop_attacks_diagonally_until_blocked():
    board = Board("7k/8/8/8/3B4/2P5/8/7K w - - 0 1")

    assert board.is_attacked(3, 4, "w") is True  # e5
    assert board.is_attacked(2, 5, "w") is True  # f6
    assert board.is_attacked(5, 4, "w") is True  # e3
    assert board.is_attacked(5, 2, "w") is True  # c3 own blocker square
    assert board.is_attacked(6, 1, "w") is False  # b2 behind blocker


def test_rook_attacks_straight_until_blocked():
    board = Board("7k/8/8/8/3R4/3P4/8/7K w - - 0 1")

    assert board.is_attacked(4, 0, "w") is True  # a4
    assert board.is_attacked(4, 7, "w") is True  # h4
    assert board.is_attacked(3, 3, "w") is True  # d5
    assert board.is_attacked(5, 3, "w") is True  # d3 own blocker square
    assert board.is_attacked(6, 3, "w") is False  # d2 behind blocker


def test_queen_combines_rook_and_bishop_attacks():
    board = Board("7k/8/8/8/3Q4/8/8/7K w - - 0 1")

    assert board.is_attacked(4, 7, "w") is True  # horizontal
    assert board.is_attacked(0, 3, "w") is True  # vertical
    assert board.is_attacked(1, 0, "w") is True  # diagonal
    assert board.is_attacked(1, 6, "w") is True  # diagonal
    assert board.is_attacked(2, 4, "w") is False  # not queen line


def test_king_attacks_adjacent_squares_only():
    board = Board("7k/8/8/8/4K3/8/8/8 w - - 0 1")

    adjacent = {
        (3, 3), (3, 4), (3, 5),
        (4, 3),         (4, 5),
        (5, 3), (5, 4), (5, 5),
    }

    for square in adjacent:
        assert board.is_attacked(*square, "w") is True

    assert board.is_attacked(2, 4, "w") is False
    assert board.is_attacked(4, 6, "w") is False
