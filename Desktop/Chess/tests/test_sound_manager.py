from unittest.mock import MagicMock, patch

import pytest

from sound_manager import SoundManager

REQUIRED_SOUNDS = [
    "move.wav",
    "capture.wav",
    "check.wav",
    "checkmate.wav",
    "start.wav",
]


def create_sound_files(directory):
    for filename in REQUIRED_SOUNDS:
        (directory / filename).write_bytes(b"fake sound")


@patch("sound_manager.pygame.mixer.Sound")
@patch("sound_manager.pygame.mixer.init")
def test_sound_manager_loads_required_sounds(mock_mixer_init, mock_sound, tmp_path):
    create_sound_files(tmp_path)

    manager = SoundManager(sounds_dir=tmp_path)

    mock_mixer_init.assert_called_once()
    assert mock_sound.call_count == 5
    assert manager.sounds_dir == tmp_path


@patch("sound_manager.pygame.mixer.init")
def test_sound_manager_raises_if_sound_file_is_missing(mock_mixer_init, tmp_path):
    with pytest.raises(FileNotFoundError):
        SoundManager(sounds_dir=tmp_path)

    mock_mixer_init.assert_called_once()


@patch("sound_manager.pygame.mixer.Sound")
@patch("sound_manager.pygame.mixer.init")
def test_play_methods_call_loaded_sound_objects(mock_mixer_init, mock_sound, tmp_path):
    sound_objects = [MagicMock() for _ in range(5)]
    mock_sound.side_effect = sound_objects

    create_sound_files(tmp_path)

    manager = SoundManager(sounds_dir=tmp_path)

    manager.play_move()
    manager.play_capture()
    manager.play_check()
    manager.play_checkmate()
    manager.play_start()

    for sound in sound_objects:
        sound.play.assert_called_once()