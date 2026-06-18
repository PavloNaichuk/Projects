from pathlib import Path
import pygame


BASE_DIR = Path(__file__).resolve().parent
DEFAULT_IMAGES_DIR = BASE_DIR / "images"

LIGHT_SQUARE = (240, 217, 181)
DARK_SQUARE = (181, 136, 99)
COORD_COLOR = (100, 100, 100)

PIECES = (
    "bR", "bN", "bB", "bQ", "bK", "bP",
    "wR", "wN", "wB", "wQ", "wK", "wP",
)

_COORD_FONT_CACHE = {}


def _get_coord_font(size=22):
    if size not in _COORD_FONT_CACHE:
        _COORD_FONT_CACHE[size] = pygame.font.SysFont("arial", size)
    return _COORD_FONT_CACHE[size]


def load_images(square_size, images_dir=None):
    """
    Load and scale chess piece images.

    The path is resolved relative to this file by default, so the game can be
    started from different working directories.
    """
    images_path = Path(images_dir) if images_dir is not None else DEFAULT_IMAGES_DIR
    images = {}

    for piece in PIECES:
        path = images_path / f"{piece}.png"

        if not path.exists():
            raise FileNotFoundError(f"Missing chess piece image: {path}")

        image = pygame.image.load(str(path)).convert_alpha()
        images[piece] = pygame.transform.smoothscale(
            image,
            (square_size, square_size),
        )

    return images


def draw_board(win, square_size):
    font = _get_coord_font()
    letters = "abcdefgh"
    numbers = "87654321"

    for row in range(8):
        for col in range(8):
            color = LIGHT_SQUARE if (row + col) % 2 == 0 else DARK_SQUARE
            rect = pygame.Rect(
                col * square_size,
                row * square_size,
                square_size,
                square_size,
            )
            pygame.draw.rect(win, color, rect)

            if col == 0:
                number = font.render(numbers[row], True, COORD_COLOR)
                win.blit(number, (rect.x + 5, rect.y + 3))

            if row == 7:
                letter = font.render(letters[col], True, COORD_COLOR)
                x = rect.right - letter.get_width() - 5
                y = rect.bottom - letter.get_height() - 2
                win.blit(letter, (x, y))


def draw_pieces(win, board, images, square_size, skip_piece=None):
    """
    Draw pieces on the board.

    Accepts either:
    - Board object with .board
    - raw 8x8 list
    """
    board_data = board.board if hasattr(board, "board") else board

    for row in range(8):
        for col in range(8):
            if skip_piece is not None and (row, col) == skip_piece:
                continue

            piece = board_data[row][col]

            if not piece:
                continue

            image = images.get(piece)
            if image is None:
                raise KeyError(f"No image loaded for piece: {piece}")

            win.blit(image, (col * square_size, row * square_size))


def draw_buttons(win):
    """
    Legacy helper.

    The current UI draws buttons inside graphics.py, but this function is kept
    so older code importing it does not break.
    """
    font = pygame.font.SysFont("arial", 20)

    buttons = [
        ((660, 50, 120, 40), "Undo", (690, 60)),
        ((660, 100, 120, 40), "Save", (690, 110)),
        ((660, 150, 120, 40), "Load", (690, 160)),
    ]

    for rect, text, pos in buttons:
        pygame.draw.rect(win, (200, 200, 200), rect)
        win.blit(font.render(text, True, (0, 0, 0)), pos)
