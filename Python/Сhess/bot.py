import time
from game import Game

PIECE_VALUES = {'P': 100, 'N': 320, 'B': 330, 'R': 500, 'Q': 900, 'K': 20000}

START_POSITIONS = {
    'wN': [(7, 1), (7, 6)],
    'bN': [(0, 1), (0, 6)],
    'wB': [(7, 2), (7, 5)],
    'bB': [(0, 2), (0, 5)],
    'wR': [(7, 0), (7, 7)],
    'bR': [(0, 0), (0, 7)],
    'wQ': [(7, 3)],
    'bQ': [(0, 3)],
}
CENTER_SQUARES = [(3, 3), (3, 4), (4, 3), (4, 4)]

class AlphaBetaBot:
    def __init__(self, max_depth=4, time_limit=1.2):
        self.max_depth = max_depth   
        self.time_limit = time_limit  
        self.ttable = {}
        self.visited_history = {}

    ### ШВИДКА ТАКТИКА ###
    def fast_tactics(self, game, color):
        score = 0
        opp_color = 'b' if color == 'w' else 'w'
        attacked_by_me = set()
        attacked_by_opp = set()

        # Всі можливі ходи свої
        for move in game.get_all_moves(color):
            _, end, _ = move
            attacked_by_me.add(end)

        # Всі можливі ходи суперника
        for move in game.get_all_moves(opp_color):
            _, end, _ = move
            attacked_by_opp.add(end)

        # Штраф за свої фігури під боєм
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq and sq[0] == color and (r, c) in attacked_by_opp:
                    score -= 50
                # Бонус за фігури, які атакують чужих
                if sq and sq[0] == opp_color and (r, c) in attacked_by_me:
                    score += 40

        # Проста "вилка": якщо одна фігура атакує >1 чужу фігуру одразу
        my_attack_map = {}
        for move in game.get_all_moves(color):
            start, end, _ = move
            my_attack_map.setdefault(start, set()).add(end)
        for startsq, attacked in my_attack_map.items():
            attacked_targets = 0
            for (r, c) in attacked:
                sq = game.board.board[r][c]
                if sq and sq[0] == opp_color and sq[1] in "NBRQ":
                    attacked_targets += 1
            if attacked_targets > 1:
                score += 80 * (attacked_targets - 1)
        return score

    ### АНТИ-ЦИКЛ + MOVE ORDERING ###
    def order_moves(self, moves, game, color):
        if len(game.move_log) > 0:
            last_move = game.move_log[-1]
            forbidden = (last_move[1], last_move[0])
            filtered = [m for m in moves if not (m[0] == forbidden[0] and m[1] == forbidden[1])]
            if filtered:
                moves = filtered

        def score(move):
            start, end, promo = move
            captured = game.board.board[end[0]][end[1]]
            s = 0
            if captured:
                s += 15 * PIECE_VALUES.get(captured[1], 0) - PIECE_VALUES.get(game.board.board[start[0]][start[1]][1], 0)
            if promo:
                s += 1500
            if self.gives_check(game, move, color):
                s += 900
            return s
        return sorted(moves, key=score, reverse=True)

    def gives_check(self, game, move, color):
        game_copy = game.copy()
        game_copy.move_piece(*move)
        return game_copy.is_in_check('b' if color == 'w' else 'w')

    ### ГОЛОВНА ОЦІНКА ПОЗИЦІЇ ###
    def evaluate_board(self, game: Game, color: str):
        score = 0
        opp_color = 'b' if color == 'w' else 'w'
        num_moves = len(game.move_log)

        # Матеріал
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq:
                    val = PIECE_VALUES[sq[1]]
                    sign = 1 if sq[0] == color else -1
                    score += val * sign

        # Контроль центру, розвиток, просунуті пішаки
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq and sq[0] == color:
                    if (r, c) in CENTER_SQUARES:
                        score += 45
                    if sq[1] == 'P':
                        if (r, c) in [(2,2), (2,3), (2,4), (2,5), (5,2), (5,3), (5,4), (5,5)]:
                            score += 25
                        if sq[0] == 'w':
                            score += (6 - r) * 8
                        else:
                            score += (r - 1) * 8

        mobility = len(game.get_all_moves(color))
        opp_mob = len(game.get_all_moves(opp_color))
        score += mobility * 20
        score -= opp_mob * 10

        undeveloped = 0
        for code, starts in START_POSITIONS.items():
            if code[1] in 'NBRQ' and code[0] == color:
                for pos in starts:
                    piece = game.board.board[pos[0]][pos[1]]
                    if piece == code:
                        undeveloped += 1
        score -= undeveloped * 70

        # Бонус за перехід фігури на нову клітинку
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq and sq[0] == color and sq[1] in 'NBRQK':
                    key = (sq, r, c)
                    if key not in self.visited_history:
                        score += 22
                        self.visited_history[key] = True

        pawns = [[0,0] for _ in range(8)]
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq and sq[1] == 'P':
                    idx = 0 if sq[0]=='w' else 1
                    pawns[c][idx] += 1
        for c in range(8):
            if pawns[c][0] > 1: score -= 32 * (pawns[c][0] - 1)
            if pawns[c][1] > 1: score += 32 * (pawns[c][1] - 1)
            if pawns[c][0] and (c==0 or not pawns[c-1][0]) and (c==7 or not pawns[c+1][0]): score -= 30
            if pawns[c][1] and (c==0 or not pawns[c-1][1]) and (c==7 or not pawns[c+1][1]): score += 30

        king_pos = None
        for r in range(8):
            for c in range(8):
                sq = game.board.board[r][c]
                if sq == color+'K':
                    king_pos = (r,c)
        if king_pos:
            safe = 0
            for dr in [-1,0,1]:
                for dc in [-1,0,1]:
                    rr, cc = king_pos[0]+dr, king_pos[1]+dc
                    if 0<=rr<8 and 0<=cc<8 and game.board.board[rr][cc]:
                        safe += 1
            if safe < 2:
                score -= 100

        # Жорсткий штраф за циклічні ходи
        if len(game.move_log) >= 2:
            last_move = game.move_log[-1]
            prev_move = game.move_log[-2]
            if last_move[0] == prev_move[1] and last_move[1] == prev_move[0]:
                score -= 900

        fen = game.board.fen()
        if hasattr(game, 'position_history') and game.position_history.count(fen) > 1:
            score -= 400 * (game.position_history.count(fen) - 1)

        # Додаємо ШВИДКУ тактичну оцінку
        score += self.fast_tactics(game, color)

        return score

    ### QUIESCENCE SEARCH ###
    def quiescence(self, game, alpha, beta, color, start_time):
        stand_pat = self.evaluate_board(game, color)
        if stand_pat >= beta:
            return beta
        if alpha < stand_pat:
            alpha = stand_pat
        for move in self.get_capture_moves(game, color):
            game_copy = game.copy()
            game_copy.move_piece(*move)
            score = -self.quiescence(game_copy, -beta, -alpha, 'b' if color == 'w' else 'w', start_time)
            if score >= beta:
                return beta
            if score > alpha:
                alpha = score
        return alpha

    def get_capture_moves(self, game, color):
        moves = game.get_all_moves(color)
        return [m for m in moves if game.board.board[m[1][0]][m[1][1]]]

    ### АЛЬФА-БЕТА ###
    def minimax(self, game: Game, depth: int, alpha: int, beta: int, maximizing_player: bool, color: str, start_time: float):
        fen = game.board.fen()
        tt_key = (fen, depth, maximizing_player)
        if tt_key in self.ttable:
            return self.ttable[tt_key]

        if depth == 0 or game.is_checkmate() or game.is_stalemate():
            val = self.evaluate_board(game, color)
            self.ttable[tt_key] = val
            return val

        if time.time() - start_time > self.time_limit:
            val = self.evaluate_board(game, color)
            self.ttable[tt_key] = val
            return val

        if maximizing_player:
            max_eval = -float('inf')
            for move in self.order_moves(game.get_all_moves(color), game, color):
                game_copy = game.copy()
                game_copy.move_piece(*move)
                eval = self.minimax(game_copy, depth - 1, alpha, beta, False, color, start_time)
                max_eval = max(max_eval, eval)
                alpha = max(alpha, eval)
                if beta <= alpha:
                    break
            self.ttable[tt_key] = max_eval
            return max_eval
        else:
            min_eval = float('inf')
            opp_color = 'b' if color == 'w' else 'w'
            for move in self.order_moves(game.get_all_moves(opp_color), game, opp_color):
                game_copy = game.copy()
                game_copy.move_piece(*move)
                eval = self.minimax(game_copy, depth - 1, alpha, beta, True, color, start_time)
                min_eval = min(min_eval, eval)
                beta = min(beta, eval)
                if beta <= alpha:
                    break
            self.ttable[tt_key] = min_eval
            return min_eval

    ### ІТЕРАТИВНЕ ПОГЛИБЛЕННЯ ###
    def choose_move(self, game: Game):
        best_move = None
        start = time.time()
        color = game.turn
        max_time = self.time_limit

        for depth in range(1, self.max_depth + 1):
            current_best = None
            current_value = -float('inf')
            for move in self.order_moves(game.get_all_moves(color), game, color):
                if time.time() - start > max_time:
                    break
                game_copy = game.copy()
                game_copy.move_piece(*move)
                board_value = self.minimax(game_copy, depth - 1, -float('inf'), float('inf'), False, color, start)
                if board_value > current_value:
                    current_value = board_value
                    current_best = move
            if time.time() - start > max_time:
                break
            if current_best:
                best_move = current_best
        return best_move

def bot_move(game: Game, max_depth=4, time_limit=1.2):
    bot = AlphaBetaBot(max_depth=max_depth, time_limit=time_limit)
    move = bot.choose_move(game)
    if move:
        game.selected = None
    return move
