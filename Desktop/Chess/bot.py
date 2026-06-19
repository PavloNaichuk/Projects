import time

from game import Game

PIECE_VALUES = {
    'P': 100,
    'N': 320,
    'B': 330,
    'R': 500,
    'Q': 900,
    'K': 20000,
}

CHECKMATE_SCORE = 1_000_000

CENTER_SQUARES = {
    (3, 3), (3, 4),
    (4, 3), (4, 4),
}

EXTENDED_CENTER = {
    (2, 2), (2, 3), (2, 4), (2, 5),
    (3, 2), (3, 3), (3, 4), (3, 5),
    (4, 2), (4, 3), (4, 4), (4, 5),
    (5, 2), (5, 3), (5, 4), (5, 5),
}

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


class AlphaBetaBot:
    def __init__(self, max_depth=4, time_limit=1.2):
        self.max_depth = max_depth
        self.time_limit = time_limit
        self.ttable = {}
        self.nodes = 0

    @staticmethod
    def opponent(color):
        return 'b' if color == 'w' else 'w'

    def choose_move(self, game: Game):
        start_time = time.time()
        root_color = game.turn
        legal_moves = game.get_all_moves(root_color)

        if not legal_moves:
            return None

        best_move = None
        best_score = -float('inf')

        # Iterative deepening: if time runs out, we still have the best move
        # from a completed shallower depth.
        for depth in range(1, self.max_depth + 1):
            if self._time_is_up(start_time):
                break

            depth_best_move = None
            depth_best_score = -float('inf')

            ordered_moves = self.order_moves(legal_moves, game, root_color)

            for move in ordered_moves:
                if self._time_is_up(start_time):
                    break

                game_copy = game.copy()
                if not game_copy.move_piece(*move):
                    continue

                score = self.minimax(
                    game_copy,
                    depth=depth - 1,
                    alpha=-float('inf'),
                    beta=float('inf'),
                    root_color=root_color,
                    start_time=start_time,
                )

                if score > depth_best_score:
                    depth_best_score = score
                    depth_best_move = move

            if depth_best_move is not None:
                best_move = depth_best_move
                best_score = depth_best_score

                # No need to search deeper if we found a forced mate.
                if best_score >= CHECKMATE_SCORE - 1000:
                    break

        return best_move

    def minimax(self, game: Game, depth: int, alpha: float, beta: float, root_color: str, start_time: float):
        self.nodes += 1

        if self._time_is_up(start_time):
            return self.evaluate_board(game, root_color)

        current_color = game.turn
        fen = game.board.fen()
        tt_key = (fen, depth, current_color, root_color)

        if tt_key in self.ttable:
            return self.ttable[tt_key]

        terminal = self.terminal_score(game, current_color, root_color, depth)
        if terminal is not None:
            self.ttable[tt_key] = terminal
            return terminal

        if depth == 0:
            score = self.quiescence(game, alpha, beta, root_color, start_time)
            self.ttable[tt_key] = score
            return score

        legal_moves = game.get_all_moves(current_color)
        if not legal_moves:
            score = self.evaluate_board(game, root_color)
            self.ttable[tt_key] = score
            return score

        if current_color == root_color:
            value = -float('inf')

            for move in self.order_moves(legal_moves, game, current_color):
                if self._time_is_up(start_time):
                    break

                game_copy = game.copy()
                if not game_copy.move_piece(*move):
                    continue

                value = max(
                    value,
                    self.minimax(game_copy, depth - 1, alpha, beta, root_color, start_time),
                )
                alpha = max(alpha, value)

                if alpha >= beta:
                    break
        else:
            value = float('inf')

            for move in self.order_moves(legal_moves, game, current_color):
                if self._time_is_up(start_time):
                    break

                game_copy = game.copy()
                if not game_copy.move_piece(*move):
                    continue

                value = min(
                    value,
                    self.minimax(game_copy, depth - 1, alpha, beta, root_color, start_time),
                )
                beta = min(beta, value)

                if alpha >= beta:
                    break

        self.ttable[tt_key] = value
        return value

    def quiescence(self, game: Game, alpha: float, beta: float, root_color: str, start_time: float):
        stand_pat = self.evaluate_board(game, root_color)

        if stand_pat >= beta:
            return beta

        if alpha < stand_pat:
            alpha = stand_pat

        current_color = game.turn
        capture_moves = self.get_capture_moves(game, current_color)
        capture_moves = self.order_moves(capture_moves, game, current_color)

        for move in capture_moves:
            if self._time_is_up(start_time):
                break

            game_copy = game.copy()
            if not game_copy.move_piece(*move):
                continue

            score = self.quiescence(game_copy, alpha, beta, root_color, start_time)

            if current_color == root_color:
                if score > alpha:
                    alpha = score
                if alpha >= beta:
                    break
            else:
                if score < beta:
                    beta = score
                if alpha >= beta:
                    break

        return alpha if current_color == root_color else beta

    def terminal_score(self, game: Game, current_color: str, root_color: str, depth: int):
        if game.board.is_checkmate(current_color):
            if current_color == root_color:
                return -CHECKMATE_SCORE - depth
            return CHECKMATE_SCORE + depth

        if game.board.is_draw(current_color):
            return 0

        return None

    def evaluate_board(self, game: Game, color: str):
        opponent = self.opponent(color)
        score = 0

        score += self.material_score(game, color)
        score -= self.material_score(game, opponent)

        score += self.piece_activity_score(game, color)
        score -= self.piece_activity_score(game, opponent)

        score += self.mobility_score(game, color)
        score -= self.mobility_score(game, opponent)

        score += self.pawn_structure_score(game, color)
        score -= self.pawn_structure_score(game, opponent)

        score += self.king_safety_score(game, color)
        score -= self.king_safety_score(game, opponent)

        score += self.rook_open_file_score(game, color)
        score -= self.rook_open_file_score(game, opponent)

        score += self.passed_pawn_bonus(game, color)
        score -= self.passed_pawn_bonus(game, opponent)

        score += self.castling_status_score(game, color)
        score -= self.castling_status_score(game, opponent)

        score += self.hanging_piece_penalty(game, color)
        score -= self.hanging_piece_penalty(game, opponent)

        score += self.repetition_penalty(game)

        return score

    def material_score(self, game: Game, color: str):
        score = 0

        for row in game.board.board:
            for piece in row:
                if piece and piece[0] == color:
                    score += PIECE_VALUES[piece[1]]

        return score

    def piece_activity_score(self, game: Game, color: str):
        score = 0

        for r in range(8):
            for c in range(8):
                piece = game.board.board[r][c]
                if not piece or piece[0] != color:
                    continue

                piece_type = piece[1]

                if (r, c) in CENTER_SQUARES:
                    score += 35
                elif (r, c) in EXTENDED_CENTER:
                    score += 12

                if piece_type == 'P':
                    if color == 'w':
                        score += (6 - r) * 8
                    else:
                        score += (r - 1) * 8

                if piece_type == 'N' and (r in (0, 7) or c in (0, 7)):
                    score -= 15

        # Opening development: discourage leaving minor pieces and queen on home squares.
        for code, starts in START_POSITIONS.items():
            if code[0] != color:
                continue

            for r, c in starts:
                if game.board.board[r][c] == code:
                    if code[1] in {'N', 'B'}:
                        score -= 35
                    elif code[1] == 'Q':
                        score -= 10

        return score

    def mobility_score(self, game: Game, color: str):
        return len(game.get_all_moves(color)) * 8

    def pawn_structure_score(self, game: Game, color: str):
        score = 0
        pawns_by_file = [[] for _ in range(8)]

        for r in range(8):
            for c in range(8):
                piece = game.board.board[r][c]
                if piece == color + 'P':
                    pawns_by_file[c].append((r, c))

        for file_index, pawns in enumerate(pawns_by_file):
            if len(pawns) > 1:
                score -= 25 * (len(pawns) - 1)

            if pawns:
                has_left_neighbor = file_index > 0 and bool(pawns_by_file[file_index - 1])
                has_right_neighbor = file_index < 7 and bool(pawns_by_file[file_index + 1])

                if not has_left_neighbor and not has_right_neighbor:
                    score -= 20

        return score

    def king_safety_score(self, game: Game, color: str):
        king_pos = self.find_piece(game, color + 'K')
        if king_pos is None:
            return -500

        score = 0
        kr, kc = king_pos
        own_pawns_near_king = 0

        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue

                rr, cc = kr + dr, kc + dc
                if 0 <= rr < 8 and 0 <= cc < 8:
                    if game.board.board[rr][cc] == color + 'P':
                        own_pawns_near_king += 1

        score += own_pawns_near_king * 12

        enemy = self.opponent(color)
        enemy_attacks = game.board.attack_map(enemy)
        if king_pos in enemy_attacks:
            score -= 120

        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                rr, cc = kr + dr, kc + dc
                if 0 <= rr < 8 and 0 <= cc < 8 and (rr, cc) in enemy_attacks:
                    score -= 10

        return score

    def rook_open_file_score(self, game: Game, color: str):
        score = 0

        for r in range(8):
            for c in range(8):
                piece = game.board.board[r][c]
                if piece != color + 'R':
                    continue

                file_pieces = [game.board.board[row][c] for row in range(8)]
                own_pawn = any(p == color + 'P' for p in file_pieces)
                enemy_pawn = any(p and p[0] != color and p[1] == 'P' for p in file_pieces)

                if not own_pawn and not enemy_pawn:
                    score += 35
                elif not own_pawn:
                    score += 18

        return score

    def passed_pawn_bonus(self, game: Game, color: str):
        score = 0
        enemy = self.opponent(color)
        direction = -1 if color == 'w' else 1

        for r in range(8):
            for c in range(8):
                piece = game.board.board[r][c]
                if piece != color + 'P':
                    continue

                is_passed = True
                rr = r + direction

                while 0 <= rr < 8:
                    for dc in (-1, 0, 1):
                        cc = c + dc
                        if 0 <= cc < 8 and game.board.board[rr][cc] == enemy + 'P':
                            is_passed = False
                            break
                    if not is_passed:
                        break
                    rr += direction

                if is_passed:
                    advancement = (6 - r) if color == 'w' else (r - 1)
                    score += 35 + max(0, advancement) * 12

        return score

    def castling_status_score(self, game: Game, color: str):
        king_pos = self.find_piece(game, color + 'K')
        if king_pos is None:
            return 0

        if color == 'w':
            if king_pos in {(7, 6), (7, 2)}:
                return 35
        else:
            if king_pos in {(0, 6), (0, 2)}:
                return 35

        if game.board.fullmove_number > 10:
            return -25

        return 0

    def hanging_piece_penalty(self, game: Game, color: str):
        enemy = self.opponent(color)
        enemy_attacks = game.board.attack_map(enemy)
        own_attacks = game.board.attack_map(color)
        penalty = 0

        for r in range(8):
            for c in range(8):
                piece = game.board.board[r][c]
                if not piece or piece[0] != color or piece[1] == 'K':
                    continue

                square = (r, c)
                if square in enemy_attacks and square not in own_attacks:
                    penalty -= PIECE_VALUES[piece[1]] // 10

        return penalty

    def repetition_penalty(self, game: Game):
        if not hasattr(game, 'position_history') or not game.position_history:
            return 0

        key = game.position_history[-1]
        repeats = game.position_history.count(key)

        if repeats <= 1:
            return 0

        return -120 * (repeats - 1)

    def order_moves(self, moves, game: Game, color: str):
        def move_score(move):
            start, end, promotion = move
            moving_piece = game.board.board[start[0]][start[1]]
            captured_piece = self.captured_piece_for_move(game, move)

            score = 0

            if captured_piece and moving_piece:
                # MVV-LVA: Most Valuable Victim, Least Valuable Attacker.
                score += (
                    10 * PIECE_VALUES.get(captured_piece[1], 0)
                    - PIECE_VALUES.get(moving_piece[1], 0)
                )

            if promotion:
                score += PIECE_VALUES.get(promotion, 0) + 800

            if self.gives_check(game, move, color):
                score += 500

            if end in CENTER_SQUARES:
                score += 20

            return score

        return sorted(moves, key=move_score, reverse=True)

    def get_capture_moves(self, game: Game, color: str):
        return [
            move
            for move in game.get_all_moves(color)
            if self.captured_piece_for_move(game, move) is not None
        ]

    def captured_piece_for_move(self, game: Game, move):
        start, end, _promotion = move
        sr, sc = start
        er, ec = end

        captured = game.board.board[er][ec]
        moving_piece = game.board.board[sr][sc]

        # En passant capture.
        if (
            moving_piece
            and moving_piece[1] == 'P'
            and captured is None
            and sc != ec
            and game.board.ep_square == (er, ec)
        ):
            captured_row = er + 1 if moving_piece[0] == 'w' else er - 1
            captured = game.board.board[captured_row][ec]

        return captured

    def gives_check(self, game: Game, move, color: str):
        game_copy = game.copy()
        if not game_copy.move_piece(*move):
            return False

        return game_copy.is_in_check(self.opponent(color))

    def find_piece(self, game: Game, piece_code: str):
        for r in range(8):
            for c in range(8):
                if game.board.board[r][c] == piece_code:
                    return r, c
        return None

    def _time_is_up(self, start_time: float):
        return time.time() - start_time >= self.time_limit


def bot_move(game: Game, max_depth=4, time_limit=1.2):
    bot = AlphaBetaBot(max_depth=max_depth, time_limit=time_limit)
    move = bot.choose_move(game)

    if move:
        game.selected = None

    return move
