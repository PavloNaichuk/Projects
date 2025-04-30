import pygame
from game import Game
from utils import load_images, draw_board, draw_pieces, draw_buttons

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

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.draw()
            self.handle_events()

    def draw(self):
        self.win.fill((255, 255, 255))
        draw_board(self.win, SQUARE_SIZE)

        if self.game.selected:
            row, col = self.game.selected
            pygame.draw.rect(
                self.win,
                (140, 110, 150),
                (col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE),
                4
            )

            if self.show_hints:
                for move in self.game.get_valid_moves((row, col)):
                    r, c = move
                    pygame.draw.circle(
                        self.win,
                        (140, 110, 150),
                        (c * SQUARE_SIZE + SQUARE_SIZE // 2, r * SQUARE_SIZE + SQUARE_SIZE // 2),
                        10
                    )

        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE)
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
                if x < 640:
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
