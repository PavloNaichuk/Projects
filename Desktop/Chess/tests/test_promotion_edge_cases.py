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


def test_white_can_promote_to_rook():
    game = make_game_from_fen("7k/P7/8/8/8/8/8/7K w - - 0 1")

    assert game.move_piece((1, 0), (0, 0), promotion="R") is True

    assert game.board.board[0][0] == "wR"
    assert game.turn == "b"


def test_white_can_promote_to_knight():
    game = make_game_from_fen("7k/P7/8/8/8/8/8/7K w - - 0 1")

    assert game.move_piece((1, 0), (0, 0), promotion="N") is True

    assert game.board.board[0][0] == "wN"
    assert game.turn == "b"


def test_black_default_promotion_is_queen():
    game = make_game_from_fen("7k/8/8/8/8/8/p7/7K b - - 0 1")

    assert game.move_piece((6, 0), (7, 0)) is True

    assert game.board.board[7][0] == "bQ"
    assert game.turn == "w"


def test_black_can_promote_to_bishop():
    game = make_game_from_fen("7k/8/8/8/8/8/p7/7K b - - 0 1")

    assert game.move_piece((6, 0), (7, 0), promotion="B") is True

    assert game.board.board[7][0] == "bB"
    assert game.turn == "w"


def test_promotion_with_capture():
    game = make_game_from_fen("1r5k/P7/8/8/8/8/8/7K w - - 0 1")

    assert game.move_piece((1, 0), (0, 1), promotion="Q") is True

    assert game.board.board[0][1] == "wQ"
    assert game.board.board[1][0] is None
    assert game.move_log[-1][3] == "bR"
    assert game.turn == "b"


def test_invalid_promotion_piece_is_rejected():
    game = make_game_from_fen("7k/P7/8/8/8/8/8/7K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((1, 0), (0, 0), promotion="X") is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_non_pawn_move_with_promotion_is_rejected():
    game = make_game_from_fen("7k/8/8/8/8/8/8/R6K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((7, 0), (6, 0), promotion="Q") is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"
