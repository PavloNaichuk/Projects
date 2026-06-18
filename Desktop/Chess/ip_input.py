import pygame
import sys

def get_ip_from_user():
    pygame.init()
    screen = pygame.display.set_mode((400, 200))
    pygame.display.set_caption("Enter Host IP")

    font = pygame.font.Font(None, 32)
    clock = pygame.time.Clock()
    input_box = pygame.Rect(50, 80, 300, 40)
    color_inactive = pygame.Color('gray')
    color_active = pygame.Color('purple')
    color = color_inactive
    active = False
    text = ''

    done = False
    ip_entered = None

    while not done:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                if input_box.collidepoint(event.pos):
                    active = not active
                else:
                    active = False
                color = color_active if active else color_inactive

            if event.type == pygame.KEYDOWN:
                if active:
                    if event.key == pygame.K_RETURN:
                        ip_entered = text
                        done = True
                    elif event.key == pygame.K_BACKSPACE:
                        text = text[:-1]
                    else:
                        text += event.unicode

        screen.fill((255, 255, 255))
        txt_surface = font.render(text, True, (0, 0, 0))
        width = max(300, txt_surface.get_width()+10)
        input_box.w = width

        screen.blit(txt_surface, (input_box.x+5, input_box.y+5))
        pygame.draw.rect(screen, color, input_box, 2)

        instruction = font.render("Enter Host IP and press Enter", True, (0,0,0))
        screen.blit(instruction, (50, 40))

        pygame.display.flip()
        clock.tick(30)

    pygame.quit()
    return ip_entered
