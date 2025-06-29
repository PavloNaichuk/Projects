import time
from game import Game

PIECE_VALUES = {'P': 100, 'N': 320, 'B': 330, 'R': 500, 'Q': 900, 'K': 20000}

class AlphaBetaBot:
    def __init__(self, max_depth=4, time_limit=2.0):
        self.max_depth = max_depth   
        self.time_limit = time_limit    

    def evaluate_board(self, game: Game, color: str):
        score = 0
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq:
                    val = PIECE_VALUES[sq[1]]
                    score += val if sq[0] == color else -val
        return score

    def minimax(self, game: Game, depth: int, alpha: int, beta: int, maximizing_player: bool, color: str, start_time: float):
        if depth == 0 or game.is_checkmate() or game.is_stalemate():
            return self.evaluate_board(game, color)
        if time.time() - start_time > self.time_limit:
            return self.evaluate_board(game, color)
        if maximizing_player:
            max_eval = -float('inf')
            for move in game.get_all_moves(color):
                game_copy = game.copy()
                game_copy.move_piece(*move)
                eval = self.minimax(game_copy, depth - 1, alpha, beta, False, color, start_time)
                max_eval = max(max_eval, eval)
                alpha = max(alpha, eval)
                if beta <= alpha:
                    break
            return max_eval
        else:
            min_eval = float('inf')
            opp_color = 'b' if color == 'w' else 'w'
            for move in game.get_all_moves(opp_color):
                game_copy = game.copy()
                game_copy.move_piece(*move)
                eval = self.minimax(game_copy, depth - 1, alpha, beta, True, color, start_time)
                min_eval = min(min_eval, eval)
                beta = min(beta, eval)
                if beta <= alpha:
                    break
            return min_eval

    def choose_move(self, game: Game):
        best_move = None
        best_value = -float('inf')
        start = time.time()
        for move in game.get_all_moves(game.turn):
            if time.time() - start > self.time_limit:
                break
            game_copy = game.copy()
            game_copy.move_piece(*move)
            board_value = self.minimax(game_copy, self.max_depth - 1, -float('inf'), float('inf'), False, game.turn, start)
            if board_value > best_value:
                best_value = board_value
                best_move = move
        return best_move


def bot_move(game: Game, max_depth=4, time_limit=2.0):
    bot = AlphaBetaBot(max_depth=max_depth, time_limit=time_limit)
    move = bot.choose_move(game)
    if move:
        game.selected = None
    return move
