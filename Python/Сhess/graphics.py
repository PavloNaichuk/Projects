import sys 
import pygame
from PIL import Image, ImageFilter
import tkinter as tk
from tkinter import filedialog
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

def gaussian_blur_surface(surf, radius=4):
    raw_str = pygame.image.tostring(surf, 'RGBA')
    img = Image.frombytes('RGBA', surf.get_size(), raw_str)
    img = img.filter(ImageFilter.GaussianBlur(radius))
    return pygame.image.fromstring(img.tobytes(), img.size, 'RGBA')

class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")
        
        self.show_side_rect = False
        self.side_rect_time = 0
        self.last_clicked_button_rect = None
        
        self.ui_locked = False
        self.ui_locked_time = 0
        self.bot_moved = False
        
        self.illegal_move_time = 0
        self.illegal_move_message = ""
        self.selected = None
        self.just_undid = False
        
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
        
        self.undo_hover = False
        self.save_hover = False
        self.load_hover = False
        self.hint_hover = False 
        self.status_message = None
        self.status_time = 0.0

        self._promo    = None
        self.bot_moved = False
        self.reset_game()
    def ask_save_filename(self):
        root = tk.Tk()
        root.withdraw()
        filename = filedialog.asksaveasfilename(
            defaultextension=".sav",
            filetypes=[("Pickle files","*.sav"),("All files","*.*")]
        )
        root.destroy()
        return filename

    def ask_load_filename(self):
        root = tk.Tk()
        root.withdraw()
        filename = filedialog.askopenfilename(
            defaultextension=".sav",
            filetypes=[("Pickle files","*.sav"),("All files","*.*")]
        )
        root.destroy()
        return filename
    
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
        self._promo    = None
        self.bot_moved = False

    def _on_remote_move(self, start, end, promotion=None):
        ok = self.game.move_piece(start, end, promotion=promotion)
        if ok:
            self.check_end()
            self.auto_scroll()

    def run(self):
        self.running = True
        while self.running:
            self.clock.tick(60)
            if self.vs_bot and self.game.turn == 'w':
                self.bot_moved = False

            self.handle_events()

            if self.animating:
                self.anim_progress += 0.03
                if self.anim_progress >= 1.0:
                    self.animating = False
                    self.ui_locked = False
                    self.anim_progress = 0.0
                    if self._pending_move:
                        prev, dst = self._pending_move
                        piece    = self.game.board[prev[0]][prev[1]]
                        captured = self.game.board[dst[0]][dst[1]]
                        self._play_move_sound(prev, dst, piece, captured)

                        if self._promo == 'ask':
                            promo = self.prompt_promotion(prev, dst)
                            pygame.event.clear()
                            ok = self.game.move_piece(prev, dst, promotion=promo)
                        else:
                            ok = self.game.move_piece(prev, dst, promotion=self._promo)
                        if ok:
                            self.check_end()
                            self.auto_scroll()
                        self._pending_move = None
                        self._promo        = None

                    self.anim_move = None

            elif self.vs_bot and not self.game_over and self.game.turn == 'b' and not self.bot_moved and not self.just_undid:
                pygame.time.wait(300)
                mv = bot_move(self.game)
                if mv:
                    promo = mv[2] if len(mv) > 2 else None
                    piece = self.game.board[mv[0][0]][mv[0][1]]
                    piece_img = self.images.get(piece)
                    self._pending_move = (mv[0], mv[1])
                    self._promo = promo
                    self.start_animation(mv[0], mv[1], piece_img)
                self.bot_moved = True

            self.draw()
            self.just_undid = False
        if hasattr(self, "game") and self.game is not None:
            try:
                self.game.save_game("autosave.sav")
                print("Game automatically saved to autosave.sav")
            except Exception as e:
                print("Failed to autosave:", e)
        pygame.quit()
        if self.net:
            self.net.close()
        sys.exit()
        
    def check_end(self):
        is_over = getattr(self.game, 'game_over', False)
        msg = getattr(self.game, 'game_over_message', "")
        if is_over:
            self.result = msg
            self.game_over = True
            if "Checkmate" in msg:
                checkmate_sound.play()
            elif "draw" in msg or "Draw" in msg:
                move_sound.play()
            else:
                move_sound.play()
            
    def _play_move_sound(self, start, end, piece, captured):
        if (captured and piece
                and isinstance(captured, str) and isinstance(piece, str)
                and len(captured) > 1 and len(piece) > 1
                and captured[0] != piece[0]):
            capture_sound.play()
        elif piece and isinstance(piece, str) and len(piece) > 1 and self.game.is_in_check(self.game.turn):
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
        mx, my = pygame.mouse.get_pos()
        self.undo_hover = self.undo_rect.collidepoint((mx, my))
        self.save_hover = self.save_rect.collidepoint((mx, my))
        self.load_hover = self.load_rect.collidepoint((mx, my))
        self.hint_hover = self.hint_rect.collidepoint(mx, my)
        skip_piece = None
        if self.animating and self.anim_move:
            skip_piece = self.anim_move[0]
        if self.game.move_log:
            last_move = self.game.move_log[-1]
            start, end = last_move[0], last_move[1]
            highlight_surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
            highlight_surf.fill((160, 110, 60))
            self.win.blit(highlight_surf, (start[1] * SQUARE_SIZE, start[0] * SQUARE_SIZE))
            self.win.blit(highlight_surf, (end[1] * SQUARE_SIZE, end[0] * SQUARE_SIZE))
            
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE, skip_piece=skip_piece)
        pygame.draw.rect(self.win, PANEL_COLOR, (WIDTH, 0, SIDE_WIDTH, HEIGHT))

        if self.game.time_control and self.game.time_control[0] > 0:
            t_w = self.game.get_time_left('w')
            mins, secs = divmod(int(t_w), 60)
            txt = self.font.render(f"White: {mins:02d}:{secs:02d}", True, (0,0,0))
            self.win.blit(txt, (WIDTH+20, 20))

            t_b = self.game.get_time_left('b')
            mins_b, secs_b = divmod(int(t_b), 60)
            txt_b = self.font.render(f"Black: {mins_b:02d}:{secs_b:02d}", True, (0,0,0))
            self.win.blit(txt_b, (WIDTH+20, self.hint_rect.bottom + 20))
        
        timer_y = self.hint_rect.bottom + 20
        y0 = timer_y + 30
        
        undo_bg = (130, 100, 160) if self.show_side_rect and self.last_clicked_button_rect == self.undo_rect else BUTTON_COLOR
        undo_border = (130, 100, 160) if self.undo_hover else (0, 0, 0)
        pygame.draw.rect(self.win, undo_bg, self.undo_rect, border_radius=8)
        pygame.draw.rect(self.win, undo_border, self.undo_rect, 2, border_radius=8)
        txt_undo = self.font.render("Undo", True, BUTTON_TEXT)
        self.win.blit(txt_undo, txt_undo.get_rect(center=self.undo_rect.center))

        save_bg = (130, 100, 160) if self.show_side_rect and self.last_clicked_button_rect == self.save_rect else BUTTON_COLOR
        save_border = (130, 100, 160) if self.save_hover else (0, 0, 0)
        pygame.draw.rect(self.win, save_bg, self.save_rect, border_radius=8)
        pygame.draw.rect(self.win, save_border, self.save_rect, 2, border_radius=8)
        txt_save = self.font.render("Save", True, BUTTON_TEXT)
        self.win.blit(txt_save, txt_save.get_rect(center=self.save_rect.center))


        load_bg = (130, 100, 160) if self.show_side_rect and self.last_clicked_button_rect == self.load_rect else BUTTON_COLOR
        load_border = (130, 100, 160) if self.load_hover else (0, 0, 0)
        pygame.draw.rect(self.win, load_bg, self.load_rect, border_radius=8)
        pygame.draw.rect(self.win, load_border, self.load_rect, 2, border_radius=8)
        txt_load = self.font.render("Load", True, BUTTON_TEXT)
        self.win.blit(txt_load, txt_load.get_rect(center=self.load_rect.center))


        hint_bg = HINT_ACTIVE_BG if self.show_hints else BUTTON_COLOR
        hint_border = (130, 100, 160) if self.hint_hover else (0, 0, 0)
        pygame.draw.rect(self.win, hint_bg, self.hint_rect, border_radius=8)
        pygame.draw.rect(self.win, hint_border, self.hint_rect, 2, border_radius=8)
        txt_hint = self.font.render("Hint", True, BUTTON_TEXT)
        self.win.blit(txt_hint, txt_hint.get_rect(center=self.hint_rect.center))

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
            p = self.game.board.board[hr][hc]
            if p and (hr, hc) != self.game.selected and p[0] == self.game.turn:
                surf = pygame.Surface((SQUARE_SIZE, SQUARE_SIZE), pygame.SRCALPHA)
                pygame.draw.rect(surf, (*HOVER_COLOR, 255), surf.get_rect(), 4)
                self.win.blit(surf, (hc*SQUARE_SIZE, hr*SQUARE_SIZE))
                
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
            capture = captured is not None and captured != ''
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
        
        if self.show_side_rect:
            if time.time() - self.side_rect_time > 2:
                self.show_side_rect = False
        if self.ui_locked:
            overlay = pygame.Surface((WIDTH + SIDE_WIDTH, HEIGHT))
            overlay.set_alpha(150)
            overlay.fill((0, 0, 0))
            self.win.blit(overlay, (0, 0))
            txt = self.font.render("Please wait...", True, (255, 255, 255))
            self.win.blit(txt, txt.get_rect(center=(WIDTH // 2, HEIGHT // 2)))
        if self.ui_locked and time.time() - self.ui_locked_time > 2:
            self.ui_locked = False          
        pygame.display.update()
        
        if self.illegal_move_message:
            now = pygame.time.get_ticks()
            if now - self.illegal_move_time < 3000:
                overlay = pygame.Surface(self.win.get_size(), pygame.SRCALPHA)
                overlay.fill((0, 0, 0, 180))
                self.win.blit(overlay, (0, 0))
                font = pygame.font.SysFont("Arial", 40, bold=True)
                text = font.render(self.illegal_move_message, True, (255, 0, 0))
                rect = text.get_rect(center=(self.win.get_width() // 2, self.win.get_height() // 2))
                self.win.blit(text, rect)
                pygame.event.clear()
            else:
                self.illegal_move_message = ""

    def handle_events(self):
        if self.ui_locked:
            return
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
                    if self.vs_bot:
                        self.game.undo_move()
                    self.just_undid = True
                    self.check_end()
                    self.auto_scroll()
                    self.undo_highlight = True
                    self.status_message = "Undo"
                    self.status_time = time.time()
                    self.show_side_rect = True
                    self.side_rect_time = time.time()
                    self.last_clicked_button_rect = self.undo_rect 
                    self.ui_locked = True
                    self.ui_locked_time = time.time() 
                    continue
                elif self.save_rect.collidepoint(x, y):
                    self.save_highlight = True
                    self.status_message = "Save"
                    self.status_time = time.time()
                    self.show_side_rect = True
                    self.side_rect_time = time.time()
                    self.last_clicked_button_rect = self.save_rect
                    self.ui_locked = True
                    self.ui_locked_time = time.time()
    
                    self.draw()
                    pygame.display.update()
                    pygame.time.wait(10) 

                    filename = self.ask_save_filename()
                    if filename:
                        self.game.save_game(filename)
                    continue
                elif self.load_rect.collidepoint(x, y):
                    self.load_highlight = True
                    self.status_message = "Load"
                    self.status_time = time.time()
                    self.show_side_rect = True
                    self.side_rect_time = time.time()
                    self.last_clicked_button_rect = self.load_rect
                    self.ui_locked = True
                    self.ui_locked_time = time.time()
                    self.draw()
                    pygame.display.update()
                    pygame.time.wait(10)

                    filename = self.ask_load_filename()
                    if filename:
                        self.game.load_game(filename)
                        self.check_end()
                        self.auto_scroll()
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
                                is_pawn = piece and piece[1] == 'P'
                                reached_last = (piece and is_pawn and ((piece[0] == 'w' and r == 0) or (piece[0] == 'b' and r == 7)))
                                piece_img = self.images.get(piece)
                                self._pending_move = (prev, (r, c))
                                if is_pawn and reached_last:
                                    self._promo = 'ask'
                                else:
                                    self._promo = None
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
                                self.illegal_move_message = "You cannot move there: your king would be in check!"
                                self.illegal_move_time = pygame.time.get_ticks()
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
                    self.check_end()
                    self.auto_scroll()
    def start_animation(self, start, end, piece_img):
        self.animating = True
        self.anim_move = (start, end, piece_img)
        self.anim_progress = 0.0
        
    def prompt_promotion(self, prev, dst):
        self.draw()
        background = self.win.copy()

        color   = self.game.board[prev[0]][prev[1]][0]
        options = ['Q','R','B','N']
        imgs    = [self.images[color + opt] for opt in options]
        width   = SQUARE_SIZE * 4
        height  = SQUARE_SIZE
        x0 = WIDTH//2 - width//2
        y0 = HEIGHT//2 - height//2
        rects   = [
            pygame.Rect(x0 + i*SQUARE_SIZE, y0, SQUARE_SIZE, SQUARE_SIZE)
            for i in range(4)
        ]

        selecting = True
        choice     = 'Q'
        while selecting:
            board_bg = background.subsurface((0,0,WIDTH,HEIGHT)).copy()
            blurred = gaussian_blur_surface(board_bg, radius = 3)
            self.win.blit(blurred, (0, 0))

            panel_bg = background.subsurface((WIDTH, 0, SIDE_WIDTH, HEIGHT)).copy()
            self.win.blit(panel_bg, (WIDTH, 0))

            bg_rect   = pygame.Rect(x0-5, y0-5, width+10, height+10)
            panel_surf = pygame.Surface(
                (bg_rect.width, bg_rect.height), pygame.SRCALPHA
            )
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

            self.clock.tick(30)

        return choice
