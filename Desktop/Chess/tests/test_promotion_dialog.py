import pygame

from promotion_dialog import (
    PROMOTION_OPTIONS,
    build_promotion_rects,
    get_promotion_choice_from_position,
)


def test_build_promotion_rects_centers_four_options():
    rects = build_promotion_rects(
        board_width=640,
        board_height=640,
        square_size=80,
    )

    assert len(rects) == 4
    assert rects[0] == pygame.Rect(160, 280, 80, 80)
    assert rects[1] == pygame.Rect(240, 280, 80, 80)
    assert rects[2] == pygame.Rect(320, 280, 80, 80)
    assert rects[3] == pygame.Rect(400, 280, 80, 80)


def test_get_promotion_choice_from_position():
    rects = build_promotion_rects(
        board_width=640,
        board_height=640,
        square_size=80,
    )

    for option, rect in zip(PROMOTION_OPTIONS, rects, strict=True):
        assert get_promotion_choice_from_position(rect.center, rects) == option


def test_get_promotion_choice_returns_none_outside_options():
    rects = build_promotion_rects(
        board_width=640,
        board_height=640,
        square_size=80,
    )

    assert get_promotion_choice_from_position((10, 10), rects) is None
