import pickle
import copy
import time
from board import Board
class Game:
    def __init__(self):
        self.board = Board()
        self.turn = 'w'
        self.selected = None
        self.move_log = []
        self.position_history = [self.get_board_key()]
        self.halfmove_clock = 0
        self.has_moved = {
            (7, 4): False,  
            (7, 0): False, 
            (7, 7): False,  
            (0, 4): False, 
            (0, 0): False, 
            (0, 7): False,  
        }
        self.time_control    = (300, 2)
        self.time_remaining  = {'w': self.time_control[0], 'b': self.time_control[0]}
        self.move_start_time = time.time()

    def get_board_key(self):
        return (tuple(tuple(row) for row in self.board.board), self.turn)

    def get_time_left(self, color):
        rem = self.time_remaining[color]
        if self.turn == color:
            rem -= (time.time() - self.move_start_time)
        return max(0, rem)
    
    def switch_turn(self):
        self.turn = 'b' if self.turn == 'w' else 'w'

    def handle_click(self, pos):
        r, c = pos
        if self.selected is None:
            piece = self.board[r][c]
            if piece and piece[0] == self.turn:
                self.selected = (r, c)
        else:
            if (r, c) in self.get_legal_moves(self.selected):
                self.move_piece(self.selected, (r, c))
            self.selected = None

    def move_piece(self, start, end, promotion=None):
        sr, sc = start
        er, ec = end
        piece    = self.board.board[sr][sc]
        captured = self.board.board[er][ec]
        move = (start, end, promotion)
        self.board.push(move)
        self.move_log.append((start, end, piece, captured))
        self.position_history.append(self.board.fen())
        self.turn = 'b' if self.turn == 'w' else 'w'
    
    def get_legal_moves(self, start):
        return [
            move[1]
            for move in self.board.legal_moves()
            if move[0] == start
        ]  
    def get_all_moves(self, color):
        return list(self.board.legal_moves())

    def is_in_check(self, color):
        return self.board.is_check(color)

    def is_checkmate(self):
        return self.board.is_checkmate()

    def is_stalemate(self):
        return self.board.is_stalemate()

    def is_draw(self):
        return self.board.is_draw()
    
    def save_game(self, filename="savegame.pkl"):
        with open(filename, "wb") as f:
            pickle.dump({
                'board': self.board,
                'turn': self.turn,
                'move_log': self.move_log,
                'position_history': self.position_history,
                'halfmove_clock': self.halfmove_clock
            }, f)

    def load_game(self, filename="savegame.pkl"):
        with open(filename, "rb") as f:
            data = pickle.load(f)
        self.board = data['board']
        self.turn = data['turn']
        self.move_log = data['move_log']
        self.position_history = data['position_history']
        self.halfmove_clock = data['halfmove_clock']
        self.selected = None
