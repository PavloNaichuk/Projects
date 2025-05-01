
import copy

PIECE_VALUES = {'P': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 1000}

def evaluate_board(board):
    score = 0
    for row in board:
        for piece in row:
            if piece != '':
                value = PIECE_VALUES.get(piece[1], 0)
                score += value if piece[0] == 'b' else -value
    return score

def get_all_legal_moves(game, color):
    legal_moves = []
    for r in range(8):
        for c in range(8):
            piece = game.board[r][c]
            if piece != '' and piece[0] == color:
                start = (r, c)
                for end in game.get_valid_moves(start):
                    backup = copy.deepcopy(game.board)
                    game.move_piece(start, end)
                    if not game.is_in_check(color):
                        legal_moves.append((start, end))
                    game.board = backup
    return legal_moves

def minimax(game, depth, maximizing_player):
    color = 'b' if maximizing_player else 'w'
    moves = get_all_legal_moves(game, color)

    if depth == 0 or not moves:
        return evaluate_board(game.board), None

    best_score = float('-inf') if maximizing_player else float('inf')
    best_move = None

    for move in moves:
        start, end = move
        backup = copy.deepcopy(game.board)
        game.move_piece(start, end)

        score, _ = minimax(game, depth - 1, not maximizing_player)

        game.board = backup

        if maximizing_player:
            if score > best_score:
                best_score = score
                best_move = move
        else:
            if score < best_score:
                best_score = score
                best_move = move

    return best_score, best_move

def bot_move(game):
    _, best_move = minimax(game, 2, True)
    if best_move:
        game.move_piece(*best_move)
        game.selected = None
