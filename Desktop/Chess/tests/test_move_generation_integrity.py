from board import Board


def perft(board, depth):
    if depth == 0:
        return 1

    total = 0
    for move in list(board.legal_moves()):
        board.push(move)
        total += perft(board, depth - 1)
        board.pop()

    return total


def assert_all_legal_moves_keep_king_safe(fen):
    board = Board(fen)
    color = board.turn

    for move in list(board.legal_moves()):
        board.push(move)

        try:
            assert not board.is_check(color), f"Move leaves king in check: {move}"
        finally:
            board.pop()


def test_initial_position_perft_depth_1():
    board = Board()

    assert perft(board, 1) == 20


def test_initial_position_perft_depth_2():
    board = Board()

    assert perft(board, 2) == 400


def test_push_pop_restores_fen_for_all_initial_legal_moves():
    board = Board()
    initial_fen = board.fen()

    for move in list(board.legal_moves()):
        board.push(move)
        board.pop()

        assert board.fen() == initial_fen


def test_all_initial_legal_moves_keep_king_safe():
    assert_all_legal_moves_keep_king_safe(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    )


def test_all_castling_position_legal_moves_keep_king_safe():
    assert_all_legal_moves_keep_king_safe(
        "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
    )


def test_en_passant_pin_position_does_not_generate_illegal_king_exposure():
    assert_all_legal_moves_keep_king_safe(
        "4r2k/8/8/3pP3/8/8/8/4K3 w - d6 0 1"
    )
