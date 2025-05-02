import pygame
from game import Game
from utils import load_images, draw_board, draw_pieces, draw_buttons
from mode_select import select_mode
from bot import bot_move

WIDTH, HEIGHT = 640, 640
SIDE_WIDTH = 160
SQUARE_SIZE = WIDTH // 8

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")
        self.clock = pygame.time.Clock()
        self.images = load_images(SQUARE_SIZE)
        self.game = Game()
        self.running = True
        self.show_hints = False
        self.vs_bot = select_mode(self.win)

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.handle_events()
            self.draw()

            if self.vs_bot and self.game.turn == 'b':
                pygame.time.wait(300)
                bot_move(self.game, depth=2)
                self.draw()

            if self.game.is_checkmate():
                print(f"{'Black' if self.game.turn == 'w' else 'White'} wins by checkmate.")
                self.running = False
            elif self.game.is_stalemate():
                print("Draw by stalemate.")
                self.running = False

    def draw(self):
        self.win.fill((255, 255, 255))
        draw_board(self.win, SQUARE_SIZE)
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE)

        if self.game.selected:
            r, c = self.game.selected
            pygame.draw.rect(
                self.win,
                (140, 110, 150),
                (c * SQUARE_SIZE, r * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE),
                4
            )

        if self.show_hints and self.game.selected:
            r0, c0 = self.game.selected
            for r, c in self.game.get_valid_moves((r0, c0)):
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (140, 110, 150, 100), surf.get_rect())
                self.win.blit(surf, (c * SQUARE_SIZE, r * SQUARE_SIZE))

        mx, my = pygame.mouse.get_pos()
        if mx < WIDTH:
            r, c = my // SQUARE_SIZE, mx // SQUARE_SIZE
            p = self.game.board[r][c]
            if p and (r, c) != self.game.selected and p[0] == self.game.turn:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (190, 170, 200, 255), surf.get_rect(), 4)
                self.win.blit(surf, (c * SQUARE_SIZE, r * SQUARE_SIZE))

        draw_buttons(self.win)
        pygame.draw.rect(self.win, (200, 200, 200), (660, 200, 120, 40))
        font = pygame.font.SysFont("arial", 20)
        self.win.blit(font.render("Hint", True, (0, 0, 0)), (695, 210))
        pygame.display.update()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                x, y = pygame.mouse.get_pos()
                if x < WIDTH and (not self.vs_bot or self.game.turn == 'w'):
                    row, col = y // SQUARE_SIZE, x // SQUARE_SIZE
                    self.game.handle_click((row, col))
                    self.show_hints = False
                elif 660 <= x <= 780:
                    if 50 <= y <= 90:
                        self.game.undo_move()
                    elif 100 <= y <= 140:
                        self.game.save_game()
                    elif 150 <= y <= 190:
                        self.game.load_game()
                    elif 200 <= y <= 240:
                        self.show_hints = True