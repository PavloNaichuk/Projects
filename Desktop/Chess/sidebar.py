import pygame

from move_notation import format_move


def get_button_background(
    show_side_rect,
    last_clicked_button_rect,
    button_rect,
    button_color,
    active_color,
):
    if show_side_rect and last_clicked_button_rect == button_rect:
        return active_color

    return button_color


def get_move_window(move_log, moves_scroll, max_visible_moves):
    total_moves = len(move_log)
    max_scroll = max(0, total_moves - max_visible_moves)
    moves_scroll = min(moves_scroll, max_scroll)

    start_idx = moves_scroll
    visible_moves = move_log[start_idx:start_idx + max_visible_moves]

    return moves_scroll, start_idx, visible_moves


def draw_button(win, font, rect, text, background, border_color, text_color):
    pygame.draw.rect(win, background, rect, border_radius=8)
    pygame.draw.rect(win, border_color, rect, 2, border_radius=8)

    label = font.render(text, True, text_color)
    win.blit(label, label.get_rect(center=rect.center))


def draw_timers(win, font, game, width, hint_rect):
    if not game.time_control or game.time_control[0] <= 0:
        return

    white_time = game.get_time_left("w")
    white_minutes, white_seconds = divmod(int(white_time), 60)
    white_text = font.render(
        f"White: {white_minutes:02d}:{white_seconds:02d}",
        True,
        (0, 0, 0),
    )
    win.blit(white_text, (width + 20, 20))

    black_time = game.get_time_left("b")
    black_minutes, black_seconds = divmod(int(black_time), 60)
    black_text = font.render(
        f"Black: {black_minutes:02d}:{black_seconds:02d}",
        True,
        (0, 0, 0),
    )
    win.blit(black_text, (width + 20, hint_rect.bottom + 20))


def draw_move_history(
    win,
    font,
    move_log,
    moves_scroll,
    max_visible_moves,
    width,
    start_y,
):
    x0 = width + 10
    row_h = 28
    col_w = 130

    moves_scroll, start_idx, visible_moves = get_move_window(
        move_log,
        moves_scroll,
        max_visible_moves,
    )

    total_moves = len(move_log)
    max_scroll = max(0, total_moves - max_visible_moves)

    scrollbar_rect = None
    scrollbar_area = None

    bar_x = x0 + col_w + 6
    bar_y = start_y
    bar_w = 13
    bar_h = max_visible_moves * row_h

    if total_moves > max_visible_moves:
        pygame.draw.rect(
            win,
            (220, 220, 220),
            (bar_x, bar_y, bar_w, bar_h),
            border_radius=6,
        )
        ratio = max_visible_moves / total_moves
        scroll_h = max(int(bar_h * ratio), 20)

        if max_scroll == 0:
            scroll_y = bar_y
        else:
            scroll_y = int(bar_y + (bar_h - scroll_h) * (moves_scroll / max_scroll))

        pygame.draw.rect(
            win,
            (180, 180, 180),
            (bar_x, scroll_y, bar_w, scroll_h),
            border_radius=5,
        )
        scrollbar_rect = pygame.Rect(bar_x, scroll_y, bar_w, scroll_h)
        scrollbar_area = pygame.Rect(bar_x, bar_y, bar_w, bar_h)

    for idx, move in enumerate(visible_moves):
        y = start_y + idx * row_h
        move_number = start_idx + idx + 1

        if move_number % 2 == 0:
            pygame.draw.rect(win, (190, 190, 210), (x0, y, col_w, row_h))

        number_text = font.render(f"{move_number}.", True, (0, 0, 0))
        win.blit(number_text, (x0, y + 4))

        move_text = font.render(format_move(move), True, (0, 0, 0))
        win.blit(move_text, (x0 + 35, y + 4))

    return moves_scroll, scrollbar_rect, scrollbar_area


def draw_sidebar(
    win,
    font,
    game,
    width,
    side_width,
    height,
    hint_rect,
    undo_rect,
    save_rect,
    load_rect,
    show_side_rect,
    last_clicked_button_rect,
    undo_hover,
    save_hover,
    load_hover,
    hint_hover,
    show_hints,
    moves_scroll,
    max_visible_moves,
    panel_color,
    button_color,
    button_text,
    hint_active_bg,
    active_color,
):
    pygame.draw.rect(win, panel_color, (width, 0, side_width, height))

    draw_timers(win, font, game, width, hint_rect)

    undo_bg = get_button_background(
        show_side_rect,
        last_clicked_button_rect,
        undo_rect,
        button_color,
        active_color,
    )
    save_bg = get_button_background(
        show_side_rect,
        last_clicked_button_rect,
        save_rect,
        button_color,
        active_color,
    )
    load_bg = get_button_background(
        show_side_rect,
        last_clicked_button_rect,
        load_rect,
        button_color,
        active_color,
    )
    hint_bg = hint_active_bg if show_hints else button_color

    undo_border = active_color if undo_hover else (0, 0, 0)
    save_border = active_color if save_hover else (0, 0, 0)
    load_border = active_color if load_hover else (0, 0, 0)
    hint_border = active_color if hint_hover else (0, 0, 0)

    draw_button(win, font, undo_rect, "Undo", undo_bg, undo_border, button_text)
    draw_button(win, font, save_rect, "Save", save_bg, save_border, button_text)
    draw_button(win, font, load_rect, "Load", load_bg, load_border, button_text)
    draw_button(win, font, hint_rect, "Hint", hint_bg, hint_border, button_text)

    timer_y = hint_rect.bottom + 20
    move_history_y = timer_y + 30

    return draw_move_history(
        win=win,
        font=font,
        move_log=game.move_log,
        moves_scroll=moves_scroll,
        max_visible_moves=max_visible_moves,
        width=width,
        start_y=move_history_y,
    )
