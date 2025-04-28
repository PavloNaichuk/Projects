
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
        self.move_log.append((start, end, piece, target))
        self.board[er][ec] = piece
        self.board[sr][sc] = ''
        self.switch_turn()

    def undo_move(self):
        if self.move_log:
            start, end, piece, captured = self.move_log.pop()
            sr, sc = start
            er, ec = end
            self.board[sr][sc] = piece
            self.board[er][ec] = captured
            self.switch_turn()

    def switch_turn(self):
        self.turn = 'b' if self.turn == 'w' else 'w'

    def save_game(self, filename="save.txt"):
        with open(filename, "w") as f:
            for row in self.board:
                f.write(",".join(row) + "\n")

    def load_game(self, filename="save.txt"):
        try:
            with open(filename, "r") as f:
                lines = f.readlines()
                self.board = [line.strip().split(',') for line in lines]
        except FileNotFoundError:
            print("Файл не знайдено.")

    def get_valid_moves(self, pos):
        r, c = pos
        piece = self.board[r][c]
        if piece == '':
            return []

        color, ptype = piece[0], piece[1]
        directions = []
        moves = []

        if ptype == 'P':
            dir = -1 if color == 'w' else 1
            start_row = 6 if color == 'w' else 1
            if self.board[r + dir][c] == '':
                moves.append((r + dir, c))
                if r == start_row and self.board[r + dir * 2][c] == '':
                    moves.append((r + dir * 2, c))
            for dc in [-1, 1]:
                if 0 <= c + dc < 8:
                    target = self.board[r + dir][c + dc]
                    if target != '' and target[0] != color:
                        moves.append((r + dir, c + dc))

        elif ptype == 'R':
            directions = [(-1,0), (1,0), (0,-1), (0,1)]

        elif ptype == 'B':
            directions = [(-1,-1), (-1,1), (1,-1), (1,1)]

        elif ptype == 'Q':
            directions = [(-1,0), (1,0), (0,-1), (0,1), (-1,-1), (-1,1), (1,-1), (1,1)]

        elif ptype == 'N':
            knight_moves = [(-2, -1), (-2, 1), (-1, -2), (-1, 2),
                            (1, -2), (1, 2), (2, -1), (2, 1)]
            for dr, dc in knight_moves:
                nr, nc = r + dr, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    target = self.board[nr][nc]
                    if target == '' or target[0] != color:
                        moves.append((nr, nc))

        elif ptype == 'K':
            king_moves = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]
            for dr, dc in king_moves:
                nr, nc = r + dr, c + dc
                if 0 <= nr < 8 and 0 <= nc < 8:
                    target = self.board[nr][nc]
                    if target == '' or target[0] != color:
                        moves.append((nr, nc))

        # For Rook, Bishop, Queen directions
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
