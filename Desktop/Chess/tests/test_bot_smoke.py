from board import Board
from bot import bot_move
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


def test_bot_returns_legal_move_for_black_after_white_opening_move():
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True  # e2 -> e4

    move = bot_move(game, max_depth=1, time_limit=0.2)

    assert move is not None
    assert move in game.get_all_moves("b")


def test_bot_returns_legal_move_for_white_when_white_to_move():
    game = Game()

    move = bot_move(game, max_depth=1, time_limit=0.2)

    assert move is not None
    assert move in game.get_all_moves("w")


def test_bot_does_not_mutate_game_state_when_choosing_move():
    game = Game()
    assert game.move_piece((6, 4), (4, 4)) is True  # e2 -> e4

    fen_before = game.board.fen()
    turn_before = game.turn
    move_log_before = list(game.move_log)

    move = bot_move(game, max_depth=1, time_limit=0.2)

    assert move is not None
    assert game.board.fen() == fen_before
    assert game.turn == turn_before
    assert game.move_log == move_log_before


def test_bot_returns_none_when_side_has_no_legal_moves():
    game = make_game_from_fen("7k/6Q1/6K1/8/8/8/8/8 b - - 0 1")

    assert game.board.is_checkmate("b") is True

    move = bot_move(game, max_depth=1, time_limit=0.2)

    assert move is None
