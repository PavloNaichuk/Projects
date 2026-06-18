class Board:
    def __init__(self, fen=None):
        self.castling_rights = {
            'w': {'K': True, 'Q': True},
            'b': {'K': True, 'Q': True},
        }
        self.ep_square = None
        self.halfmove_clock = 0
        self.fullmove_number = 1
        self.move_stack = []

        if fen is None:
            self.board = self._init_board()
            self.turn = 'w'
        else:
            self.board = self._parse_board_from_fen(fen)
            self._parse_fen_state(fen)

        self.history = [self.position_key()]

    def __getitem__(self, idx):
        return self.board[idx]

    @staticmethod
    def opponent(color):
        return 'b' if color == 'w' else 'w'

    def _init_board(self):
        return [
            ['bR', 'bN', 'bB', 'bQ', 'bK', 'bB', 'bN', 'bR'],
            ['bP'] * 8,
            [None] * 8,
            [None] * 8,
            [None] * 8,
            [None] * 8,
            ['wP'] * 8,
            ['wR', 'wN', 'wB', 'wQ', 'wK', 'wB', 'wN', 'wR'],
        ]

    def _from_fen(self, fen):
        """Backward-compatible helper: returns only the board matrix."""
        return self._parse_board_from_fen(fen)

    def _parse_board_from_fen(self, fen):
        board = []
        rows = fen.split()[0].split('/')

        if len(rows) != 8:
            raise ValueError("Invalid FEN: board must contain 8 rows")

        for row in rows:
            b_row = []
            for ch in row:
                if ch.isdigit():
                    b_row += [None] * int(ch)
                else:
                    color = 'w' if ch.isupper() else 'b'
                    piece = ch.upper()
                    if piece not in {'P', 'N', 'B', 'R', 'Q', 'K'}:
                        raise ValueError(f"Invalid FEN piece: {ch}")
                    b_row.append(color + piece)

            if len(b_row) != 8:
                raise ValueError("Invalid FEN: every row must contain 8 squares")
            board.append(b_row)

        return board

    def _parse_fen_state(self, fen):
        fields = fen.split()

        self.turn = fields[1] if len(fields) > 1 else 'w'
        if self.turn not in {'w', 'b'}:
            raise ValueError("Invalid FEN: active color must be 'w' or 'b'")

        self.castling_rights = {
            'w': {'K': False, 'Q': False},
            'b': {'K': False, 'Q': False},
        }

        castling = fields[2] if len(fields) > 2 else '-'
        if castling != '-':
            self.castling_rights['w']['K'] = 'K' in castling
            self.castling_rights['w']['Q'] = 'Q' in castling
            self.castling_rights['b']['K'] = 'k' in castling
            self.castling_rights['b']['Q'] = 'q' in castling

        ep = fields[3] if len(fields) > 3 else '-'
        self.ep_square = None if ep == '-' else self._parse_square_name(ep)

        self.halfmove_clock = int(fields[4]) if len(fields) > 4 else 0
        self.fullmove_number = int(fields[5]) if len(fields) > 5 else 1

    def fen(self):
        placement, turn, castling, ep = self._fen_core_parts()
        return f"{placement} {turn} {castling} {ep} {self.halfmove_clock} {self.fullmove_number}"

    def position_key(self):
        """
        Key for threefold repetition.

        Threefold repetition should compare the position, side to move,
        castling rights and en-passant square. It should not include
        halfmove/fullmove counters.
        """
        placement, turn, castling, ep = self._fen_core_parts()
        return f"{placement} {turn} {castling} {ep}"

    def _fen_core_parts(self):
        rows = []
        for row in self.board:
            empty = 0
            fen_row = ''

            for sq in row:
                if not sq:
                    empty += 1
                    continue

                if empty:
                    fen_row += str(empty)
                    empty = 0

                color, piece = sq[0], sq[1]
                fen_row += piece.upper() if color == 'w' else piece.lower()

            if empty:
                fen_row += str(empty)

            rows.append(fen_row)

        castling = ''
        if self.castling_rights['w']['K']:
            castling += 'K'
        if self.castling_rights['w']['Q']:
            castling += 'Q'
        if self.castling_rights['b']['K']:
            castling += 'k'
        if self.castling_rights['b']['Q']:
            castling += 'q'
        castling = castling or '-'

        ep = '-' if self.ep_square is None else self._square_name(self.ep_square)
        return '/'.join(rows), self.turn, castling, ep

    @staticmethod
    def _square_name(square):
        row, col = square
        return f"{chr(ord('a') + col)}{8 - row}"

    @staticmethod
    def _parse_square_name(name):
        if len(name) != 2:
            raise ValueError("Invalid square name")

        file_ch, rank_ch = name[0], name[1]
        if file_ch < 'a' or file_ch > 'h' or rank_ch < '1' or rank_ch > '8':
            raise ValueError("Invalid square name")

        col = ord(file_ch) - ord('a')
        row = 8 - int(rank_ch)
        return row, col

    def find_pins(self, color):
        king_pos = None

        for r in range(8):
            for c in range(8):
                if self.board[r][c] == color + 'K':
                    king_pos = (r, c)
                    break
            if king_pos is not None:
                break

        if king_pos is None:
            return {}

        pins = {}
        directions = [
            (-1, 0), (1, 0), (0, -1), (0, 1),
            (-1, -1), (-1, 1), (1, -1), (1, 1),
        ]

        for dr, dc in directions:
            found_ally = None
            r1, c1 = king_pos[0] + dr, king_pos[1] + dc

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
                if not sq or sq[0] != color:
                    continue

                if only_pawns and sq[1] != 'P':
                    continue

                moves = self._gen_moves_for_piece(
                    sq[1],
                    (r, c),
                    color=color,
                    ignore_pins=True,
                    only_attacks=True,
                )

                for _, end, _ in moves:
                    attacked.add(end)

        return attacked

    def is_attacked(self, r, c, by_color):
        return (r, c) in self.attack_map(by_color)

    def _gen_moves_for_piece(
        self,
        pt,
        pos,
        color=None,
        ignore_pins=False,
        only_attacks=False,
        pin_dir=None,
    ):
        if color is None:
            piece = self.board[pos[0]][pos[1]]
            color = piece[0] if piece else self.turn

        if pt == 'P':
            return self._gen_pawn_moves(
                pos,
                color,
                only_attacks=only_attacks,
                pin_dir=pin_dir if not ignore_pins else None,
            )

        if pt == 'N':
            if pin_dir and not ignore_pins and not only_attacks:
                return []
            return self._gen_knight_moves(pos, color, only_attacks=only_attacks)

        if pt in ('B', 'R', 'Q'):
            directions = []

            if pt in ('B', 'Q'):
                directions += [(-1, -1), (-1, 1), (1, -1), (1, 1)]
            if pt in ('R', 'Q'):
                directions += [(-1, 0), (1, 0), (0, -1), (0, 1)]

            if pin_dir and not ignore_pins and not only_attacks:
                directions = [
                    d for d in directions
                    if d == pin_dir or d == (-pin_dir[0], -pin_dir[1])
                ]

            return self._gen_sliding_moves(
                pos,
                directions,
                color,
                only_attacks=only_attacks,
            )

        if pt == 'K':
            return self._gen_king_moves(pos, color, only_attacks=only_attacks)

        return []

    def _gen_pawn_moves(self, pos, color, only_attacks=False, pin_dir=None):
        moves = []
        r, c = pos
        dr = -1 if color == 'w' else 1
        start = 6 if color == 'w' else 1

        if not only_attacks:
            if (not pin_dir) or (pin_dir == (dr, 0)):
                one_step = r + dr

                if 0 <= one_step < 8 and self.board[one_step][c] is None:
                    if one_step in (0, 7):
                        for promotion in 'QRBN':
                            moves.append(((r, c), (one_step, c), promotion))
                    else:
                        moves.append(((r, c), (one_step, c), None))

                    two_step = r + 2 * dr
                    if (
                        r == start
                        and 0 <= two_step < 8
                        and self.board[two_step][c] is None
                    ):
                        moves.append(((r, c), (two_step, c), None))

        for dc in (-1, 1):
            rr, cc = r + dr, c + dc

            if not (0 <= rr < 8 and 0 <= cc < 8):
                continue

            if only_attacks:
                moves.append(((r, c), (rr, cc), None))
                continue

            if pin_dir and pin_dir != (dr, dc):
                continue

            sq = self.board[rr][cc]

            if sq and sq[0] != color:
                if sq[1] == 'K':
                    continue

                if rr in (0, 7):
                    for promotion in 'QRBN':
                        moves.append(((r, c), (rr, cc), promotion))
                else:
                    moves.append(((r, c), (rr, cc), None))

            elif self.ep_square and (rr, cc) == self.ep_square:
                moves.append(((r, c), (rr, cc), None))

        return moves

    def _gen_knight_moves(self, pos, color, only_attacks=False):
        moves = []
        r, c = pos

        for dr, dc in [
            (-2, -1), (-2, 1), (-1, -2), (-1, 2),
            (1, -2), (1, 2), (2, -1), (2, 1),
        ]:
            rr, cc = r + dr, c + dc

            if not (0 <= rr < 8 and 0 <= cc < 8):
                continue

            if only_attacks:
                moves.append((pos, (rr, cc), None))
                continue

            sq = self.board[rr][cc]
            if not sq or (sq[0] != color and sq[1] != 'K'):
                moves.append((pos, (rr, cc), None))

        return moves

    def _gen_sliding_moves(self, pos, directions, color, only_attacks=False):
        moves = []
        r, c = pos

        for dr, dc in directions:
            rr, cc = r + dr, c + dc

            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]

                if sq is None:
                    moves.append((pos, (rr, cc), None))
                else:
                    if only_attacks:
                        moves.append((pos, (rr, cc), None))
                    elif sq[0] != color and sq[1] != 'K':
                        moves.append((pos, (rr, cc), None))
                    break

                rr += dr
                cc += dc

        return moves

    def _gen_king_moves(self, pos, color, only_attacks=False):
        moves = []
        r, c = pos

        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue

                rr, cc = r + dr, c + dc

                if not (0 <= rr < 8 and 0 <= cc < 8):
                    continue

                if only_attacks:
                    moves.append((pos, (rr, cc), None))
                    continue

                sq = self.board[rr][cc]
                if not sq or (sq[0] != color and sq[1] != 'K'):
                    moves.append((pos, (rr, cc), None))

        if only_attacks:
            return moves

        enemy_color = self.opponent(color)
        home_row = 7 if color == 'w' else 0

        if (
            self.castling_rights[color]['K']
            and self.board[home_row][5] is None
            and self.board[home_row][6] is None
            and self.board[home_row][4] == color + 'K'
            and self.board[home_row][7] == color + 'R'
            and not self.is_check(color)
            and not self.is_attacked(home_row, 4, enemy_color)
            and not self.is_attacked(home_row, 5, enemy_color)
            and not self.is_attacked(home_row, 6, enemy_color)
        ):
            moves.append(((home_row, 4), (home_row, 6), None))

        if (
            self.castling_rights[color]['Q']
            and self.board[home_row][3] is None
            and self.board[home_row][2] is None
            and self.board[home_row][1] is None
            and self.board[home_row][4] == color + 'K'
            and self.board[home_row][0] == color + 'R'
            and not self.is_check(color)
            and not self.is_attacked(home_row, 4, enemy_color)
            and not self.is_attacked(home_row, 3, enemy_color)
            and not self.is_attacked(home_row, 2, enemy_color)
        ):
            moves.append(((home_row, 4), (home_row, 2), None))

        return moves

    def legal_moves(self, color=None):
        if color is None:
            color = self.turn

        enemy = self.opponent(color)
        pins = self.find_pins(color)
        enemy_attacks = self.attack_map(enemy)
        legal = []

        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]

                if not sq or sq[0] != color:
                    continue

                pos = (r, c)
                pin_dir = pins.get(pos)

                moves = self._gen_moves_for_piece(
                    sq[1],
                    pos,
                    color=color,
                    pin_dir=pin_dir,
                )

                for move in moves:
                    _, (er, ec), promo = move
                    target = self.board[er][ec]

                    if sq[1] == 'K' and move[1] in enemy_attacks:
                        continue

                    if target and target[1] == 'K':
                        continue

                    board_copy = self.copy()
                    board_copy._move_piece_internal(pos, (er, ec), promo)

                    if not board_copy.is_check(color):
                        legal.append(move)

        return legal

    def generate_pseudo_legal_moves(self, color=None):
        if color is None:
            color = self.turn

        moves = []

        for r in range(8):
            for c in range(8):
                sq = self.board[r][c]

                if not sq or sq[0] != color:
                    continue

                pseudo_moves = self._gen_moves_for_piece(
                    sq[1],
                    (r, c),
                    color=color,
                )

                for move in pseudo_moves:
                    _, (er, ec), _ = move
                    target = self.board[er][ec]

                    if not (target and target[1] == 'K'):
                        moves.append(move)

        return moves

    def is_check(self, color=None):
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

        attacker = self.opponent(color)

        pawn_dr = -1 if attacker == 'w' else 1
        for dc in (-1, 1):
            rr, cc = kr + pawn_dr, kc + dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                if self.board[rr][cc] == attacker + 'P':
                    return True

        for dr, dc in [
            (-2, -1), (-2, 1), (-1, -2), (-1, 2),
            (1, -2), (1, 2), (2, -1), (2, 1),
        ]:
            rr, cc = kr + dr, kc + dc
            if 0 <= rr < 8 and 0 <= cc < 8:
                if self.board[rr][cc] == attacker + 'N':
                    return True

        for dr, dc in [(-1, -1), (-1, 1), (1, -1), (1, 1)]:
            rr, cc = kr + dr, kc + dc

            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]

                if sq:
                    if sq[0] == attacker and sq[1] in ('B', 'Q'):
                        return True
                    break

                rr += dr
                cc += dc

        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            rr, cc = kr + dr, kc + dc

            while 0 <= rr < 8 and 0 <= cc < 8:
                sq = self.board[rr][cc]

                if sq:
                    if sq[0] == attacker and sq[1] in ('R', 'Q'):
                        return True
                    break

                rr += dr
                cc += dc

        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue

                rr, cc = kr + dr, kc + dc

                if 0 <= rr < 8 and 0 <= cc < 8:
                    if self.board[rr][cc] == attacker + 'K':
                        return True

        return False

    def is_threefold_repetition(self):
        current = self.position_key()
        return self.history.count(current) >= 3

    def is_fifty_move_rule(self):
        return self.halfmove_clock >= 100

    def is_insufficient_material(self):
        pieces = [p for row in self.board for p in row if p]

        if set(p[1] for p in pieces) == {'K'} and len(pieces) == 2:
            return True

        if len(pieces) == 3:
            names = sorted(p[1] for p in pieces)
            if names.count('K') == 2 and ('B' in names or 'N' in names):
                return True

        return False

    def is_checkmate(self, color=None):
        if color is None:
            color = self.turn
        return self.is_check(color) and not self.legal_moves(color)

    def is_stalemate(self, color=None):
        if color is None:
            color = self.turn
        return not self.is_check(color) and not self.legal_moves(color)

    def is_draw(self, color=None):
        if color is None:
            color = self.turn

        return (
            self.is_stalemate(color)
            or self.is_insufficient_material()
            or self.is_threefold_repetition()
            or self.is_fifty_move_rule()
        )

    def is_game_over(self, color=None):
        if color is None:
            color = self.turn
        return self.is_checkmate(color) or self.is_draw(color)

    def copy(self):
        import copy

        new_board = Board()
        new_board.board = copy.deepcopy(self.board)
        new_board.turn = self.turn
        new_board.castling_rights = copy.deepcopy(self.castling_rights)
        new_board.ep_square = self.ep_square
        new_board.halfmove_clock = self.halfmove_clock
        new_board.fullmove_number = self.fullmove_number
        new_board.move_stack = list(self.move_stack)
        new_board.history = list(self.history)
        return new_board

    def push(self, move):
        (sr, sc), (er, ec), promotion = move if len(move) == 3 else (*move, None)
        piece = self.board[sr][sc]
        captured = self.board[er][ec]

        old_castling = {
            side: self.castling_rights[side].copy()
            for side in self.castling_rights
        }
        old_ep = self.ep_square
        old_halfmove_clock = self.halfmove_clock
        old_fullmove_number = self.fullmove_number

        if piece and piece[1] == 'K':
            self.castling_rights[piece[0]]['K'] = False
            self.castling_rights[piece[0]]['Q'] = False

        if piece and piece[1] == 'R':
            if (sr, sc) == (7, 0):
                self.castling_rights['w']['Q'] = False
            elif (sr, sc) == (7, 7):
                self.castling_rights['w']['K'] = False
            elif (sr, sc) == (0, 0):
                self.castling_rights['b']['Q'] = False
            elif (sr, sc) == (0, 7):
                self.castling_rights['b']['K'] = False

        if captured and captured[1] == 'R':
            if (er, ec) == (7, 0):
                self.castling_rights['w']['Q'] = False
            elif (er, ec) == (7, 7):
                self.castling_rights['w']['K'] = False
            elif (er, ec) == (0, 0):
                self.castling_rights['b']['Q'] = False
            elif (er, ec) == (0, 7):
                self.castling_rights['b']['K'] = False

        self.move_stack.append((
            (sr, sc),
            (er, ec),
            piece,
            captured,
            promotion,
            old_castling,
            old_ep,
            old_halfmove_clock,
            old_fullmove_number,
        ))

        self._move_piece_internal((sr, sc), (er, ec), promotion, ep_square=old_ep)

        self.ep_square = None
        if piece and piece[1] == 'P' and abs(er - sr) == 2:
            self.ep_square = ((sr + er) // 2, sc)

        if (piece and piece[1] == 'P') or captured:
            self.halfmove_clock = 0
        else:
            self.halfmove_clock += 1

        if piece and piece[0] == 'b':
            self.fullmove_number += 1

        self.turn = self.opponent(self.turn)
        self.history.append(self.position_key())

    def pop(self):
        if not self.move_stack:
            return

        (
            (sr, sc),
            (er, ec),
            piece,
            captured,
            promotion,
            old_castling,
            old_ep,
            old_halfmove_clock,
            old_fullmove_number,
        ) = self.move_stack.pop()

        self.board[sr][sc] = piece
        self.board[er][ec] = captured

        if piece and piece[1] == 'K' and abs(ec - sc) == 2:
            if ec > sc:
                self.board[sr][7], self.board[sr][ec - 1] = self.board[sr][ec - 1], None
            else:
                self.board[sr][0], self.board[sr][ec + 1] = self.board[sr][ec + 1], None

        if piece and piece[1] == 'P' and sc != ec and captured is None:
            if piece[0] == 'w':
                self.board[er + 1][ec] = 'bP'
            else:
                self.board[er - 1][ec] = 'wP'
            self.board[er][ec] = None

        self.turn = self.opponent(self.turn)
        self.castling_rights = {
            side: old_castling[side].copy()
            for side in old_castling
        }
        self.ep_square = old_ep
        self.halfmove_clock = old_halfmove_clock
        self.fullmove_number = old_fullmove_number

        if len(self.history) > 1:
            self.history.pop()

    def _move_piece_internal(self, start, end, promotion=None, ep_square=None):
        sr, sc = start
        er, ec = end
        piece = self.board[sr][sc]
        effective_ep_square = self.ep_square if ep_square is None else ep_square

        if (
            piece
            and piece[1] == 'P'
            and effective_ep_square == (er, ec)
            and sc != ec
            and self.board[er][ec] is None
        ):
            self.board[sr][sc] = None
            self.board[er][ec] = piece

            if piece[0] == 'w':
                self.board[er + 1][ec] = None
            else:
                self.board[er - 1][ec] = None

            return

        if piece and piece[1] == 'K' and abs(ec - sc) == 2:
            self.board[sr][sc] = None
            self.board[er][ec] = piece

            if ec > sc:
                self.board[sr][7], self.board[sr][ec - 1] = None, self.board[sr][7]
            else:
                self.board[sr][0], self.board[sr][ec + 1] = None, self.board[sr][0]

            return

        if promotion and piece and piece[1] == 'P' and er in (0, 7):
            self.board[er][ec] = piece[0] + promotion
            self.board[sr][sc] = None
            return

        self.board[er][ec] = piece
        self.board[sr][sc] = None
