def is_board_click(mouse_pos, board_width):
    mouse_x, _ = mouse_pos

    return mouse_x < board_width


def get_board_square(mouse_pos, square_size):
    mouse_x, mouse_y = mouse_pos

    return mouse_y // square_size, mouse_x // square_size


def is_move_allowed(mode, vs_bot, game_turn, net, local_turn):
    return (
        mode == "local"
        or (vs_bot and game_turn == "w")
        or (net and game_turn == local_turn)
    )


def is_pawn(piece):
    return bool(piece and len(piece) > 1 and piece[1] == "P")


def has_pawn_reached_last_rank(piece, row):
    if not is_pawn(piece):
        return False

    return (piece[0] == "w" and row == 0) or (piece[0] == "b" and row == 7)
