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
            self.move_piece(self.selected, (row, col))
            self.selected = None

    def move_piece(self, start, end):
        sr, sc = start
        er, ec = end
        piece = self.board[sr][sc]
        target = self.board[er][ec]

        if piece != '' and (target == '' or target[0] != piece[0]):
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
            print("No find  file.")
