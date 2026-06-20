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


def test_position_repeated_twice_is_not_threefold_repetition():
    game = Game()

    assert game.move_piece((7, 6), (5, 5)) is True  # Ng1 -> f3
    assert game.move_piece((0, 6), (2, 5)) is True  # Ng8 -> f6
    assert game.move_piece((5, 5), (7, 6)) is True  # Nf3 -> g1
    assert game.move_piece((2, 5), (0, 6)) is True  # Nf6 -> g8

    assert game.board.is_threefold_repetition() is False
    assert game.board.is_draw(game.turn) is False


def test_threefold_repetition_after_repeating_knight_moves():
    game = Game()

    for _ in range(2):
        assert game.move_piece((7, 6), (5, 5)) is True  # Ng1 -> f3
        assert game.move_piece((0, 6), (2, 5)) is True  # Ng8 -> f6
        assert game.move_piece((5, 5), (7, 6)) is True  # Nf3 -> g1
        assert game.move_piece((2, 5), (0, 6)) is True  # Nf6 -> g8

    assert game.board.is_threefold_repetition() is True
    assert game.board.is_draw(game.turn) is True


def test_threefold_repetition_detected_after_move_piece_sets_game_over():
    game = Game()

    for _ in range(2):
        assert game.move_piece((7, 6), (5, 5)) is True  # Ng1 -> f3
        assert game.move_piece((0, 6), (2, 5)) is True  # Ng8 -> f6
        assert game.move_piece((5, 5), (7, 6)) is True  # Nf3 -> g1
        assert game.move_piece((2, 5), (0, 6)) is True  # Nf6 -> g8

    assert game.game_over is True
    assert "threefold repetition" in game.game_over_message.lower()


def test_repetition_counter_is_restored_after_undo():
    game = Game()

    initial_fen = game.board.fen()

    assert game.move_piece((7, 6), (5, 5)) is True
    assert game.move_piece((0, 6), (2, 5)) is True
    assert game.move_piece((5, 5), (7, 6)) is True
    assert game.move_piece((2, 5), (0, 6)) is True

    assert game.board.fen() == initial_fen
    assert game.undo_move() is True

    assert game.board.is_threefold_repetition() is False
    assert game.game_over is False


def test_position_key_distinguishes_side_to_move_for_repetition():
    white_to_move = Board("8/8/8/8/8/8/8/4k2K w - - 0 1")
    black_to_move = Board("8/8/8/8/8/8/8/4k2K b - - 0 1")

    assert white_to_move.position_key() != black_to_move.position_key()


def test_position_key_distinguishes_castling_rights_for_repetition():
    with_castling = Board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1")
    without_castling = Board("8/8/8/8/8/8/8/R3K2R w - - 0 1")

    assert with_castling.position_key() != without_castling.position_key()


def test_position_key_distinguishes_en_passant_for_repetition():
    with_ep = Board("8/8/8/8/8/8/8/4k2K w - e3 0 1")
    without_ep = Board("8/8/8/8/8/8/8/4k2K w - - 0 1")

    assert with_ep.position_key() != without_ep.position_key()
