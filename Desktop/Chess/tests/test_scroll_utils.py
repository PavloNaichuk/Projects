from scroll_utils import (
    calculate_scroll_from_drag,
    clamp_scroll,
    get_max_scroll,
    update_scroll_from_wheel,
)


def test_get_max_scroll():
    assert get_max_scroll(total_items=20, max_visible_items=12) == 8
    assert get_max_scroll(total_items=5, max_visible_items=12) == 0


def test_clamp_scroll():
    assert clamp_scroll(-5, max_scroll=8) == 0
    assert clamp_scroll(4, max_scroll=8) == 4
    assert clamp_scroll(99, max_scroll=8) == 8


def test_calculate_scroll_from_drag_at_top():
    assert calculate_scroll_from_drag(
        mouse_y=100,
        bar_y=100,
        bar_h=240,
        scroll_h=80,
        drag_offset=0,
        max_scroll=8,
    ) == 0


def test_calculate_scroll_from_drag_at_bottom():
    assert calculate_scroll_from_drag(
        mouse_y=260,
        bar_y=100,
        bar_h=240,
        scroll_h=80,
        drag_offset=0,
        max_scroll=8,
    ) == 8


def test_calculate_scroll_from_drag_clamps_above_and_below():
    assert calculate_scroll_from_drag(
        mouse_y=0,
        bar_y=100,
        bar_h=240,
        scroll_h=80,
        drag_offset=0,
        max_scroll=8,
    ) == 0

    assert calculate_scroll_from_drag(
        mouse_y=999,
        bar_y=100,
        bar_h=240,
        scroll_h=80,
        drag_offset=0,
        max_scroll=8,
    ) == 8


def test_calculate_scroll_from_drag_when_no_available_height():
    assert calculate_scroll_from_drag(
        mouse_y=100,
        bar_y=100,
        bar_h=80,
        scroll_h=80,
        drag_offset=0,
        max_scroll=8,
    ) == 0


def test_update_scroll_from_wheel():
    assert update_scroll_from_wheel(5, wheel_y=1, max_scroll=8) == 4
    assert update_scroll_from_wheel(5, wheel_y=-1, max_scroll=8) == 6
    assert update_scroll_from_wheel(0, wheel_y=1, max_scroll=8) == 0
    assert update_scroll_from_wheel(8, wheel_y=-1, max_scroll=8) == 8
    assert update_scroll_from_wheel(5, wheel_y=0, max_scroll=8) == 5
