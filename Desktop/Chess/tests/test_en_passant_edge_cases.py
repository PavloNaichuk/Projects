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


def test_white_en_passant_rejected_if_it_exposes_king_to_rook():
    game = make_game_from_fen("4r2k/8/8/3pP3/8/8/8/4K3 w - d6 0 1")

    assert game.board.is_check("w") is False
    assert game.move_piece((3, 4), (2, 3)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[3][4] == "wP"
    assert game.board.board[3][3] == "bP"
    assert game.board.board[0][4] == "bR"
    assert game.turn == "w"


def test_white_en_passant_allowed_when_king_is_not_exposed():
    game = make_game_from_fen("7k/8/8/3pP3/8/8/8/7K w - d6 0 1")

    assert game.move_piece((3, 4), (2, 3)) is True

    assert game.board.board[2][3] == "wP"
    assert game.board.board[3][4] is None
    assert game.board.board[3][3] is None
    assert game.turn == "b"


def test_black_en_passant_rejected_if_it_exposes_king_to_rook():
    game = make_game_from_fen("4k3/8/8/8/3Pp3/8/8/4R2K b - d3 0 1")

    assert game.board.is_check("b") is False
    assert game.move_piece((4, 4), (5, 3)) is False

    assert game.board.board[0][4] == "bK"
    assert game.board.board[4][4] == "bP"
    assert game.board.board[4][3] == "wP"
    assert game.board.board[7][4] == "wR"
    assert game.turn == "b"


def test_black_en_passant_allowed_when_king_is_not_exposed():
    game = make_game_from_fen("7k/8/8/8/3Pp3/8/8/4R2K b - d3 0 1")

    assert game.move_piece((4, 4), (5, 3)) is True

    assert game.board.board[5][3] == "bP"
    assert game.board.board[4][4] is None
    assert game.board.board[4][3] is None
    assert game.turn == "w"
