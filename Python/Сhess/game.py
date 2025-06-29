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
        piece = self.board.board[sr][sc]
        captured = self.board.board[er][ec]

        is_pawn = piece and piece[1] == 'P'
        reached_last = (piece and is_pawn and ((piece[0] == 'w' and er == 0) or (piece[0] == 'b' and er == 7)))
        if reached_last and not promotion:
            promotion = 'Q'

        move = (start, end, promotion) 
        self.board.push(move)
        self.move_log.append((start, end, piece, captured))
        self.position_history.append(self.board.fen())
        self.turn = 'b' if self.turn == 'w' else 'w'
        is_over, msg = check_game_result(self.board)
        if is_over:
            print(msg)
            self.game_over = True
            self.game_over_message = msg
    
    def undo_move(self):
        self.board.pop()
        self.turn = 'b' if self.turn == 'w' else 'w'
        if self.move_log:
            self.move_log.pop()
        if self.position_history:
            self.position_history.pop()
    
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
        
    def copy(self):
        new_game = Game()
        new_game.board = self.board.copy()    
        new_game.turn = self.turn
        new_game.selected = self.selected
        new_game.move_log = list(self.move_log)
        new_game.position_history = list(self.position_history)
        new_game.halfmove_clock = self.halfmove_clock
        new_game.has_moved = dict(self.has_moved)
        new_game.time_control = self.time_control
        new_game.time_remaining = dict(self.time_remaining)
        new_game.move_start_time = self.move_start_time
        return new_game
  
def check_game_result(board):
    if board.is_checkmate():
        winner = 'Black' if board.turn == 'w' else 'White'
        return True, f"Checkmate! {winner} wins."
    elif board.is_stalemate():
        return True, "Stalemate — draw!"
    elif board.is_threefold_repetition():
        return True, "Draw: threefold repetition!"
    elif board.is_fifty_move_rule():
        return True, "Draw: 50-move rule (no capture or pawn move)!"
    elif board.is_insufficient_material():
        return True, "Draw: insufficient material for mate!"
    else:
        return False, ""