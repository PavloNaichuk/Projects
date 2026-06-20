import pygame

from sidebar import get_button_background, get_move_window


def test_get_button_background_returns_active_color_for_clicked_button():
    rect = pygame.Rect(1, 2, 3, 4)

    assert get_button_background(
        show_side_rect=True,
        last_clicked_button_rect=rect,
        button_rect=rect,
        button_color=(1, 1, 1),
        active_color=(2, 2, 2),
    ) == (2, 2, 2)


def test_get_button_background_returns_default_color_when_not_active():
    rect = pygame.Rect(1, 2, 3, 4)
    other = pygame.Rect(5, 6, 7, 8)

    assert get_button_background(
        show_side_rect=True,
        last_clicked_button_rect=other,
        button_rect=rect,
        button_color=(1, 1, 1),
        active_color=(2, 2, 2),
    ) == (1, 1, 1)


def test_get_move_window_clamps_scroll_to_maximum():
    moves = list(range(20))

    moves_scroll, start_idx, visible_moves = get_move_window(
        move_log=moves,
        moves_scroll=99,
        max_visible_moves=12,
    )

    assert moves_scroll == 8
    assert start_idx == 8
    assert visible_moves == moves[8:20]


def test_get_move_window_returns_all_moves_when_less_than_visible_limit():
    moves = list(range(5))

    moves_scroll, start_idx, visible_moves = get_move_window(
        move_log=moves,
        moves_scroll=3,
        max_visible_moves=12,
    )

    assert moves_scroll == 0
    assert start_idx == 0
    assert visible_moves == moves
