import sys
import pygame
from game import Game
from utils import load_images, draw_board, draw_pieces, draw_buttons
from mode_select import select_mode
from bot import bot_move
from game_over_window import GameOverWindow

WIDTH, HEIGHT = 640, 640
SIDE_WIDTH = 160
SQUARE_SIZE = WIDTH // 8

BG_COLOR = (255, 255, 255)
HIGHLIGHT_COLOR = (140, 110, 150)
BUTTON_COLOR = (200, 200, 200)
BUTTON_TEXT_COLOR = (0, 0, 0)

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")
        self.clock = pygame.time.Clock()
        self.font = pygame.font.SysFont("arial", 20)
        self.end_font = pygame.font.SysFont("arial", 36)
        self.images = load_images(SQUARE_SIZE)
        self.over_window = GameOverWindow(WIDTH, SIDE_WIDTH, HEIGHT, self.font, self.end_font)
        self.reset_game()
        self.vs_bot = select_mode(self.win)

    def reset_game(self):
        self.game = Game()
        self.selected = None
        self.show_hints = False
        self.game_over = False
        self.result = ""
        self.running = True

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.handle_events()

            # Після ходу гравця перевіряємо кінець гри
            if not self.game_over:
                self.check_end()

            # Якщо гра проти бота і зараз хід чорних — бот думає, потім перевіряємо кінець гри
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
            self.result = f"{winner} wins"
            self.game_over = True
        elif self.game.is_stalemate():
            self.result = "Draw"
            self.game_over = True

    def draw(self):
        self.win.fill(BG_COLOR)
        draw_board(self.win, SQUARE_SIZE)
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE)

        # Підсвічування обраної клітинки
        if self.game.selected:
            r, c = self.game.selected
            pygame.draw.rect(
                self.win,
                HIGHLIGHT_COLOR,
                (c * SQUARE_SIZE, r * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE),
                4
            )

        # Підказки ходів
        if self.show_hints and self.game.selected:
            r0, c0 = self.game.selected
            for r, c in self.game.get_valid_moves((r0, c0)):
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HIGHLIGHT_COLOR, 100), surf.get_rect())
                self.win.blit(surf, (c * SQUARE_SIZE, r * SQUARE_SIZE))

        # Підсвічування наведеної фігури
        mx, my = pygame.mouse.get_pos()
        if mx < WIDTH:
            r, c = my // SQUARE_SIZE, mx // SQUARE_SIZE
            p = self.game.board[r][c]
            if p and (r, c) != self.game.selected and p[0] == self.game.turn:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HIGHLIGHT_COLOR, 255), surf.get_rect(), 4)
                self.win.blit(surf, (c * SQUARE_SIZE, r * SQUARE_SIZE))

        # Бічні кнопки
        draw_buttons(self.win)
        pygame.draw.rect(self.win, BUTTON_COLOR, (WIDTH + 20, 50, 120, 40))
        self.win.blit(self.font.render("Undo", True, BUTTON_TEXT_COLOR), (WIDTH + 45, 60))
        pygame.draw.rect(self.win, BUTTON_COLOR, (WIDTH + 20, 100, 120, 40))
        self.win.blit(self.font.render("Save", True, BUTTON_TEXT_COLOR), (WIDTH + 45, 110))
        pygame.draw.rect(self.win, BUTTON_COLOR, (WIDTH + 20, 150, 120, 40))
        self.win.blit(self.font.render("Load", True, BUTTON_TEXT_COLOR), (WIDTH + 45, 160))
        pygame.draw.rect(self.win, BUTTON_COLOR, (WIDTH + 20, 200, 120, 40))
        self.win.blit(self.font.render("Hint", True, BUTTON_TEXT_COLOR), (WIDTH + 45, 210))

        # Якщо гра закінчена — показуємо вікно
        if self.game_over:
            self.over_window.draw(self.win, self.result)

        pygame.display.update()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False

            elif event.type == pygame.MOUSEBUTTONDOWN:
                # Якщо гра закінчена — делегуємо обробку GameOverWindow
                if self.game_over:
                    action = self.over_window.handle_event(event)
                    if action == 'play_again':
                        self.reset_game()
                    elif action == 'exit':
                        self.running = False
                    continue

                x, y = pygame.mouse.get_pos()
                # Хід гравця
                if x < WIDTH and (not self.vs_bot or self.game.turn == 'w'):
                    row, col = y // SQUARE_SIZE, x // SQUARE_SIZE
                    self.game.handle_click((row, col))
                    self.show_hints = False
                # Бічні кнопки
                elif WIDTH + 20 <= x <= WIDTH + 140:
                    if 50 <= y <= 90:
                        self.game.undo_move()
                    elif 100 <= y <= 140:
                        self.game.save_game()
                    elif 150 <= y <= 190:
                        self.game.load_game()
                    elif 200 <= y <= 240:
                        self.show_hints = True
