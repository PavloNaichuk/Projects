# Chess
![CI](https://github.com/PavloNaichuk/Projects/actions/workflows/ci.yml/badge.svg?branch=master)
A desktop chess game built with **Python** and **Pygame**.

The project includes local play, bot play, network mode, chess rule validation, move history, animations, sounds, timers, JSON save/load, and automated tests for core chess rules.

## Features

- Local 1 vs 1 chess mode
- Play against an Alpha-Beta bot
- Network mode: host or join a game
- Legal move validation
- Check, checkmate, and stalemate detection
- Castling
- En passant
- Pawn promotion
- Move history panel
- Undo move
- Timers with increment
- Save/load game state using JSON
- Sound effects
- Move animations
- Automated tests for important chess rules

## Tech Stack

- Python
- Pygame
- Pillow
- Pytest
- Socket networking
- Tkinter file dialogs

## Project Structure

```text
Chess/
├── board.py
├── bot.py
├── game.py
├── game_over_window.py
├── graphics.py
├── main.py
├── mode_select.py
├── network.py
├── utils.py
├── requirements.txt
├── requirements-dev.txt
├── tests/
│   └── test_game_rules.py
├── images/
│   ├── wK.png
│   ├── wQ.png
│   └── ...
└── sounds/
    ├── move.wav
    ├── capture.wav
    └── ...
```

## Installation

Clone the repository:

```bash
git clone <your-repository-url>
cd Chess
```

Create and activate a virtual environment:

```bash
python -m venv .venv
```

On Windows PowerShell:

```powershell
.\.venv\Scripts\Activate.ps1
```

Install dependencies:

```bash
python -m pip install -r requirements.txt
```

## Run the Game

```bash
python main.py
```

## Run Tests

Install development dependencies:

```bash
python -m pip install -r requirements-dev.txt
```

Run tests:

```bash
python -m pytest -q
```

## Game Modes

### Local Mode

Two players can play on the same computer.

### Bot Mode

The player controls White and the bot controls Black.

The bot uses:

- Alpha-Beta search
- Move ordering
- Basic evaluation
- Quiescence search for capture moves

### Network Mode

One player hosts the game and another player joins by entering the host IP address.

The network mode supports normal moves and pawn promotion.

## Save and Load

The game state is saved in JSON format.

Autosave file:

```text
autosave.json
```

This file should not be committed to Git.

## Notes

This project is focused on Python desktop development and chess game logic.

It demonstrates:

- Object-oriented programming
- Game loop architecture
- Board state management
- Legal move generation
- Basic AI search
- Socket communication
- JSON persistence
- Automated testing

## Future Improvements

- Improve UI layout and responsiveness
- Add stronger chess notation
- Add captured pieces panel
- Add online matchmaking or lobby
- Add difficulty levels for the bot
- Add CI with GitHub Actions
- Add screenshots and demo GIF to README
