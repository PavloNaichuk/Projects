def get_max_scroll(total_items, max_visible_items):
    return max(0, total_items - max_visible_items)


def clamp_scroll(value, max_scroll):
    return max(0, min(value, max_scroll))


def calculate_scroll_from_drag(
    mouse_y,
    bar_y,
    bar_h,
    scroll_h,
    drag_offset,
    max_scroll,
):
    available_height = bar_h - scroll_h

    if available_height <= 0:
        return 0

    relative_y = mouse_y - bar_y - drag_offset
    relative_y = max(0, min(relative_y, available_height))
    scroll_position = relative_y / available_height

    return int(scroll_position * max_scroll)


def update_scroll_from_wheel(current_scroll, wheel_y, max_scroll):
    if wheel_y > 0:
        return max(0, current_scroll - 1)

    if wheel_y < 0:
        return min(current_scroll + 1, max_scroll)

    return current_scroll
