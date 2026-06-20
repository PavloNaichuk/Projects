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


def test_pinned_piece_cannot_move_and_expose_king():
    game = make_game_from_fen("7k/4r3/8/8/8/8/4R3/4K3 w - - 0 1")

    assert game.board.is_check("w") is False
    assert game.move_piece((6, 4), (6, 3)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[6][4] == "wR"
    assert game.board.board[1][4] == "bR"
    assert game.turn == "w"


def test_pinned_piece_can_capture_attacking_piece_on_same_line():
    game = make_game_from_fen("7k/4r3/8/8/8/8/4R3/4K3 w - - 0 1")

    assert game.move_piece((6, 4), (1, 4)) is True

    assert game.board.board[1][4] == "wR"
    assert game.board.board[6][4] is None
    assert game.board.is_check("w") is False
    assert game.turn == "b"


def test_king_cannot_move_into_check():
    game = make_game_from_fen("7k/4r3/8/8/8/8/8/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 4), (6, 4)) is False

    assert game.board.board[7][4] == "wK"
    assert game.turn == "w"


def test_legal_move_can_block_check():
    game = make_game_from_fen("4r2k/8/8/8/8/8/3B4/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((6, 3), (5, 4)) is True

    assert game.board.board[5][4] == "wB"
    assert game.board.board[6][3] is None
    assert game.board.is_check("w") is False
    assert game.turn == "b"


def test_king_can_capture_unprotected_checking_piece():
    game = make_game_from_fen("7k/8/8/8/8/8/4r3/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 4), (6, 4)) is True

    assert game.board.board[6][4] == "wK"
    assert game.board.board[7][4] is None
    assert game.board.is_check("w") is False
    assert game.turn == "b"


def test_king_cannot_capture_protected_piece():
    game = make_game_from_fen("7k/8/8/7b/8/8/4r3/4K3 w - - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 4), (6, 4)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[6][4] == "bR"
    assert game.turn == "w"
