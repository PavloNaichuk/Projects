class Board:
    def __init__(self, fen=None):
        self.board = self._init_board() if fen is None else self._from_fen(fen)
        self.turn = 'w'
        self.castling_rights = {'w': {'K': True, 'Q': True}, 'b': {'K': True, 'Q': True}}
        self.ep_square = None
        self.halfmove_clock = 0
        self.fullmove_number = 1
        self.move_stack = []
        self.history = [self.fen()]

    def __getitem__(self, idx):
        return self.board[idx]

    def _init_board(self):
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

    def _from_fen(self, fen):
        parts = fen.split()
        rows = parts[0].split('/')
        board = []
        for r in rows:
            row = []
            for ch in r:
                if ch.isdigit():
                    row.extend([''] * int(ch))
                else:
                    color = 'w' if ch.isupper() else 'b'
                    row.append(color + ch.lower().upper())
            board.append(row)
        self.turn = parts[1]
        cr = parts[2]
        self.castling_rights = {
            'w': {'K': 'K' in cr, 'Q': 'Q' in cr},
            'b': {'K': 'k' in cr, 'Q': 'q' in cr}
        }
        self.ep_square = None if parts[3] == '-' else (8 - int(parts[3][1]), ord(parts[3][0]) - ord('a'))
        self.halfmove_clock = int(parts[4])
        self.fullmove_number = int(parts[5])
        return board

    def push(self, move):
        (sr, sc), (er, ec), promo = move
        piece = self.board[sr][sc]
        if not piece:
            raise ValueError(f"Board.push: no piece at square {(sr, sc)}, move={move}")
        captured = ''
        if piece[1] == 'P' and self.ep_square == (er, ec) and sc != ec:
            cap_r, cap_c = sr, ec
            captured = self.board[cap_r][cap_c]
            self.board[cap_r][cap_c] = ''
        else:
            captured = self.board[er][ec]
        self.move_stack.append((
            sr, sc, er, ec, piece, captured,
            { 'w': self.castling_rights['w'].copy(), 'b': self.castling_rights['b'].copy() },
            self.ep_square, self.halfmove_clock, self.fullmove_number, self.turn
        ))
        if promo and piece[1] == 'P' and er in (0, 7):
            self.board[er][ec] = piece[0] + promo
        else:
            self.board[er][ec] = piece
        self.board[sr][sc] = ''
        if piece[1] == 'K':
            self.castling_rights[piece[0]] = {'K': False, 'Q': False}   
        if piece[1] == 'R':
            if sc == 0:
                self.castling_rights[piece[0]]['Q'] = False
            if sc == 7:
                self.castling_rights[piece[0]]['K'] = False
        if piece[1] == 'P' and abs(er - sr) == 2:
            self.ep_square = ((sr + er) // 2, sc)
        else:
            self.ep_square = None
        if piece[1] == 'P' or captured:
            self.halfmove_clock = 0
        else:
            self.halfmove_clock += 1
        if self.turn == 'b':
            self.fullmove_number += 1
        self.turn = 'b' if self.turn == 'w' else 'w'
        self.history.append(self.fen())

    def pop(self):
        sr, sc, er, ec, piece, captured, crights, ep, half, full, turn = self.move_stack.pop()
        self.board[sr][sc] = piece
        self.board[er][ec] = captured
        self.castling_rights = { 'w': crights['w'].copy(), 'b': crights['b'].copy() }
        self.ep_square = ep
        self.halfmove_clock = half
        self.fullmove_number = full
        self.turn = turn
        if len(self.history) > 1:
            self.history.pop()

    def fen(self):
        rows = []
        for rank in self.board:
            empty = 0
            fen_rank = ''
            for sq in rank:
                if not sq:
                    empty += 1
                else:
                    if empty:
                        fen_rank += str(empty)
                        empty = 0
                    fen_rank += sq[1].upper() if sq[0] == 'w' else sq[1].lower()
            if empty:
                fen_rank += str(empty)
            rows.append(fen_rank)
        placement = '/'.join(rows)
        active = self.turn
        cr = ''
        if self.castling_rights['w']['K']: cr += 'K'
        if self.castling_rights['w']['Q']: cr += 'Q'
        if self.castling_rights['b']['K']: cr += 'k'
        if self.castling_rights['b']['Q']: cr += 'q'
        if not cr: cr = '-'
        if self.ep_square:
            file = chr(self.ep_square[1] + ord('a'))
            rank = str(8 - self.ep_square[0])
            ep = file + rank
        else:
            ep = '-'
        return f"{placement} {active} {cr} {ep} {self.halfmove_clock} {self.fullmove_number}"

    def is_legal(self, move):
        if move not in self.generate_pseudo_legal_moves():
            return False
        self.push(move)
        valid = not self.is_check(self.turn)
        self.pop()
        return valid

    def legal_moves(self):
        return [m for m in self.generate_pseudo_legal_moves() if self.is_legal(m)]

    def generate_pseudo_legal_moves(self):
        moves = []
        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]
                if sq and sq[0] == self.turn:
                    moves.extend(self._gen_moves_for_piece(sq[1], (r, c)))
        return moves

    def _gen_moves_for_piece(self, pt, pos):
        if pt == 'P': return self._gen_pawn_moves(pos)
        if pt == 'N': return self._gen_knight_moves(pos)
        if pt in ('B', 'R', 'Q'):
            dirs = []
            if pt in ('B', 'Q'): dirs += [(-1,-1),(-1,1),(1,-1),(1,1)]
            if pt in ('R', 'Q'): dirs += [(-1,0),(1,0),(0,-1),(0,1)]
            return self._gen_sliding_moves(pos, dirs)
        if pt == 'K': return self._gen_king_moves(pos)
        return []

    def _gen_pawn_moves(self, pos):
        moves = []
        r, c = pos
        col = self.turn
        dr = -1 if col == 'w' else 1
        start = 6 if col == 'w' else 1
        if 0 <= r+dr < 8 and not self.board[r+dr][c]:
            if (r+dr) in (0,7):
                for p in 'QRBN': moves.append((pos, (r+dr, c), p))
            else:
                moves.append((pos, (r+dr, c), None))
            if r == start and not self.board[r+2*dr][c]:
                moves.append((pos, (r+2*dr, c), None))
        for dc in (-1, 1):
            rr, cc = r+dr, c+dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if sq and sq[0] != col:
                    if rr in (0,7):
                        for p in 'QRBN': moves.append((pos, (rr, cc), p))
                    else:
                        moves.append((pos, (rr, cc), None))
                elif (rr, cc) == self.ep_square:
                    moves.append((pos, self.ep_square, None))
        return moves

    def _gen_knight_moves(self, pos):
        moves = []
        r, c = pos
        col = self.turn
        for dr, dc in [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]:
            rr, cc = r+dr, c+dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if not sq or sq[0] != col:
                    moves.append((pos, (rr, cc), None))
        return moves

    def _gen_sliding_moves(self, pos, directions):
        moves = []
        r, c = pos
        col = self.turn
        for dr, dc in directions:
            rr, cc = r+dr, c+dc
            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if not sq:
                    moves.append((pos, (rr, cc), None))
                else:
                    if sq[0] != col:
                        moves.append((pos, (rr, cc), None))
                    break
                rr += dr
                cc += dc
        return moves

    def _gen_king_moves(self, pos):
        moves = []
        r, c = pos
        col = self.turn
        for dr in (-1,0,1):
            for dc in (-1,0,1):
                if dr == 0 and dc == 0:
                    continue
                rr, cc = r+dr, c+dc
                if 0 <= rr < 8 and 0 <= cc < 8:
                    sq = self.board[rr][cc]
                    if not sq or sq[0] != col:
                        moves.append((pos, (rr, cc), None))
        if not self.is_check(col):
            if self.castling_rights[col]['K']:
                if all(not self.board[r][cc] for cc in (c+1, c+2)):
                    if not self.is_check_on_square(r, c+1, col) and not self.is_check_on_square(r, c+2, col):
                        moves.append((pos, (r, c+2), None))
            if self.castling_rights[col]['Q']:
                if all(not self.board[r][cc] for cc in (c-1, c-2, c-3)):
                    if not self.is_check_on_square(r, c-1, col) and not self.is_check_on_square(r, c-2, col):
                        moves.append((pos, (r, c-2), None))
        return moves

    def is_check_on_square(self, r, c, color):
        attacker = 'w' if color == 'b' else 'b'
        orig_piece = self.board[r][c]
        self.board[r][c] = color + 'K'
        res = self.is_check(color)
        self.board[r][c] = orig_piece
        return res

    def is_check(self, color):
        kr = kc = None
        for r in range(8):
            for c in range(8):
                if self.board[r][c] == color + 'K':
                    kr, kc = r, c
                    break
            if kr is not None:
                break
        if kr is None:
            return False
        attacker = 'w' if color == 'b' else 'b'
        dr = -1 if attacker == 'w' else 1
        for dc in (-1, 1):
            rr, cc = kr+dr, kc+dc
            if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == attacker + 'P':
                return True
        for dr, dc in [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]:
            rr, cc = kr+dr, kc+dc
            if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == attacker + 'N':
                return True
        for dr, dc in [(-1,-1),(-1,1),(1,-1),(1,1)]:
            rr, cc = kr+dr, kc+dc
            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if sq:
                    if sq[0] == attacker and sq[1] in ('B','Q'):
                        return True
                    break
                rr += dr; cc += dc
        for dr, dc in [(-1,0),(1,0),(0,-1),(0,1)]:
            rr, cc = kr+dr, kc+dc
            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if sq:
                    if sq[0] == attacker and sq[1] in ('R','Q'):
                        return True
                    break
                rr += dr; cc += dc
        for dr in (-1,0,1):
            for dc in (-1,0,1):
                if dr == 0 and dc == 0:
                    continue
                rr, cc = kr+dr, kc+dc
                if 0 <= rr < 8 and 0 <= cc < 8 and self.board[rr][cc] == attacker + 'K':
                    return True
        return False

    def is_threefold_repetition(self):
        current = self.fen()
        return self.history.count(current) >= 3

    def is_fifty_move_rule(self):
        return self.halfmove_clock >= 100

    def is_insufficient_material(self):
        pieces = [p[1] for row in self.board for p in row if p]
        return set(pieces) <= set(['K'])

    def is_checkmate(self):
        return self.is_check(self.turn) and not self.legal_moves()

    def is_stalemate(self):
        return not self.is_check(self.turn) and not self.legal_moves()

    def is_draw(self):
        return (
            self.is_stalemate() or
            self.is_insufficient_material() or
            self.is_threefold_repetition() or
            self.is_fifty_move_rule()
        )