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


def test_queen_and_king_checkmate_position():
    board = Board("7k/6Q1/6K1/8/8/8/8/8 b - - 0 1")

    assert board.is_check("b") is True
    assert board.legal_moves("b") == []
    assert board.is_checkmate("b") is True
    assert board.is_stalemate("b") is False


def test_queen_and_king_stalemate_position():
    board = Board("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1")

    assert board.is_check("b") is False
    assert board.legal_moves("b") == []
    assert board.is_stalemate("b") is True
    assert board.is_checkmate("b") is False


def test_check_is_not_checkmate_if_king_can_capture_unprotected_piece():
    board = Board("7k/6Q1/8/8/8/8/8/7K b - - 0 1")

    assert board.is_check("b") is True
    assert board.is_checkmate("b") is False
    assert ((0, 7), (1, 6), None) in board.legal_moves("b")


def test_stalemate_is_not_detected_when_side_has_legal_move():
    board = Board("7k/8/6K1/8/8/8/8/8 b - - 0 1")

    assert board.is_check("b") is False
    assert board.legal_moves("b") != []
    assert board.is_stalemate("b") is False


def test_game_over_after_checkmating_move():
    game = make_game_from_fen("7k/8/6K1/8/8/8/8/Q7 w - - 0 1")

    assert game.move_piece((7, 0), (1, 6)) is True  # Qa1 -> g7#

    assert game.game_over is True
    assert "Checkmate" in game.game_over_message
    assert game.board.is_checkmate("b") is True
