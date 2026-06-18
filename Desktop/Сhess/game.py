import json
import time
from copy import deepcopy

from board import Board


class Game:
    def __init__(self):
        self.board = Board()
        self.turn = self.board.turn
        self.selected = None
        self.move_log = []
        self.position_history = [self._position_key()]
        self.game_over = False
        self.game_over_message = ""

        self.time_control = (300, 2)  # seconds, increment
        self.time_remaining = {
            'w': self.time_control[0],
            'b': self.time_control[0],
        }
        self.move_start_time = time.time()

    def _position_key(self):
        if hasattr(self.board, 'position_key'):
            return self.board.position_key()
        return self.board.fen()

    def _sync_turn_from_board(self):
        self.turn = self.board.turn

    def get_board_key(self):
        return (tuple(tuple(row) for row in self.board.board), self.turn)

    def get_time_left(self, color):
        remaining = self.time_remaining[color]
        if self.turn == color and not self.game_over:
            remaining -= time.time() - self.move_start_time
        return max(0, remaining)

    def _commit_clock_after_move(self, moved_color):
        now = time.time()
        elapsed = now - self.move_start_time
        base_time, increment = self.time_control

        self.time_remaining[moved_color] = max(
            0,
            self.time_remaining[moved_color] - elapsed + increment,
        )
        self.move_start_time = now

    def switch_turn(self):
        self.turn = 'b' if self.turn == 'w' else 'w'
        self.board.turn = self.turn
        self.move_start_time = time.time()

    def handle_click(self, pos):
        if self.game_over:
            return False

        r, c = pos

        if self.selected is None:
            piece = self.board[r][c]
            if piece and piece[0] == self.turn:
                self.selected = (r, c)
            return False

        moved = self.move_piece(self.selected, (r, c))
        self.selected = None
        return moved

    def _normalize_promotion(self, piece, end, promotion):
        if promotion is not None:
            promotion = promotion.upper()
            if promotion not in {'Q', 'R', 'B', 'N'}:
                return None, False
            return promotion, True

        er, _ = end
        if piece and piece[1] == 'P' and er in (0, 7):
            return 'Q', True

        return None, True

    def _find_legal_move(self, start, end, promotion):
        for move in self.board.legal_moves():
            move_start, move_end, move_promotion = move
            if move_start == start and move_end == end and move_promotion == promotion:
                return move
        return None

    def _captured_piece_for_log(self, start, end):
        sr, sc = start
        er, ec = end
        piece = self.board.board[sr][sc]
        captured = self.board.board[er][ec]

        # En passant capture: the target square is empty, but a pawn is captured behind it.
        if (
            piece
            and piece[1] == 'P'
            and sc != ec
            and captured is None
            and self.board.ep_square == (er, ec)
        ):
            captured_row = er + 1 if piece[0] == 'w' else er - 1
            captured = self.board.board[captured_row][ec]

        return captured

    def move_piece(self, start, end, promotion=None):
        if self.game_over:
            return False

        sr, sc = start
        er, ec = end

        if not (0 <= sr < 8 and 0 <= sc < 8 and 0 <= er < 8 and 0 <= ec < 8):
            return False

        piece = self.board.board[sr][sc]
        if piece is None or piece[0] != self.turn:
            return False

        promotion, is_valid_promotion = self._normalize_promotion(piece, end, promotion)
        if not is_valid_promotion:
            return False

        legal_move = self._find_legal_move(start, end, promotion)
        if legal_move is None:
            return False

        moved_color = self.turn
        captured = self._captured_piece_for_log(start, end)

        self.board.push(legal_move)
        self._sync_turn_from_board()
        self._commit_clock_after_move(moved_color)

        self.move_log.append((start, end, piece, captured, promotion))
        self.position_history.append(self._position_key())

        is_over, message = check_game_result(self.board)
        if is_over:
            self.game_over = True
            self.game_over_message = message

        return True

    def undo_move(self):
        if not self.board.move_stack:
            return False

        self.board.pop()
        self._sync_turn_from_board()

        if self.move_log:
            self.move_log.pop()
        if len(self.position_history) > 1:
            self.position_history.pop()

        self.selected = None
        self.game_over = False
        self.game_over_message = ""
        self.move_start_time = time.time()
        return True

    def get_legal_moves(self, start):
        return [
            move[1]
            for move in self.board.legal_moves()
            if move[0] == start
        ]

    def get_legal_moves_with_promotion(self, start):
        return [
            move
            for move in self.board.legal_moves()
            if move[0] == start
        ]

    def generate_pseudo_legal_moves(self, color=None):
        if color is None:
            color = self.turn

        old_turn = self.board.turn
        self.board.turn = color
        try:
            return list(self.board.generate_pseudo_legal_moves())
        finally:
            self.board.turn = old_turn

    def generate_legal_moves(self, color=None):
        return self.get_all_moves(color or self.turn)

    def get_all_moves(self, color=None):
        if color is None:
            color = self.turn

        old_turn = self.board.turn
        self.board.turn = color
        try:
            return list(self.board.legal_moves())
        finally:
            self.board.turn = old_turn

    def is_in_check(self, color=None):
        return self.board.is_check(color or self.turn)

    def is_checkmate(self):
        return self.board.is_checkmate()

    def is_stalemate(self):
        return self.board.is_stalemate()

    def is_draw(self):
        return self.board.is_draw()

    def _to_json_data(self):
        return {
            'board': self.board.board,
            'board_turn': self.board.turn,
            'turn': self.turn,
            'castling_rights': self.board.castling_rights,
            'ep_square': self.board.ep_square,
            'halfmove_clock': self.board.halfmove_clock,
            'fullmove_number': self.board.fullmove_number,
            'board_history': self.board.history,
            'move_log': self.move_log,
            'position_history': self.position_history,
            'time_control': self.time_control,
            'time_remaining': self.time_remaining,
            'game_over': self.game_over,
            'game_over_message': self.game_over_message,
        }

    def save_game(self, filename="autosave.json"):
        with open(filename, "w", encoding="utf-8") as file:
            json.dump(self._to_json_data(), file, ensure_ascii=False, indent=2)

    def load_game(self, filename):
        with open(filename, "r", encoding="utf-8") as file:
            data = json.load(file)

        self.board = Board()
        self.board.board = data['board']
        self.board.turn = data.get('board_turn', data.get('turn', 'w'))
        self.board.castling_rights = data.get(
            'castling_rights',
            {'w': {'K': True, 'Q': True}, 'b': {'K': True, 'Q': True}},
        )
        ep_square = data.get('ep_square')
        self.board.ep_square = tuple(ep_square) if ep_square else None
        self.board.halfmove_clock = data.get('halfmove_clock', 0)
        self.board.fullmove_number = data.get('fullmove_number', 1)
        self.board.history = data.get('board_history', [self._position_key()])
        self.board.move_stack = []

        self.turn = data.get('turn', self.board.turn)
        self.board.turn = self.turn
        self.selected = None
        self.move_log = data.get('move_log', [])
        self.position_history = data.get('position_history', [self._position_key()])
        self.time_control = tuple(data.get('time_control', (300, 2)))
        self.time_remaining = data.get(
            'time_remaining',
            {'w': self.time_control[0], 'b': self.time_control[0]},
        )
        self.game_over = data.get('game_over', False)
        self.game_over_message = data.get('game_over_message', "")
        self.move_start_time = time.time()

    def copy(self):
        new_game = Game()
        new_game.board = self.board.copy()
        new_game.turn = self.turn
        new_game.selected = self.selected
        new_game.move_log = deepcopy(self.move_log)
        new_game.position_history = list(self.position_history)
        new_game.time_control = tuple(self.time_control)
        new_game.time_remaining = dict(self.time_remaining)
        new_game.move_start_time = self.move_start_time
        new_game.game_over = self.game_over
        new_game.game_over_message = self.game_over_message
        return new_game


def check_game_result(board):
    if board.is_checkmate():
        winner = 'Black' if board.turn == 'w' else 'White'
        return True, f"Checkmate! {winner} wins."
    if board.is_stalemate():
        return True, "Stalemate — draw!"
    if board.is_threefold_repetition():
        return True, "Draw: threefold repetition!"
    if board.is_fifty_move_rule():
        return True, "Draw: 50-move rule (no capture or pawn move)!"
    if board.is_insufficient_material():
        return True, "Draw: insufficient material for mate!"
    return False, ""
