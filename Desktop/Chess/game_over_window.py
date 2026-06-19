import pygame

OVERLAY_COLOR = (0, 0, 0, 160)
BUTTON_BG = (200, 200, 200)
BUTTON_TEXT = (0, 0, 0)
HIGHLIGHT_COLOR = (130, 100, 160)
HOVER_COLOR = (190, 160, 200)
TEXT_COLOR = (255, 255, 255)


class GameOverWindow:
    def __init__(self, width, side_width, height, font, end_font=None):
        self.width = width
        self.side_width = side_width
        self.height = height
        self.font = font
        self.end_font = end_font or pygame.font.SysFont("arial", 36)

        button_width = 130
        button_height = 42
        gap = 30
        total_width = button_width * 2 + gap
        start_x = self.width // 2 - total_width // 2
        y = self.height // 2 + 20

        self.play_again_rect = pygame.Rect(
            start_x,
            y,
            button_width,
            button_height,
        )
        self.exit_rect = pygame.Rect(
            start_x + button_width + gap,
            y,
            button_width,
            button_height,
        )

        self.play_again_hover = False
        self.exit_hover = False
        self.play_again_active = False
        self.exit_active = False

    def draw(self, win, result_text):
        overlay = pygame.Surface(
            (self.width + self.side_width, self.height),
            pygame.SRCALPHA,
        )
        overlay.fill(OVERLAY_COLOR)
        win.blit(overlay, (0, 0))

        self._draw_result_text(win, result_text)

        self._draw_button(
            win,
            rect=self.play_again_rect,
            text="Play Again",
            hover=self.play_again_hover,
            active=self.play_again_active,
        )
        self._draw_button(
            win,
            rect=self.exit_rect,
            text="Exit",
            hover=self.exit_hover,
            active=self.exit_active,
        )

    def _draw_result_text(self, win, result_text):
        if not result_text:
            result_text = "Game over"

        max_width = self.width - 60
        lines = self._wrap_text(str(result_text), self.end_font, max_width)

        line_height = self.end_font.get_height() + 6
        total_height = len(lines) * line_height
        start_y = self.height // 2 - 80 - total_height // 2

        for i, line in enumerate(lines):
            text_surf = self.end_font.render(line, True, TEXT_COLOR)
            text_rect = text_surf.get_rect(
                center=(self.width // 2, start_y + i * line_height)
            )
            win.blit(text_surf, text_rect)

    def _draw_button(self, win, rect, text, hover=False, active=False):
        bg = HOVER_COLOR if hover else BUTTON_BG
        border_color = HIGHLIGHT_COLOR if (hover or active) else BUTTON_TEXT
        border_width = 4 if active else 2

        pygame.draw.rect(win, bg, rect, border_radius=8)
        pygame.draw.rect(win, border_color, rect, border_width, border_radius=8)

        text_surf = self.font.render(text, True, BUTTON_TEXT)
        win.blit(text_surf, text_surf.get_rect(center=rect.center))

    def _wrap_text(self, text, font, max_width):
        words = text.split()
        if not words:
            return ["Game over"]

        lines = []
        current = ""

        for word in words:
            candidate = word if not current else f"{current} {word}"

            if font.size(candidate)[0] <= max_width:
                current = candidate
            else:
                if current:
                    lines.append(current)
                current = word

        if current:
            lines.append(current)

        return lines

    def handle_event(self, event):
        if event.type == pygame.MOUSEMOTION:
            self.play_again_hover = self.play_again_rect.collidepoint(event.pos)
            self.exit_hover = self.exit_rect.collidepoint(event.pos)

        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            self.play_again_active = self.play_again_rect.collidepoint(event.pos)
            self.exit_active = self.exit_rect.collidepoint(event.pos)

        elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            if self.play_again_active and self.play_again_rect.collidepoint(event.pos):
                self._reset_active_state()
                return "play_again"

            if self.exit_active and self.exit_rect.collidepoint(event.pos):
                self._reset_active_state()
                return "exit"

            self._reset_active_state()

        elif event.type == pygame.KEYDOWN:
            if event.key in (pygame.K_RETURN, pygame.K_SPACE):
                return "play_again"

            if event.key == pygame.K_ESCAPE:
                return "exit"

        return None

    def _reset_active_state(self):
        self.play_again_active = False
        self.exit_active = False
