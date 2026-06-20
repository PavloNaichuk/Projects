import sys

import pygame
from PIL import Image, ImageFilter

PROMOTION_OPTIONS = ("Q", "R", "B", "N")


def gaussian_blur_surface(surface, radius=4):
    raw_string = pygame.image.tostring(surface, "RGBA")
    image = Image.frombytes("RGBA", surface.get_size(), raw_string)
    image = image.filter(ImageFilter.GaussianBlur(radius))

    return pygame.image.fromstring(image.tobytes(), image.size, "RGBA")


def build_promotion_rects(board_width, board_height, square_size):
    panel_width = square_size * len(PROMOTION_OPTIONS)
    panel_height = square_size
    x0 = board_width // 2 - panel_width // 2
    y0 = board_height // 2 - panel_height // 2

    return [
        pygame.Rect(x0 + index * square_size, y0, square_size, square_size)
        for index in range(len(PROMOTION_OPTIONS))
    ]


def get_promotion_choice_from_position(position, rects):
    for index, rect in enumerate(rects):
        if rect.collidepoint(position):
            return PROMOTION_OPTIONS[index]

    return None


def prompt_promotion(
    win,
    clock,
    draw_callback,
    board,
    images,
    prev,
    board_width,
    board_height,
    side_width,
    square_size,
    hover_color,
):
    draw_callback()
    background = win.copy()

    color = board[prev[0]][prev[1]][0]
    images_for_options = [images[color + option] for option in PROMOTION_OPTIONS]
    rects = build_promotion_rects(board_width, board_height, square_size)

    panel_width = square_size * len(PROMOTION_OPTIONS)
    panel_height = square_size
    panel_x = board_width // 2 - panel_width // 2
    panel_y = board_height // 2 - panel_height // 2

    while True:
        board_background = background.subsurface((0, 0, board_width, board_height)).copy()
        blurred = gaussian_blur_surface(board_background, radius=3)
        win.blit(blurred, (0, 0))

        side_panel_background = background.subsurface(
            (board_width, 0, side_width, board_height)
        ).copy()
        win.blit(side_panel_background, (board_width, 0))

        background_rect = pygame.Rect(
            panel_x - 5,
            panel_y - 5,
            panel_width + 10,
            panel_height + 10,
        )
        panel_surface = pygame.Surface(
            (background_rect.width, background_rect.height),
            pygame.SRCALPHA,
        )
        panel_surface.fill((210, 180, 140, 200))
        win.blit(panel_surface, (background_rect.x, background_rect.y))

        mouse_position = pygame.mouse.get_pos()
        for image, rect in zip(images_for_options, rects, strict=True):
            if rect.collidepoint(mouse_position):
                highlight = pygame.Surface((square_size, square_size), pygame.SRCALPHA)
                highlight.fill((210, 180, 140, 150))
                win.blit(highlight, rect.topleft)
                pygame.draw.rect(win, hover_color, rect, 3)

            win.blit(image, rect)

        pygame.display.update()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                choice = get_promotion_choice_from_position(event.pos, rects)
                if choice is not None:
                    return choice

        clock.tick(30)
