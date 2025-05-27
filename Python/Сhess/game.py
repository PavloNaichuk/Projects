import pickle
import copy

class Game:
    def __init__(self):
        self.board = self.init_board()
        self.turn = 'w'
        self.selected = None
        self.move_log = []
        self.position_history = [self.get_board_key()]
        self.halfmove_clock = 0

    def init_board(self):
        return [
            ['bR','bN','bB','bQ','bK','bB','bN','bR'],
            ['bP']*8,
            ['']*8,
            ['']*8,
            ['']*8,
            ['']*8,
            ['wP']*8,
            ['wR','wN','wB','wQ','wK','wB','wN','wR']
        ]

    def get_board_key(self):
        return (tuple(tuple(row) for row in self.board), self.turn)

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
            
    def move_piece(self, start, end):
        sr, sc = start
        er, ec = end
        piece = self.board[sr][sc]
        captured = self.board[er][ec]

        self.move_log.append((start, end, piece, captured))
        self.board[er][ec] = piece
        self.board[sr][sc] = ''

        if piece[1] == 'P' or captured:
            self.halfmove_clock = 0
        else:
            self.halfmove_clock += 1

        self.switch_turn()
        self.position_history.append(self.get_board_key())

    def undo_move(self):
        if not self.move_log:
            return
        start, end, piece, captured = self.move_log.pop()
        sr, sc = start
        er, ec = end
        self.board[sr][sc] = piece
        self.board[er][ec] = captured
        self.switch_turn()
        if len(self.position_history) > 1:
            self.position_history.pop()
        self.halfmove_clock = 0

    def get_valid_moves(self, pos):
        r, c = pos
        piece = self.board[r][c]
        if not piece:
            return []
        color, p = piece[0], piece[1]
        moves, directions = [], []

        if p == 'P':
            d = -1 if color == 'w' else 1
            start_row = 6 if color == 'w' else 1
            if 0 <= r+d < 8 and self.board[r+d][c] == '':
                moves.append((r+d, c))
                if r == start_row and self.board[r+2*d][c] == '':
                    moves.append((r+2*d, c))
            for dc in (-1, 1):
                nr, nc = r+d, c+dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    t = self.board[nr][nc]
                    if t and t[0] != color and t[1] != 'K':
                        moves.append((nr, nc))

        elif p == 'N':
            for dr, dc in [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]:
                nr, nc = r+dr, c+dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    t = self.board[nr][nc]
                    if not t or (t[0] != color and t[1] != 'K'):
                        moves.append((nr, nc))

        elif p == 'K':
            for dr in (-1, 0, 1):
                for dc in (-1, 0, 1):
                    if dr == 0 and dc == 0:
                        continue
                    nr, nc = r+dr, c+dc
                    if 0 <= nr < 8 and 0 <= nc < 8:
                        t = self.board[nr][nc]
                        if not t or (t[0] != color and t[1] != 'K'):
                            moves.append((nr, nc))

        else:
            if p == 'R':
                directions = [(-1,0),(1,0),(0,-1),(0,1)]
            elif p == 'B':
                directions = [(-1,-1),(-1,1),(1,-1),(1,1)]
            elif p == 'Q':
                directions = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]
            for dr, dc in directions:
                nr, nc = r+dr, c+dc
                while 0 <= nr < 8 and 0 <= nc < 8:
                    t = self.board[nr][nc]
                    if not t:
                        moves.append((nr, nc))
                    else:
                        if t[0] != color and t[1] != 'K':
                            moves.append((nr, nc))
                        break
                    nr += dr; nc += dc

        return moves

    def get_legal_moves(self, pos):
        color = self.board[pos[0]][pos[1]][0]
        legal = []
        for dest in self.get_valid_moves(pos):
            self.move_piece(pos, dest)
            in_check = self.is_in_check(color)
            self.undo_move()
            if not in_check:
                legal.append(dest)
        return legal
    
    def get_all_moves(self, color):
        moves = []
        for r in range(8):
            for c in range(8):
                p = self.board[r][c]
                if p and p[0] == color:
                    for dest in self.get_legal_moves((r, c)):
                        moves.append(((r, c), dest))
        return moves

    def is_in_check(self, color):
        king = f"{color}K"
        kr = kc = None
        for r in range(8):
            for c in range(8):
                if self.board[r][c] == king:
                    kr, kc = r, c
                    break
            if kr is not None:
                break
        if kr is None:
            return False
        opp = 'b' if color == 'w' else 'w'
        pawn_dirs = [(-1,-1),(-1,1)] if opp == 'w' else [(1,-1),(1,1)]
        for dr, dc in pawn_dirs:
            rr, cc = kr+dr, kc+dc
            if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == f"{opp}P":
                return True
        knight_moves = [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]
        for dr, dc in knight_moves:
            rr, cc = kr+dr, kc+dc
            if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == f"{opp}N":
                return True
        directions = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]
        for dr, dc in directions:
            rr, cc = kr+dr, kc+dc
            while 0 <= rr < 8 and 0 <= cc < 8:
                t = self.board[rr][cc]
                if t:
                    if t[0] == opp:
                        ttype = t[1]
                        if (dr == 0 or dc == 0) and ttype in ('R','Q'):
                            return True
                        if (dr != 0 and dc != 0) and ttype in ('B','Q'):
                            return True
                    break
                rr += dr; cc += dc
        for dr in (-1,0,1):
            for dc in (-1,0,1):
                if dr == 0 and dc == 0: continue
                rr, cc = kr+dr, kc+dc
                if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == f"{opp}K":
                    return True
        return False

    def has_legal_moves(self, color):
        for move in self.get_all_moves(color):
            newg = copy.deepcopy(self)
            newg.move_piece(move[0], move[1])
            if not newg.is_in_check(color):
                return True
        return False

    def is_checkmate(self):
        return self.is_in_check(self.turn) and not self.has_legal_moves(self.turn)

    def is_stalemate(self):
        return not self.is_in_check(self.turn) and not self.has_legal_moves(self.turn)

    def is_insufficient_material(self):
        pieces = [p for row in self.board for p in row if p]
        minors = [p for p in pieces if p[1] in ('N','B')]
        others = [p for p in pieces if p[1] not in ('K','N','B')]
        return not others and len(minors) <= 1

    def is_fifty_move_rule(self):
        return self.halfmove_clock >= 100 

    def is_threefold_repetition(self):
        return self.position_history.count(self.get_board_key()) >= 3

    def is_draw(self):
        return (
            self.is_stalemate() or
            self.is_insufficient_material() or
            self.is_fifty_move_rule() or
            self.is_threefold_repetition()
        )

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
