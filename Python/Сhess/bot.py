import copy

PIECE_VALUES = {'P': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 10000}

CENTER_SQUARES = {(3, 3), (3, 4), (4, 3), (4, 4)}

def evaluate_board(game, color='b'):
    opponent = 'w' if color == 'b' else 'b'
    score = 0

    for r in range(8):
        for c in range(8):
            p = game.board[r][c]
            if p:
                val = PIECE_VALUES[p[1]]
                score += val if p[0] == color else -val
                if (r, c) in CENTER_SQUARES:
                    score += 0.1 if p[0] == color else -0.1

    my_moves = len(game.get_all_moves(color))
    opp_moves = len(game.get_all_moves(opponent))
    score += 0.05 * (my_moves - opp_moves)

    if game.is_in_check(opponent):
        score += 0.5
    if game.is_in_check(color):
        score -= 0.5

    return score

def _alphabeta(game, depth, alpha, beta, maximizing, root_color):
    if depth == 0 or game.is_checkmate() or game.is_draw():
        return evaluate_board(game, root_color), None

    best_move = None
    if maximizing:
        value = float('-inf')
        for move in game.get_all_moves(root_color):
            new_game = copy.deepcopy(game)
            new_game.move_piece(move[0], move[1])
            val, _ = _alphabeta(new_game, depth-1, alpha, beta, False, root_color)
            if val > value:
                value = val
                best_move = move
            alpha = max(alpha, value)
            if alpha >= beta:
                break
        return value, best_move
    else:
        value = float('inf')
        opp_color = 'w' if root_color == 'b' else 'b'
        for move in game.get_all_moves(opp_color):
            new_game = copy.deepcopy(game)
            new_game.move_piece(move[0], move[1])
            val, _ = _alphabeta(new_game, depth-1, alpha, beta, True, root_color)
            if val < value:
                value = val
                best_move = move
            beta = min(beta, value)
            if beta <= alpha:
                break
        return value, best_move

def bot_move(game, depth=3):
    if game.is_checkmate() or game.is_draw():
        return
    _, best = _alphabeta(game, depth, float('-inf'), float('inf'), True, 'b')
    if best:
        game.move_piece(best[0], best[1])
        game.selected = None
