import pygame
import os

def load_images(square_size):
    pieces = ['bR', 'bN', 'bB', 'bQ', 'bK', 'bP',
              'wR', 'wN', 'wB', 'wQ', 'wK', 'wP']
    images = {}
    for piece in pieces:
        path = os.path.join("images", f"{piece}.png")
        images[piece] = pygame.transform.scale(
            pygame.image.load(path),
            (square_size, square_size)
        )
    return images

def draw_board(win, square_size):
    white = (240, 217, 181)
    brown = (181, 136, 99)
    font = pygame.font.SysFont("arial", 22)
    letters = "abcdefgh"
    numbers = "87654321" 

    for row in range(8):
        for col in range(8):
            color = white if (row + col) % 2 == 0 else brown
            pygame.draw.rect(win, color, (col * square_size, row * square_size, square_size, square_size))

            if col == 0:
                number = font.render(numbers[row], True, (100, 100, 100))
                x = 5
                y = row * square_size + 3 
                win.blit(number, (x, y))

            if row == 7:
                letter = font.render(letters[col], True, (100, 100, 100))
                x = col * square_size + square_size - letter.get_width() - 5
                y = 8 * square_size - letter.get_height() - 2
                win.blit(letter, (x, y))

def draw_pieces(win, board, images, square_size):
    for row in range(8):
        for col in range(8):
            piece = board[row][col]
            if piece != '':
                win.blit(images[piece], (col * square_size, row * square_size))

def draw_buttons(win):
    font = pygame.font.SysFont("arial", 20)
    pygame.draw.rect(win, (200, 200, 200), (660, 50, 120, 40))
    pygame.draw.rect(win, (200, 200, 200), (660, 100, 120, 40))
    pygame.draw.rect(win, (200, 200, 200), (660, 150, 120, 40))
    win.blit(font.render("Undo", True, (0, 0, 0)), (690, 60))
    win.blit(font.render("Save", True, (0, 0, 0)), (690, 110))
    win.blit(font.render("Load", True, (0, 0, 0)), (690, 160))
