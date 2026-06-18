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


def test_white_kingside_castling_allowed():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")

    assert game.move_piece((7, 4), (7, 6)) is True

    assert game.board.board[7][6] == "wK"
    assert game.board.board[7][5] == "wR"
    assert game.board.board[7][4] is None
    assert game.board.board[7][7] is None
    assert game.turn == "b"


def test_white_queenside_castling_allowed():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")

    assert game.move_piece((7, 4), (7, 2)) is True

    assert game.board.board[7][2] == "wK"
    assert game.board.board[7][3] == "wR"
    assert game.board.board[7][4] is None
    assert game.board.board[7][0] is None
    assert game.turn == "b"


def test_castling_rejected_when_piece_blocks_path():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3KB1R w KQ - 0 1")

    assert game.move_piece((7, 4), (7, 6)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[7][7] == "wR"
    assert game.turn == "w"


def test_castling_rejected_when_king_is_in_check():
    game = make_game_from_fen("4r3/8/8/8/8/8/8/R3K2R w KQ - 0 1")

    assert game.board.is_check("w") is True
    assert game.move_piece((7, 4), (7, 6)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[7][7] == "wR"
    assert game.turn == "w"


def test_castling_rejected_through_attacked_square():
    game = make_game_from_fen("4k3/8/8/8/8/5r2/8/R3K2R w KQ - 0 1")

    assert game.board.is_attacked(7, 5, "b") is True
    assert game.move_piece((7, 4), (7, 6)) is False

    assert game.board.board[7][4] == "wK"
    assert game.board.board[7][7] == "wR"
    assert game.turn == "w"


def test_castling_rights_removed_after_king_moves():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")

    assert game.move_piece((7, 4), (6, 4)) is True
    assert game.board.castling_rights["w"]["K"] is False
    assert game.board.castling_rights["w"]["Q"] is False


def test_castling_rights_removed_after_rook_moves():
    game = make_game_from_fen("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1")

    assert game.move_piece((7, 7), (7, 6)) is True

    assert game.board.castling_rights["w"]["K"] is False
    assert game.board.castling_rights["w"]["Q"] is True
