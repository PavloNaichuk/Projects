
def evaluate_move(game, start, end):
    target = game.board[end[0]][end[1]]
    values = {'P': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 100}
    if target == '':
        return 0
    return values.get(target[1], 0)

def bot_move(game):
    best_score = -1
    best_move = None
    for r in range(8):
        for c in range(8):
            piece = game.board[r][c]
            if piece != '' and piece[0] == 'b':
                moves = game.get_valid_moves((r, c))
                for move in moves:
                    score = evaluate_move(game, (r, c), move)
                    if score > best_score:
                        best_score = score
                        best_move = ((r, c), move)
    if best_move:
        game.move_piece(*best_move)
        game.selected = None
    else:
        # fallback if no good move found
        for r in range(8):
            for c in range(8):
                piece = game.board[r][c]
                if piece != '' and piece[0] == 'b':
                    moves = game.get_valid_moves((r, c))
                    if moves:
                        game.move_piece((r, c), moves[0])
                        game.selected = None
                        return
