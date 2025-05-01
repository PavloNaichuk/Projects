class Game:
    def __init__(self):
        self.board = self.init_board()
        self.turn = 'w'
        self.selected = None
        self.move_log = []

    def init_board(self):
        return [
            ['bR', 'bN', 'bB', 'bQ', 'bK', 'bB', 'bN', 'bR'],
            ['bP'] * 8,
            [''] * 8,
            [''] * 8,
            [''] * 8,
            [''] * 8,
            ['wP'] * 8,
            ['wR', 'wN', 'wB', 'wQ', 'wK', 'wB', 'wN', 'wR']
        ]

    def switch_turn(self):
        self.turn = 'b' if self.turn == 'w' else 'w'

    def handle_click(self, pos):
        row, col = pos
        if self.selected is None:
            piece = self.board[row][col]
            if piece != '' and piece[0] == self.turn:
                self.selected = (row, col)
        else:
            if (row, col) in self.get_valid_moves(self.selected):
                self.move_piece(self.selected, (row, col))
            self.selected = None

    def move_piece(self, start, end):
        sr, sc = start
        er, ec = end
        piece = self.board[sr][sc]
        target = self.board[er][ec]
        if piece != '' and (target == '' or target[0] != piece[0]):
            self.board[er][ec] = piece
            self.board[sr][sc] = ''
            self.switch_turn()

    def get_valid_moves(self, pos):
        r, c = pos
        piece = self.board[r][c]
        if piece == '':
            return []
        color = piece[0]
        moves = []
        directions = []

        if piece[1] == 'P':
            dir = -1 if color == 'w' else 1
            start_row = 6 if color == 'w' else 1
            if self.board[r + dir][c] == '':
                moves.append((r + dir, c))
                if r == start_row and self.board[r + 2 * dir][c] == '':
                    moves.append((r + 2 * dir, c))
            for dc in [-1, 1]:
                nc = c + dc
                nr = r + dir
                if 0 <= nc < 8 and 0 <= nr < 8:
                    target = self.board[nr][nc]
                    if target != '' and target[0] != color:
                        moves.append((nr, nc))

        elif piece[1] == 'R':
            directions = [(-1,0),(1,0),(0,-1),(0,1)]
        elif piece[1] == 'B':
            directions = [(-1,-1),(-1,1),(1,-1),(1,1)]
        elif piece[1] == 'Q':
            directions = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]
        elif piece[1] == 'N':
            knight_moves = [(-2,-1),(-2,1),(-1,-2),(-1,2),(1,-2),(1,2),(2,-1),(2,1)]
            for dr, dc in knight_moves:
                nr, nc = r + dr, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    target = self.board[nr][nc]
                    if target == '' or target[0] != color:
                        moves.append((nr, nc))
        elif piece[1] == 'K':
            king_moves = [(-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),(1,1)]
            for dr, dc in king_moves:
                nr, nc = r + dr, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    target = self.board[nr][nc]
                    if target == '' or target[0] != color:
                        moves.append((nr, nc))

        for dr, dc in directions:
            nr, nc = r + dr, c + dc
            while 0 <= nr < 8 and 0 <= nc < 8:
                target = self.board[nr][nc]
                if target == '':
                    moves.append((nr, nc))
                elif target[0] != color:
                    moves.append((nr, nc))
                    break
                else:
                    break
                nr += dr
                nc += dc

        return moves

    def is_in_check(self, color):
        king_pos = None
        for r in range(8):
            for c in range(8):
                if self.board[r][c] == color + 'K':
                    king_pos = (r, c)
                    break
        if not king_pos:
            return True

        for r in range(8):
            for c in range(8):
                piece = self.board[r][c]
                if piece != '' and piece[0] != color:
                    moves = self.get_valid_moves((r, c))
                    if king_pos in moves:
                        return True
        return False
