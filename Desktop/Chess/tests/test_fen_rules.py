from board import Board
from game import Game


def test_initial_board_fen_is_complete():
    board = Board()

    assert board.fen() == (
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR "
        "w KQkq - 0 1"
    )


def test_fen_round_trip_preserves_position_and_state():
    fen = "r3k2r/pppq1ppp/2npbn2/3Np3/2B1P3/2N2Q2/PPP2PPP/R3K2R b KQkq e3 42 17"

    board = Board(fen)

    assert board.fen() == fen


def test_fen_parses_turn_castling_en_passant_and_clocks():
    board = Board("r3k2r/8/8/8/8/8/8/R3K2R b Kq d3 15 9")

    assert board.turn == "b"
    assert board.castling_rights["w"]["K"] is True
    assert board.castling_rights["w"]["Q"] is False
    assert board.castling_rights["b"]["K"] is False
    assert board.castling_rights["b"]["Q"] is True
    assert board.ep_square == (5, 3)
    assert board.halfmove_clock == 15
    assert board.fullmove_number == 9


def test_fen_after_double_pawn_move_contains_en_passant_square():
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True

    assert game.board.fen() == (
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR "
        "b KQkq e3 0 1"
    )


def test_fen_after_black_move_increments_fullmove_number():
    game = Game()

    assert game.move_piece((6, 4), (4, 4)) is True  # e2 -> e4
    assert game.move_piece((1, 4), (3, 4)) is True  # e7 -> e5

    assert game.board.turn == "w"
    assert game.board.fullmove_number == 2
    assert game.board.fen() == (
        "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR "
        "w KQkq e6 0 2"
    )


def test_position_key_ignores_halfmove_and_fullmove_counters():
    first = Board("8/8/8/8/8/8/8/4k2K w - - 0 1")
    second = Board("8/8/8/8/8/8/8/4k2K w - - 99 50")

    assert first.position_key() == second.position_key()


def test_position_key_includes_turn_castling_and_en_passant():
    base = Board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1")
    different_turn = Board("8/8/8/8/8/8/8/R3K2R b KQ - 0 1")
    different_castling = Board("8/8/8/8/8/8/8/R3K2R w K - 0 1")
    different_ep = Board("8/8/8/8/8/8/8/R3K2R w KQ e3 0 1")

    assert base.position_key() != different_turn.position_key()
    assert base.position_key() != different_castling.position_key()
    assert base.position_key() != different_ep.position_key()
