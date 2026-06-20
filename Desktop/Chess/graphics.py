import sys
import time

import pygame

from bot import bot_move
from file_dialogs import ask_load_filename, ask_save_filename
from game import Game
from game_over_window import GameOverWindow
from mode_select import select_mode
from mouse_utils import (
    get_board_square,
    has_pawn_reached_last_rank,
    is_board_click,
    is_move_allowed,
)
from network import GameClient, GameServer
from promotion_dialog import prompt_promotion as ask_promotion
from scroll_utils import (
    calculate_scroll_from_drag,
    get_max_scroll,
    update_scroll_from_wheel,
)
from sidebar import draw_sidebar
from sound_manager import SoundManager
from ui_overlays import (
    draw_hint_squares,
    draw_hover_square,
    draw_illegal_move_overlay,
    draw_last_move_highlight,
    draw_selected_square,
    draw_wait_overlay,
    should_show_timed_message,
)
from utils import draw_board, draw_pieces, load_images

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


class ChessApp:
    def __init__(self):
        pygame.init()
        self.win = pygame.display.set_mode((WIDTH + SIDE_WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")
        self.sounds = SoundManager()
        
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
        self._pending_send_network = False
        self.bot_moved = False
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
        self.sounds.play_start()
        self._promo    = None
        self._pending_send_network = False
        self.bot_moved = False

    def _on_remote_move(self, start, end, promotion=None):
        piece = self.game.board[start[0]][start[1]]
        captured = self.game.board[end[0]][end[1]]

        ok = self.game.move_piece(start, end, promotion=promotion)
        if ok:
            is_check = self.game.is_in_check(self.game.turn)
            self._play_move_sound(piece, captured, is_check)
            self.check_end()
            self.auto_scroll()

    def run(self):
        self.running = True
        while self.running:
            self.clock.tick(60)
            if self.vs_bot and self.game.turn == 'w':
                self.bot_moved = False

            self.handle_events()
            self.check_timeout()
            bot_should_move = (
                self.vs_bot
                and not self.game_over
                and self.game.turn == "b"
                and not self.bot_moved
                and not self.just_undid
            )
            if self.animating:
                self.anim_progress += 0.03
                if self.anim_progress >= 1.0:
                    self.animating = False
                    self.ui_locked = False
                    self.anim_progress = 0.0
                    if self._pending_move:
                        prev, dst = self._pending_move
                        piece = self.game.board[prev[0]][prev[1]]
                        captured = self.game.board[dst[0]][dst[1]]

                        if self._promo == 'ask':
                            actual_promo = self.prompt_promotion(prev, dst)
                            pygame.event.clear()
                        else:
                            actual_promo = self._promo

                        ok = self.game.move_piece(prev, dst, promotion=actual_promo)
                        if ok:
                            is_check = self.game.is_in_check(self.game.turn)
                            self._play_move_sound(piece, captured, is_check)

                            if self.net and self._pending_send_network:
                                self._send_network_move(prev, dst, actual_promo)

                            self.check_end()
                            self.auto_scroll()

                        self._pending_move = None
                        self._promo = None
                        self._pending_send_network = False

                    self.anim_move = None

            elif bot_should_move:
                pygame.time.wait(300)
                mv = bot_move(self.game)
                if mv:
                    promo = mv[2] if len(mv) > 2 else None
                    piece = self.game.board[mv[0][0]][mv[0][1]]
                    piece_img = self.images.get(piece)
                    self._pending_move = (mv[0], mv[1])
                    self._promo = promo
                    self._pending_send_network = False
                    self.start_animation(mv[0], mv[1], piece_img)
                self.bot_moved = True

            self.draw()
            self.just_undid = False
        if hasattr(self, "game") and self.game is not None:
            try:
                self.game.save_game("autosave.json")
                print("Game automatically saved to autosave.json")
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
                self.sounds.play_checkmate()
            elif "draw" in msg or "Draw" in msg:
                self.sounds.play_move()
            else:
                self.sounds.play_move()
            
    def check_timeout(self):
        if self.game_over:
            return

        if not self.game.time_control or self.game.time_control[0] <= 0:
            return

        current_color = self.game.turn
        if self.game.get_time_left(current_color) > 0:
            return

        self.game.time_remaining[current_color] = 0

        winner = "Black" if current_color == "w" else "White"
        self.result = f"Time out! {winner} wins."
        self.game_over = True
        self.game.game_over = True
        self.game.game_over_message = self.result

        self.sounds.play_move()

    def _play_move_sound(self, piece, captured, is_check=False):
        if (
            captured
            and piece
            and isinstance(captured, str)
            and isinstance(piece, str)
            and len(captured) > 1
            and len(piece) > 1
            and captured[0] != piece[0]
        ):
            self.sounds.play_capture()
        elif is_check:
            self.sounds.play_check()
        else:
            self.sounds.play_move()

    def _send_network_move(self, start, end, promotion=None):
        if promotion:
            try:
                self.net.send_move(start, end, promotion=promotion)
                return
            except TypeError:
                pass
        self.net.send_move(start, end)

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
        draw_last_move_highlight(self.win, self.game.move_log, SQUARE_SIZE)
        draw_pieces(self.win, self.game.board, self.images, SQUARE_SIZE, skip_piece=skip_piece)
        draw_selected_square(
            self.win,
            self.game.selected,
            SQUARE_SIZE,
            HIGHLIGHT_COLOR,
        )
        draw_hint_squares(
            self.win,
            self.show_hints,
            self.game.selected,
            self.hint_squares,
            SQUARE_SIZE,
            HIGHLIGHT_COLOR,
        )
        draw_hover_square(
            self.win,
            self.game.board.board,
            self.game.selected,
            self.game.turn,
            SQUARE_SIZE,
            WIDTH,
            HOVER_COLOR,
            mouse_pos=(mx, my),
        )
        (
            self.moves_scroll,
            self._scrollbar_rect,
            self._scrollbar_area,
        ) = draw_sidebar(
            win=self.win,
            font=self.font,
            game=self.game,
            width=WIDTH,
            side_width=SIDE_WIDTH,
            height=HEIGHT,
            hint_rect=self.hint_rect,
            undo_rect=self.undo_rect,
            save_rect=self.save_rect,
            load_rect=self.load_rect,
            show_side_rect=self.show_side_rect,
            last_clicked_button_rect=self.last_clicked_button_rect,
            undo_hover=self.undo_hover,
            save_hover=self.save_hover,
            load_hover=self.load_hover,
            hint_hover=self.hint_hover,
            show_hints=self.show_hints,
            moves_scroll=self.moves_scroll,
            max_visible_moves=self.max_visible_moves,
            panel_color=PANEL_COLOR,
            button_color=BUTTON_COLOR,
            button_text=BUTTON_TEXT,
            hint_active_bg=HINT_ACTIVE_BG,
            active_color=HIGHLIGHT_COLOR,
        )

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
            draw_wait_overlay(self.win, self.font, WIDTH, HEIGHT, SIDE_WIDTH)
        if self.ui_locked and time.time() - self.ui_locked_time > 2:
            self.ui_locked = False          

        if self.illegal_move_message:
            now = pygame.time.get_ticks()
            if should_show_timed_message(self.illegal_move_time, now):
                draw_illegal_move_overlay(self.win, self.illegal_move_message)
                pygame.event.clear()
            else:
                self.illegal_move_message = ""

        pygame.display.update()

    def handle_events(self):
        if self.ui_locked:
            return
        if self.game_over:
            for event in pygame.event.get():
                action = self.over_window.handle_event(event)
                if action == 'play_again':
                    self.reset_game()
                    self.game_over = False
                elif action == 'exit':
                    self.running = False
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

                    filename = ask_save_filename()
                    if filename:
                        self.game.save_game(filename)
                    self.ui_locked = False
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

                    filename = ask_load_filename()
                    if filename:
                        self.game.load_game(filename)
                        self.game_over = getattr(self.game, 'game_over', False)
                        self.result = getattr(self.game, 'game_over_message', "")
                        self.check_end()
                        self.auto_scroll()
                    self.ui_locked = False
                    continue
                
                elif self.hint_rect.collidepoint(x, y):
                    self.show_hints = not self.show_hints
                    if self.show_hints and self.game.selected:
                        self.hint_squares = self.game.get_legal_moves(self.game.selected)
                    else:
                        self.hint_squares = []
                elif is_board_click(ev.pos, WIDTH) and not self.game_over:
                    allowed = is_move_allowed(
                        mode=self.mode,
                        vs_bot=self.vs_bot,
                        game_turn=self.game.turn,
                        net=self.net,
                        local_turn=self.local_turn,
                    )
                    if allowed:
                        r, c = get_board_square(ev.pos, SQUARE_SIZE)
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
                                reached_last = has_pawn_reached_last_rank(piece, r)
                                piece_img = self.images.get(piece)
                                self._pending_move = (prev, (r, c))
                                self._pending_send_network = bool(self.net)
                                if reached_last:
                                    self._promo = 'ask'
                                else:
                                    self._promo = None
                                self.start_animation(prev, (r, c), piece_img)
                                self.game.selected = None
                                self.hint_squares = []
                                self.auto_scroll()
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
                    max_scroll = get_max_scroll(total_moves, self.max_visible_moves)

                    if total_moves > self.max_visible_moves:
                        scroll_h = max(
                            int(bar_h * self.max_visible_moves / total_moves),
                            20,
                        )
                        self.moves_scroll = calculate_scroll_from_drag(
                            mouse_y=ev.pos[1],
                            bar_y=bar_y,
                            bar_h=bar_h,
                            scroll_h=scroll_h,
                            drag_offset=getattr(self, "_drag_offset", 0),
                            max_scroll=max_scroll,
                        )
            elif ev.type == pygame.MOUSEWHEEL:
                max_scroll = get_max_scroll(
                    len(self.game.move_log),
                    self.max_visible_moves,
                )
                self.moves_scroll = update_scroll_from_wheel(
                    current_scroll=self.moves_scroll,
                    wheel_y=ev.y,
                    max_scroll=max_scroll,
                )
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
        return ask_promotion(
            win=self.win,
            clock=self.clock,
            draw_callback=self.draw,
            board=self.game.board,
            images=self.images,
            prev=prev,
            board_width=WIDTH,
            board_height=HEIGHT,
            side_width=SIDE_WIDTH,
            square_size=SQUARE_SIZE,
            hover_color=HOVER_COLOR,
        )
