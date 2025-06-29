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
        board = [
            ['bR','bN','bB','bQ','bK','bB','bN','bR'],
            ['bP']*8,
            [None]*8,
            [None]*8,
            [None]*8,
            [None]*8,
            ['wP']*8,
            ['wR','wN','wB','wQ','wK','wB','wN','wR'],
        ]
        return board

    def _from_fen(self, fen):
        board = []
        rows = fen.split()[0].split('/')
        for row in rows:
            b_row = []
            for ch in row:
                if ch.isdigit():
                    b_row += [None]*int(ch)
                else:
                    color = 'w' if ch.isupper() else 'b'
                    piece = ch.upper()
                    b_row.append(color + piece)
            board.append(b_row)
        return board

    def fen(self):
        rows = []
        for row in self.board:
            empty = 0
            fen_row = ''
            for sq in row:
                if not sq:
                    empty += 1
                else:
                    if empty > 0:
                        fen_row += str(empty)
                        empty = 0
                    color, piece = sq[0], sq[1]
                    fen_row += piece.upper() if color == 'w' else piece.lower()
            if empty > 0:
                fen_row += str(empty)
            rows.append(fen_row)
        return '/'.join(rows)

    def find_pins(self, color):
        king_pos = None
        for r in range(8):
            for c in range(8):
                if self.board[r][c] == color + 'K':
                    king_pos = (r, c)
                    break
        if king_pos is None:
            return {}
        enemy = 'b' if color == 'w' else 'w'
        pins = {}
        directions = [(-1,0), (1,0), (0,-1), (0,1), (-1,-1), (-1,1), (1,-1), (1,1)]
        for dr, dc in directions:
            found_ally = None
            r1, c1 = king_pos[0]+dr, king_pos[1]+dc
            while 0 <= r1 < 8 and 0 <= c1 < 8:
                sq = self.board[r1][c1]
                if sq:
                    if sq[0] == color:
                        if found_ally is None:
                            found_ally = (r1, c1)
                        else:
                            break
                    else:
                        pt = sq[1]
                        if found_ally is not None:
                            if (dr == 0 or dc == 0) and pt in ('Q', 'R'):
                                pins[found_ally] = (dr, dc)
                            elif (dr != 0 and dc != 0) and pt in ('Q', 'B'):
                                pins[found_ally] = (dr, dc)
                        break
                r1 += dr
                c1 += dc
        return pins

    def attack_map(self, color, only_pawns=False):
        attacked = set()
        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]
                if sq and sq[0] == color:
                    if only_pawns and sq[1] != 'P':
                        continue
                    moves = self._gen_moves_for_piece(
                        sq[1], (r, c),
                        ignore_pins=True,
                        only_attacks=True
                    )
                    for _, end, _ in moves:
                        attacked.add(end)
        return attacked

    def _gen_moves_for_piece(self, pt, pos, ignore_pins=False, only_attacks=False, pin_dir=None):
        if pt == 'P':
            return self._gen_pawn_moves(pos, only_attacks=only_attacks, pin_dir=pin_dir if not ignore_pins else None)
        elif pt == 'N':
            if pin_dir and not ignore_pins:
                return []
            return self._gen_knight_moves(pos)
        elif pt in ('B', 'R', 'Q'):
            dirs = []
            if pt in ('B', 'Q'): dirs += [(-1,-1),(-1,1),(1,-1),(1,1)]
            if pt in ('R', 'Q'): dirs += [(-1,0),(1,0),(0,-1),(0,1)]
            if pin_dir and not ignore_pins:
                dirs = [d for d in dirs if d == pin_dir or d == (-pin_dir[0], -pin_dir[1])]
            return self._gen_sliding_moves(pos, dirs)
        elif pt == 'K':
            return self._gen_king_moves(pos)
        return []

    def _gen_pawn_moves(self, pos, only_attacks=False, pin_dir=None):
        moves = []
        r, c = pos
        col = self.turn
        dr = -1 if col == 'w' else 1
        start = 6 if col == 'w' else 1
        if not only_attacks:
            if (not pin_dir) or (pin_dir == (dr, 0)):
                if 0 <= r+dr < 8 and not self.board[r+dr][c]:
                    if (r+dr) in (0,7):
                        for p in 'QRBN': moves.append(((r, c), (r+dr, c), p))
                    else:
                        moves.append(((r, c), (r+dr, c), None))
                    if r == start and not self.board[r+2*dr][c]:
                        moves.append(((r, c), (r+2*dr, c), None))
        for dc in (-1, 1):
            rr, cc = r+dr, c+dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                if only_attacks or (not pin_dir) or (pin_dir == (dr, dc)):
                    if only_attacks:
                        moves.append(((r, c), (rr, cc), None))
                    else:
                        sq = self.board[rr][cc]
                        if sq and sq[0] != col:
                            if sq and sq[1] == 'K':
                                continue
                            if rr in (0,7):
                                for p in 'QRBN': moves.append(((r, c), (rr, cc), p))
                            else:
                                moves.append(((r, c), (rr, cc), None))
                        elif self.ep_square and (rr, cc) == self.ep_square:
                            moves.append(((r, c), (rr, cc), None))
        return moves

    def _gen_knight_moves(self, pos):
        moves = []
        r, c = pos
        col = self.turn
        for dr, dc in [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]:
            rr, cc = r+dr, c+dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]
                if sq and sq[1] == 'K':
                    continue
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
                    if sq[1] == 'K':
                        break 
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
                    if sq and sq[1] == 'K':
                        continue 
                    if not sq or sq[0] != col:
                        moves.append((pos, (rr, cc), None))
        return moves

    def legal_moves(self):
        color = self.turn
        enemy = 'b' if color == 'w' else 'w'
        pins = self.find_pins(color)
        enemy_attacks = self.attack_map(enemy)
        king_pos = None
        for r in range(8):
            for c in range(8):
                if self.board[r][c] == color+'K':
                    king_pos = (r, c)
                    break
        legal = []
        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]
                if sq and sq[0] == color:
                    pos = (r, c)
                    pin_dir = pins.get(pos)
                    moves = self._gen_moves_for_piece(sq[1], pos, pin_dir=pin_dir)
                    for move in moves:
                        (_, (er, ec), _) = move
                        target = self.board[er][ec]
                        if sq[1] == 'K' and move[1] in enemy_attacks:
                            continue
                        if target and target[1] == 'K':
                            continue
                        legal.append(move)
        return legal 

    def generate_pseudo_legal_moves(self):
        moves = []
        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]
                if sq and sq[0] == self.turn:
                    moves.extend(self._gen_moves_for_piece(sq[1], (r, c)))
                    filtered = []
                    for m in pmoves:
                        (_, (er, ec), _) = m
                        target = self.board[er][ec]
                        if not (target and target[1] == 'K'):
                            filtered.append(m)
                    moves.extend(filtered)
        return moves
    
    def is_check(self, color):
        if color is None:
            color = self.turn
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

    def is_checkmate(self, color=None):
        if color is None:
            color = self.turn
        return self.is_check(self.turn) and not self.legal_moves()

    def is_stalemate(self, color=None):
        if color is None:
            color = self.turn
        return not self.is_check(self.turn) and not self.legal_moves()

    def is_draw(self):
        if color is None:
            color = self.turn
        return (
            self.is_stalemate() or
            self.is_insufficient_material() or
            self.is_threefold_repetition() or
            self.is_fifty_move_rule()
        )
    def is_game_over(self, color=None):
        if color is None:
            color = self.turn
        return self.is_checkmate(color) or self.is_draw(color)
    
    def copy(self):
        new_board = Board()
        new_board.board = [list(row) for row in self.board]
        return new_board

    def push(self, move):
        (sr, sc), (er, ec), promotion = move if len(move) == 3 else (*move, None)
        piece = self.board[sr][sc]
        captured = self.board[er][ec]
        self.move_stack.append(((sr, sc), (er, ec), piece, captured, promotion))
        self.board[er][ec] = piece if not promotion else piece[0] + promotion
        self.board[sr][sc] = None
        self.turn = 'b' if self.turn == 'w' else 'w'

    def pop(self):
        if not self.move_stack:
            return
        (sr, sc), (er, ec), piece, captured, promotion = self.move_stack.pop()
        self.board[sr][sc] = piece
        self.board[er][ec] = captured
        self.turn = 'b' if self.turn == 'w' else 'w'
