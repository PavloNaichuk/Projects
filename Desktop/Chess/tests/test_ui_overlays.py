from ui_overlays import (
    get_board_square_from_mouse,
    has_friendly_piece_at_square,
    should_show_timed_message,
)


def test_should_show_timed_message_before_duration_expires():
    assert should_show_timed_message(
        start_time=1000,
        current_time=3999,
        duration_ms=3000,
    ) is True


def test_should_show_timed_message_after_duration_expires():
    assert should_show_timed_message(
        start_time=1000,
        current_time=4000,
        duration_ms=3000,
    ) is False


def test_get_board_square_from_mouse_inside_board():
    assert get_board_square_from_mouse(
        mouse_pos=(120, 240),
        square_size=80,
        board_width=640,
    ) == (3, 1)


def test_get_board_square_from_mouse_outside_board_returns_none():
    assert get_board_square_from_mouse(
        mouse_pos=(700, 240),
        square_size=80,
        board_width=640,
    ) is None


def test_has_friendly_piece_at_square():
    board = [[None for _ in range(8)] for _ in range(8)]
    board[3][4] = "wN"

    assert has_friendly_piece_at_square(
        board_matrix=board,
        square=(3, 4),
        selected=None,
        turn="w",
    ) is True


def test_has_friendly_piece_at_square_returns_false_for_enemy_piece():
    board = [[None for _ in range(8)] for _ in range(8)]
    board[3][4] = "bN"

    assert has_friendly_piece_at_square(
        board_matrix=board,
        square=(3, 4),
        selected=None,
        turn="w",
    ) is False


def test_has_friendly_piece_at_square_returns_false_for_selected_square():
    board = [[None for _ in range(8)] for _ in range(8)]
    board[3][4] = "wN"

    assert has_friendly_piece_at_square(
        board_matrix=board,
        square=(3, 4),
        selected=(3, 4),
        turn="w",
    ) is False
