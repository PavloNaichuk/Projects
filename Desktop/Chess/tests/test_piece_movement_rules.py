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


def test_white_pawn_cannot_move_backward():
    game = make_game_from_fen("7k/8/8/8/4P3/8/8/7K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((4, 4), (5, 4)) is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_black_pawn_cannot_move_backward():
    game = make_game_from_fen("7k/8/8/4p3/8/8/8/7K b - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((3, 4), (2, 4)) is False

    assert game.board.fen() == initial_fen
    assert game.turn == "b"


def test_pawn_cannot_move_diagonally_without_capture():
    game = make_game_from_fen("7k/8/8/8/4P3/8/8/7K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((4, 4), (3, 5)) is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_pawn_can_capture_diagonally():
    game = make_game_from_fen("7k/8/8/3p4/4P3/8/8/7K w - - 0 1")

    assert game.move_piece((4, 4), (3, 3)) is True

    assert game.board.board[3][3] == "wP"
    assert game.board.board[4][4] is None
    assert game.turn == "b"


def test_pawn_cannot_move_two_squares_after_leaving_starting_rank():
    game = make_game_from_fen("7k/8/8/8/4P3/8/8/7K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((4, 4), (2, 4)) is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_knight_can_jump_over_pieces_from_initial_position():
    game = Game()

    assert game.move_piece((7, 6), (5, 5)) is True  # Ng1 -> f3

    assert game.board.board[5][5] == "wN"
    assert game.board.board[7][6] is None
    assert game.turn == "b"


def test_bishop_cannot_jump_over_piece():
    game = Game()
    initial_fen = game.board.fen()

    assert game.move_piece((7, 2), (5, 4)) is False  # Bc1 blocked by d2 pawn

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_rook_cannot_jump_over_piece():
    game = Game()
    initial_fen = game.board.fen()

    assert game.move_piece((7, 0), (5, 0)) is False  # Ra1 blocked by a2 pawn

    assert game.board.fen() == initial_fen
    assert game.turn == "w"


def test_queen_can_move_diagonally_on_clear_board():
    game = make_game_from_fen("7k/8/8/8/8/8/8/3Q3K w - - 0 1")

    assert game.move_piece((7, 3), (4, 6)) is True

    assert game.board.board[4][6] == "wQ"
    assert game.board.board[7][3] is None
    assert game.turn == "b"


def test_queen_can_move_straight_on_clear_board():
    game = make_game_from_fen("7k/8/8/8/8/8/8/3Q3K w - - 0 1")

    assert game.move_piece((7, 3), (3, 3)) is True

    assert game.board.board[3][3] == "wQ"
    assert game.board.board[7][3] is None
    assert game.turn == "b"


def test_piece_cannot_capture_own_piece():
    game = make_game_from_fen("7k/8/8/8/8/8/4P3/3Q3K w - - 0 1")
    initial_fen = game.board.fen()

    assert game.move_piece((7, 3), (6, 4)) is False

    assert game.board.fen() == initial_fen
    assert game.turn == "w"
