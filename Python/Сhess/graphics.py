import sys
import pygame
from game import Game
from utils import load_images, draw_board, draw_pieces
from mode_select import select_mode
from bot import bot_move
from game_over_window import GameOverWindow

WIDTH, HEIGHT = 640, 640
SIDE_WIDTH = 160
SQUARE_SIZE = WIDTH // 8

BG_COLOR        = (255, 255, 255)
PANEL_COLOR     = (180, 180, 180)
HIGHLIGHT_COLOR = (140, 110, 150)

HOVER_COLOR     = (190, 160, 200)
BUTTON_COLOR    = (200, 200, 200)
BUTTON_TEXT     = (0, 0, 0)

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")
        self.clock    = pygame.time.Clock()
        self.font     = pygame.font.SysFont("arial", 20)
        self.end_font = pygame.font.SysFont("arial", 36)
        self.images   = load_images(SQUARE_SIZE)

        x = WIDTH + 20
        self.undo_rect = pygame.Rect(x,  50, 120, 40)
        self.save_rect = pygame.Rect(x, 100, 120, 40)
        self.load_rect = pygame.Rect(x, 150, 120, 40)
        self.hint_rect = pygame.Rect(x, 200, 120, 40)

        self.over_window = GameOverWindow(WIDTH, SIDE_WIDTH, HEIGHT, self.font, self.end_font)

        self.vs_bot = select_mode(self.win)
        self.reset_game()

    def reset_game(self):
        self.game       = Game()
        self.selected   = None
        self.show_hints = False
        self.game_over  = False
        self.result     = ""
        self.running    = True

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.handle_events()

            if not self.game_over and self.vs_bot and self.game.turn == 'b':
                pygame.time.wait(300)
                bot_move(self.game, depth=2)
                self.check_end()

            self.draw()

        pygame.quit()
        sys.exit()

    def check_end(self):
        if self.game.is_checkmate():
            winner = 'White' if self.game.turn == 'b' else 'Black'
            self.result    = f"{winner} wins"
            self.game_over = True
        elif self.game.is_stalemate():
            self.result    = "Draw"
            self.game_over = True

    def draw(self):
        self.win.fill(BG_COLOR)
        draw_board(self.win, SQUARE_SIZE)
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE)

        if self.game.selected:
            r, c = self.game.selected
            pygame.draw.rect(
                self.win,
                HIGHLIGHT_COLOR,
                (c * SQUARE_SIZE, r * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE),
                4
            )

        if self.show_hints and self.game.selected:
            r0, c0 = self.game.selected
            for r, c in self.game.get_valid_moves((r0, c0)):
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HIGHLIGHT_COLOR, 100), surf.get_rect())
                self.win.blit(surf, (c * SQUARE_SIZE, r * SQUARE_SIZE))

        mx, my = pygame.mouse.get_pos()
        if mx < WIDTH:
            hr, hc = my // SQUARE_SIZE, mx // SQUARE_SIZE
            piece = self.game.board[hr][hc]
            if piece and (hr, hc) != self.game.selected and piece[0] == self.game.turn:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HOVER_COLOR, 255), surf.get_rect(), 4)
                self.win.blit(surf, (hc * SQUARE_SIZE, hr * SQUARE_SIZE))

        pygame.draw.rect(self.win, PANEL_COLOR, (WIDTH, 0, SIDE_WIDTH, HEIGHT))
        for rect, label in [
            (self.undo_rect, "Undo"),
            (self.save_rect, "Save"),
            (self.load_rect, "Load"),
            (self.hint_rect, "Hint"),
        ]:
            pygame.draw.rect(self.win, BUTTON_COLOR, rect)
            text = self.font.render(label, True, BUTTON_TEXT)
            self.win.blit(text, text.get_rect(center=rect.center))

        if self.game_over:
            self.over_window.draw(self.win, self.result)

        pygame.display.update()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False

            elif event.type == pygame.MOUSEBUTTONDOWN:
                if self.game_over:
                    action = self.over_window.handle_event(event)
                    if action == 'play_again':
                        self.reset_game()
                    elif action == 'exit':
                        self.running = False
                    continue

                x, y = event.pos
                if   self.undo_rect.collidepoint(x, y):
                    self.game.undo_move()
                elif self.save_rect.collidepoint(x, y):
                    self.game.save_game()
                elif self.load_rect.collidepoint(x, y):
                    self.game.load_game()
                elif self.hint_rect.collidepoint(x, y):
                    self.show_hints = True

                elif x < WIDTH and (not self.vs_bot or self.game.turn == 'w'):
                    row, col = y // SQUARE_SIZE, x // SQUARE_SIZE
                    self.game.handle_click((row, col))
                    self.show_hints = False
                    self.check_end()
