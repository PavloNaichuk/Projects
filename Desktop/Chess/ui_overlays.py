import pygame


def should_show_timed_message(start_time, current_time, duration_ms=3000):
    return current_time - start_time < duration_ms


def get_board_square_from_mouse(mouse_pos, square_size, board_width):
    mouse_x, mouse_y = mouse_pos

    if mouse_x >= board_width:
        return None

    return mouse_y // square_size, mouse_x // square_size


def has_friendly_piece_at_square(board_matrix, square, selected, turn):
    if square is None or square == selected:
        return False

    row, col = square
    piece = board_matrix[row][col]

    return bool(piece and piece[0] == turn)


def draw_last_move_highlight(win, move_log, square_size, color=(160, 110, 60)):
    if not move_log:
        return

    last_move = move_log[-1]
    start, end = last_move[0], last_move[1]

    highlight = pygame.Surface((square_size, square_size), pygame.SRCALPHA)
    highlight.fill(color)

    win.blit(highlight, (start[1] * square_size, start[0] * square_size))
    win.blit(highlight, (end[1] * square_size, end[0] * square_size))


def draw_selected_square(win, selected, square_size, highlight_color):
    if not selected:
        return

    row, col = selected
    pygame.draw.rect(
        win,
        highlight_color,
        (col * square_size, row * square_size, square_size, square_size),
        4,
    )


def draw_hint_squares(
    win,
    show_hints,
    selected,
    hint_squares,
    square_size,
    highlight_color,
):
    if not show_hints or not selected:
        return

    for row, col in hint_squares:
        surface = pygame.Surface((square_size, square_size), pygame.SRCALPHA)
        pygame.draw.rect(surface, (*highlight_color, 100), surface.get_rect())
        win.blit(surface, (col * square_size, row * square_size))


def draw_hover_square(
    win,
    board_matrix,
    selected,
    turn,
    square_size,
    board_width,
    hover_color,
    mouse_pos=None,
):
    mouse_pos = mouse_pos or pygame.mouse.get_pos()
    square = get_board_square_from_mouse(mouse_pos, square_size, board_width)

    if not has_friendly_piece_at_square(board_matrix, square, selected, turn):
        return

    row, col = square
    surface = pygame.Surface((square_size, square_size), pygame.SRCALPHA)
    pygame.draw.rect(surface, (*hover_color, 255), surface.get_rect(), 4)
    win.blit(surface, (col * square_size, row * square_size))


def draw_wait_overlay(win, font, board_width, board_height, side_width):
    overlay = pygame.Surface((board_width + side_width, board_height))
    overlay.set_alpha(150)
    overlay.fill((0, 0, 0))
    win.blit(overlay, (0, 0))

    text = font.render("Please wait...", True, (255, 255, 255))
    text_rect = text.get_rect(center=(board_width // 2, board_height // 2))
    win.blit(text, text_rect)


def draw_illegal_move_overlay(win, message):
    overlay = pygame.Surface(win.get_size(), pygame.SRCALPHA)
    overlay.fill((0, 0, 0, 180))
    win.blit(overlay, (0, 0))

    font = pygame.font.SysFont("Arial", 40, bold=True)
    text = font.render(message, True, (255, 0, 0))
    text_rect = text.get_rect(
        center=(win.get_width() // 2, win.get_height() // 2)
    )
    win.blit(text, text_rect)
