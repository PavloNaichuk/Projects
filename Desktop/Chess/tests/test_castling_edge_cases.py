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


def test_black_kingside_castling_allowed():
    game = make_game_from_fen("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1")

    assert game.move_piece((0, 4), (0, 6)) is True

    assert game.board.board[0][6] == "bK"
    assert game.board.board[0][5] == "bR"
    assert game.board.board[0][4] is None
    assert game.board.board[0][7] is None
    assert game.turn == "w"


def test_black_queenside_castling_allowed():
    game = make_game_from_fen("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1")

    assert game.move_piece((0, 4), (0, 2)) is True

    assert game.board.board[0][2] == "bK"
    assert game.board.board[0][3] == "bR"
    assert game.board.board[0][4] is None
    assert game.board.board[0][0] is None
    assert game.turn == "w"


def test_black_castling_rejected_when_path_is_blocked():
    game = make_game_from_fen("r3kb1r/8/8/8/8/8/8/4K3 b kq - 0 1")

    assert game.move_piece((0, 4), (0, 6)) is False

    assert game.board.board[0][4] == "bK"
    assert game.board.board[0][7] == "bR"
    assert game.turn == "b"


def test_black_castling_rejected_when_king_is_in_check():
    game = make_game_from_fen("r3k2r/8/8/8/8/8/8/4R2K b kq - 0 1")

    assert game.board.is_check("b") is True
    assert game.move_piece((0, 4), (0, 6)) is False

    assert game.board.board[0][4] == "bK"
    assert game.board.board[0][7] == "bR"
    assert game.turn == "b"


def test_black_castling_rejected_through_attacked_square():
    game = make_game_from_fen("r3k2r/8/8/8/8/5R2/8/4K3 b kq - 0 1")

    assert game.board.is_attacked(0, 5, "w") is True
    assert game.move_piece((0, 4), (0, 6)) is False

    assert game.board.board[0][4] == "bK"
    assert game.board.board[0][7] == "bR"
    assert game.turn == "b"


def test_white_castling_rights_removed_when_kingside_rook_is_captured():
    game = make_game_from_fen("4k3/8/8/8/8/8/7q/R3K2R b KQ - 0 1")

    assert game.move_piece((6, 7), (7, 7)) is True

    assert game.board.board[7][7] == "bQ"
    assert game.board.castling_rights["w"]["K"] is False
    assert game.board.castling_rights["w"]["Q"] is True


def test_white_castling_rights_removed_when_queenside_rook_is_captured():
    game = make_game_from_fen("4k3/8/8/8/8/8/q7/R3K2R b KQ - 0 1")

    assert game.move_piece((6, 0), (7, 0)) is True

    assert game.board.board[7][0] == "bQ"
    assert game.board.castling_rights["w"]["K"] is True
    assert game.board.castling_rights["w"]["Q"] is False


def test_black_castling_rights_removed_when_kingside_rook_is_captured():
    game = make_game_from_fen("r3k2r/7Q/8/8/8/8/8/4K3 w kq - 0 1")

    assert game.move_piece((1, 7), (0, 7)) is True

    assert game.board.board[0][7] == "wQ"
    assert game.board.castling_rights["b"]["K"] is False
    assert game.board.castling_rights["b"]["Q"] is True


def test_black_castling_rights_removed_when_queenside_rook_is_captured():
    game = make_game_from_fen("r3k2r/Q7/8/8/8/8/8/4K3 w kq - 0 1")

    assert game.move_piece((1, 0), (0, 0)) is True

    assert game.board.board[0][0] == "wQ"
    assert game.board.castling_rights["b"]["K"] is True
    assert game.board.castling_rights["b"]["Q"] is False
