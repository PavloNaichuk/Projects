import pygame

OVERLAY_COLOR = (0, 0, 0, 180)
BUTTON_COLOR = (200, 200, 200)
BUTTON_TEXT_COLOR = (0, 0, 0)

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

    def draw(self, win, result_text):
        overlay = pygame.Surface((self.width + self.side_width, self.height), pygame.SRCALPHA)
        overlay.fill(OVERLAY_COLOR)
        win.blit(overlay, (0, 0))


        text_surf = self.end_font.render(result_text, True, (255, 255, 255))
        text_rect = text_surf.get_rect(center=(self.width // 2, self.height // 2 - 50))
        win.blit(text_surf, text_rect)

        pygame.draw.rect(win, BUTTON_COLOR, self.play_again_rect)
        pa_text = self.font.render("Play Again", True, BUTTON_TEXT_COLOR)
        win.blit(pa_text, pa_text.get_rect(center=self.play_again_rect.center))

        pygame.draw.rect(win, BUTTON_COLOR, self.exit_rect)
        ex_text = self.font.render("Exit", True, BUTTON_TEXT_COLOR)
        win.blit(ex_text, ex_text.get_rect(center=self.exit_rect.center))

        pygame.display.update()

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            if self.play_again_rect.collidepoint(event.pos):
                return 'play_again'
            if self.exit_rect.collidepoint(event.pos):
                return 'exit'
        return None
