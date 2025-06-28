import copy
import time

PIECE_VALUES = {'P': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 10000}

CENTER_SQUARES = {(3, 3), (3, 4), (4, 3), (4, 4)}

PAWN_POSITION_SCORE = [
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5],
    [0.2, 0.2, 0.3, 0.4, 0.4, 0.3, 0.2, 0.2],
    [0.1, 0.1, 0.2, 0.3, 0.3, 0.2, 0.1, 0.1],
    [0, 0, 0.1, 0.2, 0.2, 0.1, 0, 0],
    [0.1, -0.1, -0.1, 0, 0, -0.1, -0.1, 0.1],
    [0.1, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, 0.1],
    [0, 0, 0, 0, 0, 0, 0, 0]
]

def evaluate_board(game, color='b'):
    opponent = 'w' if color == 'b' else 'b'
    score = 0

    for r in range(8):
        for c in range(8):
            piece = game.board[r][c]
            if piece:
                piece_type = piece[1]
                piece_color = piece[0]

                val = PIECE_VALUES[piece_type]
                position_bonus = 0

                if piece_type == 'P':
                    position_bonus = PAWN_POSITION_SCORE[r][c] if piece_color == 'w' else PAWN_POSITION_SCORE[7-r][c]
                elif piece_type in ('N', 'B'):
                    position_bonus = 0.1 if (r, c) in CENTER_SQUARES else 0

                total_piece_value = val + position_bonus

                score += total_piece_value if piece_color == color else -total_piece_value

    my_moves = len(game.get_all_moves(color))
    opp_moves = len(game.get_all_moves(opponent))
    score += 0.1 * (my_moves - opp_moves)

    if game.is_checkmate():
        if game.turn == color:
            score -= 9999
        else:
            score += 9999
    elif game.is_in_check(opponent):
        score += 1
    elif game.is_in_check(color):
        score -= 1

    if game.is_stalemate() or game.is_draw():
        score -= 50

    return score

def _alphabeta(game, depth, alpha, beta, maximizing, root_color):
    if depth == 0 or game.is_checkmate() or game.is_draw():
        return evaluate_board(game, root_color), None

    moves = game.get_all_moves(root_color if maximizing else ('w' if root_color == 'b' else 'b'))
    moves.sort(key=lambda move: evaluate_move(game, move, root_color), reverse=maximizing)

    best_move = None
    if maximizing:
        value = float('-inf')
        for move in moves:
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
        for move in moves:
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

def evaluate_move(game, move, root_color):
    new_game = copy.deepcopy(game)
    new_game.move_piece(move[0], move[1])
    return evaluate_board(new_game, root_color)


def bot_move(game, max_depth=3, max_time=2.0):
    start_time = time.time()
    best_move = None

    for depth in range(1, max_depth + 1):
        if time.time() - start_time > max_time:
            break
        score, current_best_move = _alphabeta(game, depth, float('-inf'), float('inf'), True, 'b')
        if current_best_move:
            best_move = current_best_move

    if best_move:
        game.move_piece(best_move[0], best_move[1])
        game.selected = None
