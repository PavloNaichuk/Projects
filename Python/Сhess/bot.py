import copy

PIECE_VALUES = {'P': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 10000}

CENTER_SQUARES = {(3, 3), (3, 4), (4, 3), (4, 4)}

def evaluate_board(game, color='b'):
    """Evaluate material + center control + mobility + check bonus."""
    opponent = 'w' if color == 'b' else 'b'
    score = 0

    for r in range(8):
        for c in range(8):
            p = game.board[r][c]
            if p:
                val = PIECE_VALUES[p[1]]
                mult = 1 if p[0] == color else -1
                score += val * mult
                if (r, c) in CENTER_SQUARES:
                    score += 0.2 * mult

    own_moves = _get_legal_moves(game, color)
    opp_moves = _get_legal_moves(game, opponent)
    score += 0.1 * (len(own_moves) - len(opp_moves))

    if game.is_in_check(opponent):
        score += 5
    if _is_checkmate(game, opponent):
        score += 10000

    return score

def _get_legal_moves(game, color):
    """All legal moves for color (filters out those leaving king in check)."""
    moves = []
    for r in range(8):
        for c in range(8):
            p = game.board[r][c]
            if p and p[0] == color:
                for dest in game.get_valid_moves((r, c)):
                    # apply
                    captured = game.board[dest[0]][dest[1]]
                    game.move_piece((r, c), dest)
                    in_check = game.is_in_check(color)
                    # undo
                    game.undo_move()
                    if not in_check:
                        moves.append(((r, c), dest))
    return moves

def _is_checkmate(game, color):
    return game.is_in_check(color) and not _get_legal_moves(game, color)

def _is_stalemate(game, color):
    return not game.is_in_check(color) and not _get_legal_moves(game, color)

def _alphabeta(game, depth, alpha, beta, maximizing, root_color):
    """Minimax search with alpha-beta pruning."""
    color = root_color if maximizing else ('w' if root_color == 'b' else 'b')
    moves = _get_legal_moves(game, color)

    # terminal
    if depth == 0 or not moves:
        return evaluate_board(game, root_color), None

    best_move = None

    if maximizing:
        value = float('-inf')
        for move in moves:
            start, end = move
            game.move_piece(start, end)
            val, _ = _alphabeta(game, depth - 1, alpha, beta, False, root_color)
            game.undo_move()
            if val > value:
                value, best_move = val, move
            alpha = max(alpha, value)
            if alpha >= beta:
                break
        return value, best_move
    else:
        value = float('inf')
        for move in moves:
            start, end = move
            game.move_piece(start, end)
            val, _ = _alphabeta(game, depth - 1, alpha, beta, True, root_color)
            game.undo_move()
            if val < value:
                value, best_move = val, move
            beta = min(beta, value)
            if beta <= alpha:
                break
        return value, best_move

def bot_move(game, depth=3):
    """
    Compute and execute the best move for Black.
    Uses Minimax + Alpha-Beta to 'depth'.
    """
    if game.is_checkmate() or game.is_stalemate():
        return

    _, best = _alphabeta(game, depth, float('-inf'), float('inf'), True, 'b')
    if best:
        game.move_piece(*best)
        game.selected = None
