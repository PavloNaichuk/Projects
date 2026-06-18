from board import Board
from game import Game


def test_initial_position_has_20_legal_moves():
    game = Game()

    assert len(game.get_all_moves('w')) == 20


def test_illegal_pawn_move_is_rejected():
    game = Game()
    initial_fen = game.board.fen()

    moved = game.move_piece((6, 0), (3, 0))  # a2 -> a5 is illegal

    assert moved is False
    assert game.board.fen() == initial_fen
    assert game.turn == 'w'


def test_legal_pawn_move_updates_turn_and_en_passant_square():
    game = Game()

    moved = game.move_piece((6, 4), (4, 4))  # e2 -> e4

    assert moved is True
    assert game.board.board[4][4] == 'wP'
    assert game.board.board[6][4] is None
    assert game.turn == 'b'
    assert game.board.ep_square == (5, 4)


def test_undo_restores_position_after_move():
    game = Game()
    initial_fen = game.board.fen()

    assert game.move_piece((6, 4), (4, 4)) is True
    assert game.undo_move() is True

    assert game.board.fen() == initial_fen
    assert game.turn == 'w'
    assert game.move_log == []


def test_fools_mate_is_checkmate():
    game = Game()

    assert game.move_piece((6, 5), (5, 5)) is True  # f2 -> f3
    assert game.move_piece((1, 4), (3, 4)) is True  # e7 -> e5
    assert game.move_piece((6, 6), (4, 6)) is True  # g2 -> g4
    assert game.move_piece((0, 3), (4, 7)) is True  # Qd8 -> Qh4

    assert game.game_over is True
    assert game.board.is_checkmate('w') is True


def test_en_passant_capture():
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True  # e2 -> e4
    assert game.move_piece((1, 0), (2, 0)) is True  # a7 -> a6
    assert game.move_piece((4, 4), (3, 4)) is True  # e4 -> e5
    assert game.move_piece((1, 3), (3, 3)) is True  # d7 -> d5
    assert game.move_piece((3, 4), (2, 3)) is True  # e5xd6 en passant

    assert game.board.board[2][3] == 'wP'
    assert game.board.board[3][3] is None
    assert game.board.board[3][4] is None


def test_default_promotion_is_queen():
    game = Game()
    game.board = Board("8/P7/8/8/8/8/8/4k2K w - - 0 1")
    game.turn = game.board.turn
    game.move_log = []
    game.position_history = [game.board.position_key()]

    assert game.move_piece((1, 0), (0, 0)) is True
    assert game.board.board[0][0] == 'wQ'


def test_save_and_load_json(tmp_path):
    save_file = tmp_path / "save.json"
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True
    game.save_game(save_file)

    loaded = Game()
    loaded.load_game(save_file)

    assert loaded.board.board == game.board.board
    assert loaded.turn == game.turn
    assert loaded.move_log == [
        [[6, 4], [4, 4], 'wP', None, None]
    ]
