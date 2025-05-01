import pygame
import sys

def select_mode(win):
    font = pygame.font.SysFont("arial", 28)
    button_font = pygame.font.SysFont("arial", 24)

    option1 = pygame.Rect(200, 220, 280, 50)
    option2 = pygame.Rect(200, 300, 280, 50)

    while True:
        win.fill((230, 230, 255))  # ніжно-блакитний фон

        title = font.render("Game:", True, (50, 50, 50))
        win.blit(title, (200, 140))

        pygame.draw.rect(win, (200, 200, 255), option1, border_radius=8)
        pygame.draw.rect(win, (200, 200, 255), option2, border_radius=8)
        pygame.draw.rect(win, (100, 100, 200), option1, 2, border_radius=8)
        pygame.draw.rect(win, (100, 100, 200), option2, 2, border_radius=8)

        text1 = button_font.render("Play 1 to 1", True, (0, 0, 0))
        text2 = button_font.render("Play witch PC", True, (0, 0, 0))
        win.blit(text1, (option1.x + 50, option1.y + 12))
        win.blit(text2, (option2.x + 50, option2.y + 12))

        pygame.display.update()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                x, y = pygame.mouse.get_pos()
                if option1.collidepoint(x, y):
                    return False  # режим 1 на 1
                elif option2.collidepoint(x, y):
                    return True  # проти компʼютера
