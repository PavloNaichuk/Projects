from pathlib import Path

import pygame

BASE_DIR = Path(__file__).resolve().parent
DEFAULT_SOUNDS_DIR = BASE_DIR / "sounds"


class SoundManager:
    def __init__(self, sounds_dir=None):
        pygame.mixer.init()

        self.sounds_dir = Path(sounds_dir) if sounds_dir else DEFAULT_SOUNDS_DIR

        self.move = self._load("move.wav")
        self.capture = self._load("capture.wav")
        self.check = self._load("check.wav")
        self.checkmate = self._load("checkmate.wav")
        self.start = self._load("start.wav")

    def _load(self, filename):
        path = self.sounds_dir / filename

        if not path.exists():
            raise FileNotFoundError(f"Missing sound file: {path}")

        return pygame.mixer.Sound(str(path))

    def play_move(self):
        self.move.play()

    def play_capture(self):
        self.capture.play()

    def play_check(self):
        self.check.play()

    def play_checkmate(self):
        self.checkmate.play()

    def play_start(self):
        self.start.play()
