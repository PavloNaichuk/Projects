import sys
import pygame
import time
from game import Game
from utils import load_images, draw_board, draw_pieces
from mode_select import select_mode
from bot import bot_move
from network import GameServer, GameClient
from game_over_window import GameOverWindow

WIDTH, HEIGHT = 640, 640
SIDE_WIDTH = 180
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

pygame.mixer.init()
move_sound = pygame.mixer.Sound("sounds/move.wav")
capture_sound = pygame.mixer.Sound("sounds/capture.wav")
check_sound = pygame.mixer.Sound("sounds/check.wav")
checkmate_sound = pygame.mixer.Sound("sounds/checkmate.wav")
start_sound = pygame.mixer.Sound("sounds/start.wav")

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")

        mode_title = ""
        res = select_mode(self.win)
        if isinstance(res, tuple) and len(res) == 3:
            self.mode, self.net_param, self.time_control = res
        elif isinstance(res, tuple):
            self.mode, self.net_param = res
            self.time_control = (5*60, 0)
        else:
            self.mode = "bot" if res else "local"
            self.net_param = None
            self.time_control = (5*60, 0)

        if self.mode == "local":
            mode_title = " (Play 1-on-1)"
        elif self.mode == "bot":
            mode_title = " (Play vs PC)"
        elif self.mode in ["net_host", "net_client"]:
            mode_title = " (PC Over Network)"
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
        
        self.save_highlight = False
        self.load_highlight = False
        self.undo_highlight = False
        self.highlight_time  = 0.0
        self.status_message = None
        self.status_time    = 0.0

        if self.mode == "net_host":
            self.net = GameServer(on_move=self._on_remote_move)
            self.local_turn = 'w'
        elif self.mode == "net_client":
            self.net = GameClient(self.net_param, on_move=self._on_remote_move)
            self.local_turn = 'b'
        else:
            self.local_turn = None
        self.max_visible_moves = 12
        self.moves_scroll = 0
        self.move_scroll_drag = False

        self.animating = False
        self.anim_move = None
        self.anim_progress = 0.0
        self._pending_move = None
        
        self.save_highlight = False
        self.load_highlight = False
        self.highlight_time = 0.0
        self.status_message = None
        self.status_time = 0.0

        self.reset_game()

    def reset_game(self):
        self.game = Game()
        self.game.time_control   = self.time_control
        self.game.time_remaining = {
            'w': self.time_control[0],
            'b': self.time_control[0]
        }
        self.game.move_start_time = time.time()
        self.show_hints = True
        self.hint_squares = []
        self.game_over = False
        self.result = ""
        self.running = True
        self.animating = False
        self.anim_move = None
        self.anim_progress = 0.0
        self._pending_move = None
        self.auto_scroll()
        start_sound.play()

    def _on_remote_move(self, start, end):
        self.game.move_piece(start, end)
        self.check_end()
        self.auto_scroll()

    def run(self):
        while self.running:
            self.clock.tick(60)
            self.handle_events()

            if self.animating:
                self.anim_progress += 0.03
                if self.anim_progress >= 1.0:
                    self.animating = False
                    self.anim_progress = 0.0
                    
                    if self._pending_move:
                        prev, dst = self._pending_move
                        self._play_move_sound(prev, dst)

                        piece = self.game.board[prev[0]][prev[1]]
                        is_pawn = piece[1] == 'P'
                        reached_last = (piece[0] == 'w' and dst[0] == 0) or (piece[0] == 'b' and dst[0] == 7)

                        if is_pawn and reached_last:
                            promo = self.prompt_promotion(prev, dst)
                            self.game.move_piece(prev, dst, promotion=promo)
                        else:
                            self.game.move_piece(prev, dst)

                        self.check_end()
                        self.auto_scroll()

                        self._pending_move = None

                    self.anim_move = None


            elif not self.game_over and self.vs_bot and self.game.turn == 'b':
                pygame.time.wait(300)
                bot_move(self.game)
                last_move = self.game.move_log[-1]
                self._play_move_sound(*last_move[:2])
                self.check_end()
                self.auto_scroll()

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
            checkmate_sound.play()
        elif self.game.is_stalemate():
            self.result = "Draw"
            self.game_over = True
    def _play_move_sound(self, start, end):
        piece = self.game.board[start[0]][start[1]]
        captured = self.game.board[end[0]][end[1]]
        if captured and captured[0] != piece[0]:
            capture_sound.play()
        elif self.game.is_in_check(self.game.turn):
            check_sound.play()
        else:
            move_sound.play()
    
    def auto_scroll(self):
        move_log = self.game.move_log
        n = len(move_log)
        if n > self.max_visible_moves:
            self.moves_scroll = n - self.max_visible_moves
        else:
            self.moves_scroll = 0

    def draw(self):
        self.win.fill(BG_COLOR)
        draw_board(self.win, SQUARE_SIZE)
        skip_piece = None
        if self.animating and self.anim_move:
            skip_piece = self.anim_move[0]
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE, skip_piece=skip_piece)
        pygame.draw.rect(self.win, PANEL_COLOR, (WIDTH, 0, SIDE_WIDTH, HEIGHT))

        t_w = self.game.get_time_left('w')
        mins, secs = divmod(int(t_w), 60)
        txt = self.font.render(f"White: {mins:02d}:{secs:02d}", True, (0,0,0))
        self.win.blit(txt, (WIDTH+20, 20))
        
        color_undo = HOVER_COLOR if (self.undo_highlight and (time.time() - self.highlight_time < 0.3)) else BUTTON_COLOR
        pygame.draw.rect(self.win, color_undo, self.undo_rect)
        txt_undo = self.font.render("Undo", True, BUTTON_TEXT)
        self.win.blit(txt_undo, txt_undo.get_rect(center=self.undo_rect.center))
        
        color_save = HOVER_COLOR if self.save_highlight and (time.time() - self.highlight_time < 0.3) else BUTTON_COLOR
        pygame.draw.rect(self.win, color_save, self.save_rect)
        txt_save = self.font.render("Save", True, BUTTON_TEXT)
        self.win.blit(txt_save, txt_save.get_rect(center=self.save_rect.center))
        
        if self.undo_highlight and (time.time() - self.highlight_time >= 0.3):
            self.undo_highlight = False
                
        color_load = HOVER_COLOR if self.load_highlight and (time.time() - self.highlight_time < 0.3) else BUTTON_COLOR
        pygame.draw.rect(self.win, color_load, self.load_rect)
        txt_load = self.font.render("Load", True, BUTTON_TEXT)
        self.win.blit(txt_load, txt_load.get_rect(center=self.load_rect.center))
        if self.load_highlight and (time.time() - self.highlight_time >= 0.3):
            self.load_highlight = False
            
        hint_bg = HINT_ACTIVE_BG if self.show_hints else BUTTON_COLOR
        pygame.draw.rect(self.win, hint_bg, self.hint_rect)
        txt = self.font.render("Hint", True, BUTTON_TEXT)
        self.win.blit(txt, txt.get_rect(center=self.hint_rect.center))

        if self.status_message and (time.time() - self.status_time < 2):
            msg_surf = self.font.render(self.status_message, True, (0, 0, 0))
            total_w = WIDTH + SIDE_WIDTH
            total_h = HEIGHT
            msg_rect = msg_surf.get_rect(center=(total_w // 2, total_h // 2))
            self.win.blit(msg_surf, msg_rect)
        elif self.status_message and (time.time() - self.status_time >= 2):
            self.status_message = None
        
        timer_y = self.hint_rect.bottom + 20
        t_b = self.game.get_time_left('b')
        mins, secs = divmod(int(t_b), 60)
        txt = self.font.render(f"Black: {mins:02d}:{secs:02d}", True, (0,0,0))
        self.win.blit(txt, (WIDTH+20, timer_y))    
                            
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
        hint_bg = HINT_ACTIVE_BG if self.show_hints else BUTTON_COLOR
        pygame.draw.rect(self.win, hint_bg, self.hint_rect)
        txt = self.font.render("Hint", True, BUTTON_TEXT)
        self.win.blit(txt, txt.get_rect(center=self.hint_rect.center))

        move_log = self.game.move_log
        font = self.font
        x0 = WIDTH + 10
        y0 = timer_y  + 30
        row_h = 28
        col_w = 130

        total_moves = len(move_log)
        max_scroll = max(0, total_moves - self.max_visible_moves)
        self.moves_scroll = min(self.moves_scroll, max_scroll)
        start_idx = self.moves_scroll
        end_idx = min(total_moves, start_idx + self.max_visible_moves)
        visible_moves = move_log[start_idx:end_idx]

        bar_x = x0 + col_w + 6
        bar_y = y0
        bar_w = 13
        bar_h = self.max_visible_moves * row_h

        if total_moves > self.max_visible_moves:
            pygame.draw.rect(self.win, (220, 220, 220), (bar_x, bar_y, bar_w, bar_h), border_radius=6)
            ratio = self.max_visible_moves / total_moves
            scroll_h = max(int(bar_h * ratio), 20)
            scroll_max_y = bar_y + bar_h - scroll_h
            if max_scroll == 0:
                scroll_y = bar_y
            else:
                scroll_y = int(bar_y + (bar_h - scroll_h) * (self.moves_scroll / max_scroll))
            pygame.draw.rect(self.win, (180, 180, 180), (bar_x, scroll_y, bar_w, scroll_h), border_radius=5)
            self._scrollbar_rect = pygame.Rect(bar_x, scroll_y, bar_w, scroll_h)
            self._scrollbar_area = pygame.Rect(bar_x, bar_y, bar_w, bar_h)
        else:
            self._scrollbar_rect = None
            self._scrollbar_area = None
        for idx, move in enumerate(visible_moves):
            y = y0 + idx * row_h
            move_number = start_idx + idx + 1
            if move_number % 2 == 0:
                pygame.draw.rect(self.win, (190, 190, 210), (x0, y, col_w, row_h))
            num_txt = font.render(f"{move_number}.", True, (0, 0, 0))
            self.win.blit(num_txt, (x0, y + 4))
            start, end, piece, captured = move
            capture = captured != ""
            PIECE_LETTER = {
                'P': '',
                'N': 'N',
                'B': 'B',
                'R': 'R',
                'Q': 'Q',
                'K': 'K'
            }
            piece_letter = PIECE_LETTER.get(piece[1].upper(), '')
            move_alg = f"{piece_letter}{chr(start[1]+97)}{8-start[0]} - {chr(end[1]+97)}{8-end[0]}"
            if capture:
                move_alg += " +"
            txt = font.render(move_alg, True, (0, 0, 0))
            self.win.blit(txt, (x0 + 35, y + 4))

        if self.game_over:
            self.over_window.draw(self.win, self.result)
        
        if self.animating and self.anim_move:
            (sr, sc), (er, ec), img = self.anim_move
            prog = self.anim_progress
            cx = sc * SQUARE_SIZE + (ec - sc) * SQUARE_SIZE * prog
            cy = sr * SQUARE_SIZE + (er - sr) * SQUARE_SIZE * prog
            self.win.blit(img, (cx, cy))

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
                if self._scrollbar_rect and self._scrollbar_rect.collidepoint(x, y):
                    self.move_scroll_drag = True
                    self._drag_offset = y - self._scrollbar_rect.y
                elif self.undo_rect.collidepoint(x, y):
                    self.game.undo_move()
                    self.auto_scroll()
                    self.undo_highlight = True
                    self.highlight_time = time.time()
                    self.status_message = "Undo"
                    self.status_time = time.time()
                    continue
                elif self.save_rect.collidepoint(x, y):
                    self.game.save_game()
                    self.save_highlight = True
                    self.highlight_time = time.time()
                    self.status_message = "Save"
                    self.status_time = time.time()
                    continue
                elif self.load_rect.collidepoint(x, y):
                    self.game.load_game()
                    self.auto_scroll()
                    self.load_highlight = True
                    self.highlight_time = time.time()
                    self.status_message = "Load"
                    self.status_time = time.time()
                    continue
                elif self.hint_rect.collidepoint(x, y):
                    self.show_hints = not self.show_hints
                    if self.show_hints and self.game.selected:
                        self.hint_squares = self.game.get_legal_moves(self.game.selected)
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
                                    self.hint_squares = self.game.get_legal_moves(self.game.selected)
                                else:
                                    self.hint_squares = []
                        else:
                            if (r, c) in self.game.get_legal_moves(self.game.selected):
                                prev = self.game.selected
                                piece = self.game.board[prev[0]][prev[1]]
                                piece_img = self.images.get(piece)
                                self._pending_move = (prev, (r, c))
                                self.start_animation(prev, (r, c), piece_img)
                                self.game.selected = None
                                self.hint_squares = []
                                self.auto_scroll()
                                if self.net and prev is not None:
                                    self.net.send_move(prev, (r, c))
                            elif self.game.board[r][c] and self.game.board[r][c][0] == self.game.turn:
                                self.game.selected = (r, c)
                                if self.show_hints:
                                    self.hint_squares = self.game.get_legal_moves(self.game.selected)
                                else:
                                    self.hint_squares = []
                            else:
                                self.game.selected = None
                                self.hint_squares = []
            elif ev.type == pygame.MOUSEBUTTONUP:
                self.move_scroll_drag = False
            elif ev.type == pygame.MOUSEMOTION:
                if self.move_scroll_drag and self._scrollbar_area:
                    bar_y = self._scrollbar_area.y
                    bar_h = self._scrollbar_area.height
                    total_moves = len(self.game.move_log)
                    max_scroll = max(0, total_moves - self.max_visible_moves)
                    if total_moves > self.max_visible_moves:
                        scroll_h = max(int(bar_h * self.max_visible_moves / total_moves), 20)
                        my = ev.pos[1]
                        rel = (my - bar_y - getattr(self, "_drag_offset", 0))
                        rel = max(0, min(rel, bar_h - scroll_h))
                        scroll_pos = rel / (bar_h - scroll_h) if (bar_h - scroll_h) > 0 else 0
                        self.moves_scroll = int(scroll_pos * max_scroll)
            elif ev.type == pygame.MOUSEWHEEL:
                move_log = self.game.move_log
                total_moves = len(move_log)
                max_scroll = max(0, total_moves - self.max_visible_moves)
                if ev.y > 0:
                    self.moves_scroll = max(0, self.moves_scroll - 1)
                elif ev.y < 0:
                    self.moves_scroll = min(self.moves_scroll + 1, max_scroll)
            elif ev.type == pygame.KEYDOWN:
                if ev.key == pygame.K_z and (pygame.key.get_mods() & pygame.KMOD_CTRL):
                    self.game.undo_move()
                    self.auto_scroll()

    def start_animation(self, start, end, piece_img):
        self.animating = True
        self.anim_move = (start, end, piece_img)
        self.anim_progress = 0.0
        
    def prompt_promotion(self, prev, dst):
        self.draw()
        background = self.win.copy()
        
        color = self.game.board[prev[0]][prev[1]][0]
        options = ['Q','R','B','N']
        imgs = [self.images[color + opt] for opt in options]
        width = SQUARE_SIZE * 4
        height = SQUARE_SIZE
        x0 = WIDTH//2 - width//2
        y0 = HEIGHT//2 - height//2
        rects = [pygame.Rect(x0 + i*SQUARE_SIZE, y0, SQUARE_SIZE, SQUARE_SIZE) for i in range(4)]

        selecting = True
        choice = 'Q'
        while selecting:
            self.win.blit(background, (0, 0))
            for ev in pygame.event.get():
                if ev.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                elif ev.type == pygame.MOUSEBUTTONDOWN:
                    mx, my = ev.pos
                    for i, rect in enumerate(rects):
                        if rect.collidepoint(mx, my):
                            choice = options[i]
                            selecting = False

            small   = pygame.transform.smoothscale(background, (background.get_width()//8, background.get_height()//8))
            blurred = pygame.transform.smoothscale( small, (background.get_width(), background.get_height()))
            self.win.blit(blurred, (0, 0))
            
            bg_rect = pygame.Rect(x0-5, y0-5, width+10, height+10)
            panel_surf = pygame.Surface((bg_rect.width, bg_rect.height), pygame.SRCALPHA)
            panel_surf.fill((210, 180, 140, 200)) 
            self.win.blit(panel_surf, (bg_rect.x, bg_rect.y))

            mx, my = pygame.mouse.get_pos()
            for i, img in enumerate(imgs):
                rect = rects[i]
                if rect.collidepoint(mx, my):
                    hl = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                    hl.fill((210, 180, 140, 150))  
                    self.win.blit(hl, rect.topleft)
                    pygame.draw.rect(self.win, HOVER_COLOR, rect, 3)
                self.win.blit(img, rect)

            pygame.display.update()
            self.clock.tick(30)

        return choice
