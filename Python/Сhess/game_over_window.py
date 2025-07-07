import pygame

BUTTON_BG        = (200, 200, 200)
BUTTON_TEXT      = (  0,   0,   0)
HIGHLIGHT_COLOR  = (130, 100, 160)
HOVER_COLOR      = (190, 160, 200)

class GameOverWindow:
    def __init__(self, width, side_width, height, font, end_font=None):
        self.width = width
        self.side_width = side_width
        self.height = height
        self.font = font
        self.end_font = end_font or pygame.font.SysFont("arial", 36)
        
        self.play_again_rect = pygame.Rect(
            self.width // 2 - 140,
            self.height // 2 + 10,
            120,
            40
        )
        self.exit_rect = pygame.Rect(
            self.width // 2 + 20,
            self.height // 2 + 10,
            120,
            40
        )
        self.play_again_hover = False
        self.exit_hover = False
        self.play_again_active = False
        self.exit_active = False

    def draw(self, win, result_text):
        text_surf = self.end_font.render(result_text, True, (0, 0, 0))
        text_rect = text_surf.get_rect(center=(self.width // 2, self.height // 2 - 60))
        win.blit(text_surf, text_rect)

        play_bg = HOVER_COLOR if self.play_again_hover else BUTTON_BG
        play_border = HIGHLIGHT_COLOR if (self.play_again_hover or self.play_again_active) else BUTTON_TEXT
        border_width = 4 if self.play_again_active else 2
        pygame.draw.rect(win, play_bg, self.play_again_rect, border_radius=8)
        pygame.draw.rect(win, play_border, self.play_again_rect, border_width, border_radius=8)
        pa_text = self.font.render("Play Again", True, BUTTON_TEXT)
        win.blit(pa_text, pa_text.get_rect(center=self.play_again_rect.center))

        exit_bg = HOVER_COLOR if self.exit_hover else BUTTON_BG
        exit_border = HIGHLIGHT_COLOR if (self.exit_hover or self.exit_active) else BUTTON_TEXT
        border_width_ex = 4 if self.exit_active else 2
        pygame.draw.rect(win, exit_bg, self.exit_rect, border_radius=8)
        pygame.draw.rect(win, exit_border, self.exit_rect, border_width_ex, border_radius=8)
        ex_text = self.font.render("Exit", True, BUTTON_TEXT)
        win.blit(ex_text, ex_text.get_rect(center=self.exit_rect.center))

    def handle_event(self, event):
        if event.type == pygame.MOUSEMOTION:
            self.play_again_hover = self.play_again_rect.collidepoint(event.pos)
            self.exit_hover = self.exit_rect.collidepoint(event.pos)
        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            if self.play_again_rect.collidepoint(event.pos):
                self.play_again_active = True
            if self.exit_rect.collidepoint(event.pos):
                self.exit_active = True
        elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            if self.play_again_active and self.play_again_rect.collidepoint(event.pos):
                self.play_again_active = False
                return 'play_again'
            if self.exit_active and self.exit_rect.collidepoint(event.pos):
                self.exit_active = False
                return 'exit'
            self.play_again_active = False
            self.exit_active = False
        return None
