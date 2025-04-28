
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

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.draw()
            self.handle_events()

    def draw(self):
        self.win.fill((255, 255, 255))
        draw_board(self.win, SQUARE_SIZE)
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE)
        draw_buttons(self.win)
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
                elif 660 <= x <= 780:
                    if 50 <= y <= 90:
                        self.game.undo_move()
                    elif 100 <= y <= 140:
                        self.game.save_game()
                    elif 150 <= y <= 190:
                        self.game.load_game()
