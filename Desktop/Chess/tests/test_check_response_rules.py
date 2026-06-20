from board import Board
from game import Game


def make_game_from_fen(fen):
    game = Game()
    game.board = Board(fen)
    game.turn = game.board.turn
    game.move_log = []
    game.position_history = [game.board.position_key()]
    game.game_over = False
    game.game_over_message = ""
    return game


def test_sliding_check_can_be_blocked():
    game = make_game_from_fen("4r2k/8/8/8/8/8/2B5/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((6, 2), (4, 4)) is True  # Bc2 -> e4

    assert game.board.board[4][4] == "wB"
    assert game.board.is_check("w") is False
    assert game.turn == "b"


def test_sliding_check_cannot_be_ignored():
    game = make_game_from_fen("4r2k/8/8/8/8/8/2B5/4K3 w - - 0 1")
    initial_fen = game.board.fen()

    assert game.board.is_check("w") is True
    assert game.move_piece((6, 2), (5, 1)) is False  # Bc2 -> b3 does not help

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_knight_check_cannot_be_blocked():
    game = make_game_from_fen("7k/8/8/8/8/5n2/8/2B1K3 w - - 0 1")
    initial_fen = game.board.fen()

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 2), (5, 4)) is False  # Bc1 -> e3 cannot block a knight

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_knight_check_can_be_answered_by_capturing_knight():
    game = make_game_from_fen("7k/8/8/8/8/5n2/8/4KR2 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 5), (5, 5)) is True  # Rf1xf3

    assert game.board.board[5][5] == "wR"
    assert game.board.board[7][5] is None
    assert game.board.is_check("w") is False
    assert game.turn == "b"


def test_double_check_cannot_be_blocked_by_non_king_move():
    game = make_game_from_fen("4r2k/8/8/8/1b6/8/8/2B1K3 w - - 0 1")
    initial_fen = game.board.fen()

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 2), (5, 4)) is False  # Bc1 -> e3 blocks only rook

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_double_check_can_be_answered_by_king_move():
    game = make_game_from_fen("4r2k/8/8/8/1b6/8/8/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 4), (7, 5)) is True  # Ke1 -> f1

    assert game.board.board[7][5] == "wK"
    assert game.board.is_check("w") is False
    assert game.turn == "b"
