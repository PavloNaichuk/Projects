# Pavlo Naichuk — Personal Projects

Collection of personal projects in web and desktop application development.

## Repository Structure

- `/Web` — Web applications (Django, React, REST APIs)
- `/Desktop` — Desktop applications (C++, Qt, WinAPI, Direct3D)
- `/Assets` — Screenshots and images

---

## 🔹 Web Projects

### Messenger

A full-stack real-time messaging application inspired by Telegram-style communication platforms.

**Technologies:** Python, Django, Django REST Framework, Django Channels, Redis, React, TypeScript, Vite, CSS, postgreSQL, Docker
  
**Key features:**

* JWT authentication and user authorization
* User registration, login, logout, and profile management
* Avatar upload, update, and deletion
* Private conversations between users
* Real-time messaging using WebSockets
* Typing indicator and online/offline status
* Message editing, deletion, replies, forwarding, reactions, search and pagination
* File and image attachments
* User blocking and unblocking
* Conversation pinning, muting, clearing, and deletion
* Real-time sidebar and profile updates
  
### Fitness Tracker API

A backend-focused fitness tracking application for managing workouts, body measurements, personal exercise records, weekly progress, and generated reports.

**Technologies:** Python, FastAPI, PostgreSQL, SQLAlchemy, Alembic, Pydantic, JWT, Redis, Celery, Docker, Pytest, Ruff, GitHub Actions

**Key features:**

* User registration and login using JWT authentication
* Login with either username or email
* Secure password hashing and protected user-specific data
* Public exercise catalog with muscle group and equipment filters
* Workout creation with multiple exercise sets
* Workout history with date and exercise filters
* Weekly statistics for workouts, sets, repetitions, and training volume
* Personal records for maximum weight, repetitions, volume, and estimated one-rep max
* Body measurement history and latest measurement retrieval
* Synchronous and background weekly report generation with Celery and Redis
* PostgreSQL database with SQLAlchemy models and Alembic migrations
* Docker Compose environment with FastAPI, PostgreSQL, Redis, and a Celery worker
* Simple browser dashboard and interactive API documentation
* Ruff linting and GitHub Actions CI

## 🔹 Desktop Projects
## Chess

A desktop chess game built with Python and Pygame.

**Technologies:** Python, Pygame, Pytest, Ruff, Socket networking, JSON

**Key features:**

* Local play, bot play, and network mode
* Legal move generation and full chess rule validation
* Check, checkmate, stalemate, castling, en passant, promotion, and draw rules
* Alpha-Beta bot with basic evaluation
* Move history, timers, undo, animations, sounds, and JSON save/load
* Modular UI structure and automated tests for core chess logic
* Ruff linting and GitHub Actions CI

## Battle City

A custom implementation inspired by the classic Battle City game.

**Technologies:** C++, Qt, QML

**Key features:**
- Model-view separation between game data and visual representation
- Tile-based level map
- Collision detection optimized for grid-based movement
- Enemy tank AI using goal-based behavior
- Breadth-first search pathfinding with clearance checks for larger units

## Pac Man

A custom Pac Man-style game implemented in C++.

**Technologies:** C++, WinAPI, Direct3D 11

**Key features:**
- Game loop and entity management
- Keyboard input handling
- Rendering with Direct3D 11
- Level map and text rendering

## Other Projects

- **Memory Puzzle** — 4x4 card matching game built with C++ and Qt
- **Tic Tac Toe** — console game with player-vs-player and player-vs-computer modes
