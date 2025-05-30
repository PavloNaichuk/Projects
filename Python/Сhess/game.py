import pickle
import copy
import time

class Game:
    def __init__(self):
        self.board = self.init_board()
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
        piece = self.board[sr][sc]
        captured = self.board[er][ec]

        if piece[1] in ('K', 'R'):
            self.has_moved[(sr, sc)] = True

        orig_piece = piece
        if orig_piece[1] == 'P' and ((orig_piece[0] == 'w' and er == 0) or (orig_piece[0] == 'b' and er == 7)):
            promo = promotion if promotion in ('Q','R','B','N') else 'Q'
            piece = orig_piece[0] + promo

        self.move_log.append((start, end, orig_piece, captured))

        if orig_piece[1] == 'K' and abs(ec - sc) == 2:
            if ec > sc:
                self.board[sr][5] = self.board[sr][7]
                self.board[sr][7] = ''
                self.has_moved[(sr, 7)] = True
            else:
                self.board[sr][3] = self.board[sr][0]
                self.board[sr][0] = ''
                self.has_moved[(sr, 0)] = True

        self.board[er][ec] = piece
        self.board[sr][sc] = ''

        if orig_piece[1] in ('K', 'R'):
            self.has_moved[(er, ec)] = self.has_moved.pop((sr, sc))

        if orig_piece[1] == 'P' or captured:
            self.halfmove_clock = 0
        else:
            self.halfmove_clock += 1

        now      = time.time()
        elapsed  = now - self.move_start_time
        self.time_remaining[self.turn] -= elapsed
        self.time_remaining[self.turn] += self.time_control[1]

        self.switch_turn()
        self.move_start_time = time.time()
        self.position_history.append(self.get_board_key())

    def undo_move(self):
        if not self.move_log:
            return
        start, end, piece, captured = self.move_log.pop()
        sr, sc = start
        er, ec = end
        
        self.board[sr][sc] = piece
        self.board[er][ec] = captured

        if piece[1] == 'K' and abs(ec - sc) == 2:
            if ec > sc:
                self.board[sr][7] = self.board[sr][5]
                self.board[sr][5] = ''
            else:
                self.board[sr][0] = self.board[sr][3]
                self.board[sr][3] = ''

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
        moves = []
        if p == 'P':
            step = -1 if color == 'w' else 1
            start_row = 6 if color == 'w' else 1
            nr, nc = r + step, c
            if 0 <= nr < 8 and self.board[nr][nc] == '':
                moves.append((nr, nc))
                nr2 = r + 2*step
                if r == start_row and self.board[nr2][nc] == '':
                    moves.append((nr2, nc))
        
            for dc in (-1, 1):
                nr, nc = r + step, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    t = self.board[nr][nc]
                    if t and t[0] != color and t[1] != 'K':
                        moves.append((nr, nc))

        elif p == 'N':
            for dr, dc in [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]:
                nr, nc = r + dr, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    t = self.board[nr][nc]
                    if not t or (t[0] != color and t[1] != 'K'):
                        moves.append((nr, nc))

        elif p == 'K':
            for dr in (-1, 0, 1):
                for dc in (-1, 0, 1):
                    if dr == 0 and dc == 0:
                        continue
                    nr, nc = r + dr, c + dc
                    if 0 <= nr < 8 and 0 <= nc < 8:
                        t = self.board[nr][nc]
                        if not t or (t[0] != color and t[1] != 'K'):
                            moves.append((nr, nc))

            if not self.has_moved.get((r, c), True) and not self.is_in_check(color):
                row = r
                if (not self.has_moved.get((row, 7), True)
                    and self.board[row][5] == '' and self.board[row][6] == ''):
                    safe = True
                    for cc in (5, 6):
                        self.board[row][4], self.board[row][cc] = '', piece
                        if self.is_in_check(color):
                            safe = False
                        self.board[row][cc], self.board[row][4] = '', piece
                        if not safe:
                            break
                    if safe:
                        moves.append((row, 6))

                if (not self.has_moved.get((row, 0), True)
                    and self.board[row][1] == '' and self.board[row][2] == '' and self.board[row][3] == ''):
                    safe = True
                    for cc in (3, 2):
                        self.board[row][4], self.board[row][cc] = '', piece
                        if self.is_in_check(color):
                            safe = False
                        self.board[row][cc], self.board[row][4] = '', piece
                        if not safe:
                            break
                    if safe:
                        moves.append((row, 2))

        else:
            if p == 'R':
                directions = [(-1,0),(1,0),(0,-1),(0,1)]
            elif p == 'B':
                directions = [(-1,-1),(-1,1),(1,-1),(1,1)]
            elif p == 'Q':
                directions = [(-1,0),(1,0),(0,-1),(0,1), (-1,-1),(-1,1),(1,-1),(1,1)]
            else:
                directions = []
                
            for dr, dc in directions:
                nr, nc = r + dr, c + dc
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
        orig_has_moved = self.has_moved.copy()
        for dest in self.get_valid_moves(pos):
            self.move_piece(pos, dest)
            in_check = self.is_in_check(color)
            self.undo_move()
            self.has_moved = orig_has_moved.copy()
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
