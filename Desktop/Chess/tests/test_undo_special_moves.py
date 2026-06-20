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


def assert_game_restored(game, expected_fen, expected_turn, expected_log_len):
    assert game.board.fen() == expected_fen
    assert game.turn == expected_turn
    assert game.board.turn == expected_turn
    assert len(game.move_log) == expected_log_len


def test_undo_after_kingside_castling_restores_position():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((7, 4), (7, 6)) is True
    assert game.undo_move() is True

    assert_game_restored(game, initial_fen, "w", 0)
    assert game.board.board[7][4] == "wK"
    assert game.board.board[7][7] == "wR"


def test_undo_after_queenside_castling_restores_position():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((7, 4), (7, 2)) is True
    assert game.undo_move() is True

    assert_game_restored(game, initial_fen, "w", 0)
    assert game.board.board[7][4] == "wK"
    assert game.board.board[7][0] == "wR"


def test_undo_after_en_passant_restores_position():
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True  # e2 -> e4
    assert game.move_piece((1, 0), (2, 0)) is True  # a7 -> a6
    assert game.move_piece((4, 4), (3, 4)) is True  # e4 -> e5
    assert game.move_piece((1, 3), (3, 3)) is True  # d7 -> d5

    fen_before_en_passant = game.board.fen()
    log_len_before_en_passant = len(game.move_log)

    assert game.move_piece((3, 4), (2, 3)) is True  # e5xd6 en passant
    assert game.board.board[2][3] == "wP"
    assert game.board.board[3][3] is None

    assert game.undo_move() is True

    assert_game_restored(
        game,
        fen_before_en_passant,
        "w",
        log_len_before_en_passant,
    )
    assert game.board.board[3][4] == "wP"
    assert game.board.board[3][3] == "bP"


def test_undo_after_promotion_restores_pawn():
    game = make_game_from_fen("8/P7/8/8/8/8/8/4k2K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((1, 0), (0, 0), promotion="Q") is True
    assert game.board.board[0][0] == "wQ"

    assert game.undo_move() is True

    assert_game_restored(game, initial_fen, "w", 0)
    assert game.board.board[1][0] == "wP"
    assert game.board.board[0][0] is None


def test_undo_after_capture_restores_captured_piece():
    game = make_game_from_fen("7k/8/8/8/8/8/4r3/4K3 w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((7, 4), (6, 4)) is True
    assert game.board.board[6][4] == "wK"
    assert game.board.board[7][4] is None

    assert game.undo_move() is True

    assert_game_restored(game, initial_fen, "w", 0)
    assert game.board.board[7][4] == "wK"
    assert game.board.board[6][4] == "bR"


def test_undo_returns_false_when_no_moves_exist():
    game = Game()

    assert game.undo_move() is False
    assert game.turn == "w"
    assert len(game.move_log) == 0
