import sys
import pygame
from game import Game
from utils import load_images, draw_board, draw_pieces
from mode_select import select_mode
from bot import bot_move
from network import GameServer, GameClient
from game_over_window import GameOverWindow

WIDTH, HEIGHT = 640, 640
SIDE_WIDTH = 160
SQUARE_SIZE = WIDTH // 8

BG_COLOR = (255, 255, 255)
PANEL_COLOR = (255, 255, 255)
HIGHLIGHT_COLOR = (140, 110, 150)
HOVER_COLOR = (190, 160, 200)
BUTTON_COLOR = (200, 200, 200)
BUTTON_TEXT = (0, 0, 0)
HINT_ACTIVE_BG = (180, 220, 180)
MOVE_BG = (220, 220, 220)
MOVE_BG_ACTIVE = (180, 180, 200)
LAST_BLACK_BG = (185, 185, 200)

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        
        mode_title = ""
        res = select_mode(self.win)
        if isinstance(res, tuple):
            self.mode, self.net_param = res
        else:
            self.mode = "bot" if res else "local"
            self.net_param = None

        if self.mode == "local":
            mode_title = " (1-on-1)"
        elif self.mode == "bot":
            mode_title = " (vs PC)"
        elif self.mode in ["net_host", "net_client"]:
            mode_title = " (Network)"
        pygame.display.set_caption(f"Chess{mode_title}")

        self.clock = pygame.time.Clock()
        self.font = pygame.font.SysFont("arial", 20)
        self.end_font = pygame.font.SysFont("arial", 36)
        self.images = load_images(SQUARE_SIZE)
        self.small_images = {
            k: pygame.transform.scale(v, (SQUARE_SIZE//2, SQUARE_SIZE//2))
            for k, v in self.images.items()
        }
        x = WIDTH + 20
        self.undo_rect = pygame.Rect(x, 50, 120, 40)
        self.save_rect = pygame.Rect(x, 100, 120, 40)
        self.load_rect = pygame.Rect(x, 150, 120, 40)
        self.hint_rect = pygame.Rect(x, 200, 120, 40)
        self.over_window = GameOverWindow(WIDTH, SIDE_WIDTH, HEIGHT, self.font, self.end_font)
        self.vs_bot = (self.mode == "bot")
        self.net = None
        if self.mode == "net_host":
            self.net = GameServer(on_move=self._on_remote_move)
            self.local_turn = 'w'
        elif self.mode == "net_client":
            self.net = GameClient(self.net_param, on_move=self._on_remote_move)
            self.local_turn = 'b'
        else:
            self.local_turn = None
        self.reset_game()

    def reset_game(self):
        self.game = Game()
        self.show_hints = False
        self.hint_squares = []
        self.game_over = False
        self.result = ""
        self.running = True

    def _on_remote_move(self, start, end):
        self.game.move_piece(start, end)
        self.check_end()

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.handle_events()
            if not self.game_over and self.vs_bot and self.game.turn == 'b':
                pygame.time.wait(300)
                bot_move(self.game)
                self.check_end()
            self.draw()
        pygame.quit()
        if self.net:
            self.net.close()
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
        if self.game.selected:
            r, c = self.game.selected
            pygame.draw.rect(self.win, HIGHLIGHT_COLOR, (c*SQUARE_SIZE, r*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE), 4)
        if self.show_hints and self.game.selected:
            for hr, hc in self.hint_squares:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HIGHLIGHT_COLOR, 100), surf.get_rect())
                self.win.blit(surf, (hc*SQUARE_SIZE, hr*SQUARE_SIZE))
        mx, my = pygame.mouse.get_pos()
        if mx < WIDTH:
            hr, hc = my//SQUARE_SIZE, mx//SQUARE_SIZE
            p = self.game.board[hr][hc]
            if p and (hr, hc) != self.game.selected and p[0] == self.game.turn:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HOVER_COLOR, 255), surf.get_rect(), 4)
                self.win.blit(surf, (hc*SQUARE_SIZE, hr*SQUARE_SIZE))
        pygame.draw.rect(self.win, PANEL_COLOR, (WIDTH, 0, SIDE_WIDTH, HEIGHT))
        for rect, label in [(self.undo_rect, "Undo"), (self.save_rect, "Save"), (self.load_rect, "Load")]:
            pygame.draw.rect(self.win, BUTTON_COLOR, rect)
            txt = self.font.render(label, True, BUTTON_TEXT)
            self.win.blit(txt, txt.get_rect(center=rect.center))
        hint_bg = HINT_ACTIVE_BG if self.show_hints else BUTTON_COLOR
        pygame.draw.rect(self.win, hint_bg, self.hint_rect)
        txt = self.font.render("Hint", True, BUTTON_TEXT)
        self.win.blit(txt, txt.get_rect(center=self.hint_rect.center))

        move_log = self.game.move_log
        font = self.font
        x0 = WIDTH + 10
        y0 = self.hint_rect.bottom + 20
        row_h = 28
        col_w = 120

        for idx, move in enumerate(move_log):
            y = y0 + idx * row_h
            move_number = idx + 1 
            if idx % 2 == 1:
                pygame.draw.rect(self.win, (190, 190, 210), (x0, y, col_w, row_h))
            num_txt = font.render(f"{move_number}.", True, (0, 0, 0))
            self.win.blit(num_txt, (x0, y + 4))
            start, end, piece, _ = move
            move_alg = f"{chr(start[1]+97)}{8-start[0]} - {chr(end[1]+97)}{8-end[0]}"
            txt = font.render(move_alg, True, (0, 0, 0))
            self.win.blit(txt, (x0 + 35, y + 4))

        if self.game_over:
            self.over_window.draw(self.win, self.result)
        pygame.display.update()

    def handle_events(self):
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                self.running = False
            elif ev.type == pygame.MOUSEBUTTONDOWN:
                if self.game_over:
                    action = self.over_window.handle_event(ev)
                    if action == 'play_again':
                        self.reset_game()
                    elif action == 'exit':
                        self.running = False
                    continue
                x, y = ev.pos
                if self.undo_rect.collidepoint(x, y):
                    self.game.undo_move()
                elif self.save_rect.collidepoint(x, y):
                    self.game.save_game()
                elif self.load_rect.collidepoint(x, y):
                    self.game.load_game()
                elif self.hint_rect.collidepoint(x, y):
                    self.show_hints = not self.show_hints
                    if self.show_hints and self.game.selected:
                        self.hint_squares = self.game.get_valid_moves(self.game.selected)
                    else:
                        self.hint_squares = []
                elif x < WIDTH and not self.game_over:
                    allowed = (self.mode=="local" or (self.vs_bot and self.game.turn=='w') or (self.net and self.game.turn==self.local_turn))
                    if allowed:
                        r, c = y//SQUARE_SIZE, x//SQUARE_SIZE
                        if self.game.selected is None:
                            piece = self.game.board[r][c]
                            if piece and piece[0] == self.game.turn:
                                self.game.selected = (r, c)
                                if self.show_hints:
                                    self.hint_squares = self.game.get_valid_moves(self.game.selected)
                                else:
                                    self.hint_squares = []
                        else:
                            if (r, c) in self.game.get_valid_moves(self.game.selected):
                                prev = self.game.selected
                                self.game.handle_click((r, c))
                                self.check_end()
                                self.hint_squares = []
                                if self.net and prev is not None and self.game.selected is None:
                                    self.net.send_move(prev, (r, c))
                            elif self.game.board[r][c] and self.game.board[r][c][0] == self.game.turn:
                                self.game.selected = (r, c)
                                if self.show_hints:
                                    self.hint_squares = self.game.get_valid_moves(self.game.selected)
                                else:
                                    self.hint_squares = []
                            else:
                                self.game.selected = None
                                self.hint_squares = []

