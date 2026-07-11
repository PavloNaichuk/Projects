# Fitness Tracker API

A backend API for tracking workouts, exercises, body measurements, personal records, weekly progress, and generated weekly reports.

The project is built as a backend-focused portfolio project using FastAPI, PostgreSQL, SQLAlchemy, Alembic, Redis, Celery, Docker, pytest, Ruff, and GitHub Actions CI.

## Features

- User registration and login
- JWT authentication
- Password hashing
- Protected user routes
- Exercise database
- Workout sessions
- Workout sets with weight, reps, and exercise relation
- Workout history
- Weekly progress calculation
- Exercise records
- Estimated one-rep max calculation
- Body measurements tracking
- Weekly report generation
- Background weekly report generation with Celery
- Redis broker and result backend
- PostgreSQL database
- Alembic migrations
- Docker Compose setup
- Simple HTML dashboard
- API tests with pytest
- Test coverage with coverage.py
- Ruff linting
- GitHub Actions CI
- Docker build check in CI

## Tech Stack

- Python
- FastAPI
- PostgreSQL
- SQLAlchemy
- Alembic
- Pydantic
- PyJWT
- pwdlib
- Redis
- Celery
- Docker Compose
- pytest
- coverage.py
- Ruff
- GitHub Actions

## Demo UI

The project includes a simple HTML dashboard for testing the main API features.

After starting the server, open:

```text
http://127.0.0.1:8000/ui
```

API documentation is available at:

```text
http://127.0.0.1:8000/docs
```

## Environment Variables

The project uses environment variables for database connection, JWT authentication, Redis, and Celery settings.

Create a local `.env` file from the example file:

```powershell
copy .env.example .env
```

Then update the values in `.env` if needed.

Example:

```env
PROJECT_NAME=Fitness Tracker API

DATABASE_URL=postgresql+psycopg://postgres:postgres@localhost:5432/fitness_tracker_db

SECRET_KEY=replace-this-with-a-long-secure-secret-key-at-least-32-characters
ALGORITHM=HS256
ACCESS_TOKEN_EXPIRE_MINUTES=30

REDIS_URL=redis://localhost:6379/0
CELERY_BROKER_URL=redis://localhost:6379/0
CELERY_RESULT_BACKEND=redis://localhost:6379/1
```

The `.env` file is ignored by Git and should not be committed.

## Run Locally

Create and activate a virtual environment:

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
```

Install dependencies:

```powershell
pip install -r requirements.txt
```

Start PostgreSQL and Redis using Docker Compose:

```powershell
docker compose up -d db redis
```

Run Alembic migrations:

```powershell
alembic upgrade head
```

Start the FastAPI server:

```powershell
uvicorn app.main:app --reload
```

Open the dashboard:

```text
http://127.0.0.1:8000/ui
```

Open API documentation:

```text
http://127.0.0.1:8000/docs
```

## Run with Docker Compose

Build and start all services:

```powershell
docker compose up -d --build
```

Run migrations inside the app container:

```powershell
docker compose exec app alembic upgrade head
```

Open the dashboard:

```text
http://127.0.0.1:8000/ui
```

Services:

```text
app              FastAPI backend
db               PostgreSQL database
redis            Redis broker
celery_worker    Celery background worker
```

## API Endpoints

### Auth

```text
POST /auth/register
POST /auth/login
GET  /auth/me
```

### Exercises

```text
POST /exercises
GET  /exercises
GET  /exercises/{exercise_id}
```

### Workouts

```text
POST   /workouts
GET    /workouts
GET    /workouts/{workout_id}
DELETE /workouts/{workout_id}
```

### Progress

```text
GET /progress/weekly
GET /progress/exercises/{exercise_id}/records
```

### Body Measurements

```text
POST /measurements
GET  /measurements
GET  /measurements/latest
```

### Weekly Reports

```text
POST /reports/weekly/generate
POST /reports/weekly/generate-async
GET  /reports/tasks/{task_id}
GET  /reports/weekly
```

## Example Register Request

```json
{
  "email": "test@example.com",
  "username": "testuser",
  "password": "password123"
}
```

## Example Login

The login endpoint uses OAuth2 form data.

```text
username: testuser
password: password123
```

Response:

```json
{
  "access_token": "jwt-token",
  "token_type": "bearer"
}
```

## Example Exercise

```json
{
  "name": "Bench Press",
  "muscle_group": "Chest",
  "equipment": "Barbell"
}
```

## Example Workout

```json
{
  "title": "Chest Day",
  "workout_date": "2026-07-05",
  "notes": "Bench press training",
  "sets": [
    {
      "exercise_id": 1,
      "set_number": 1,
      "weight": 80,
      "reps": 10
    },
    {
      "exercise_id": 1,
      "set_number": 2,
      "weight": 85,
      "reps": 8
    }
  ]
}
```

## Example Weekly Progress

```text
GET /progress/weekly?week_start=2026-07-05
```

Example response:

```json
{
  "week_start": "2026-07-05",
  "week_end": "2026-07-11",
  "workouts_count": 1,
  "total_sets": 2,
  "total_reps": 18,
  "total_volume": 1480
}
```

## Business Logic

The project includes backend business logic for workout analytics:

```text
set volume = weight * reps
weekly volume = sum of all set volumes in a week
estimated one-rep max = weight * (1 + reps / 30)
```

Example:

```text
80 kg x 10 reps = 800 volume
80 * (1 + 10 / 30) = 106.67 estimated 1RM
```

## Tests

Run tests:

```powershell
python -m pytest -q
```

Current test coverage includes:

- Auth API tests
- Exercises API tests
- Workouts API tests
- Progress API tests
- Reports API tests
- Measurements API tests
- Workout set volume calculation
- Estimated one-rep max calculation
- Weekly report generation
- Updating existing weekly reports

Current result:

```text
36 passed
```

## Test Coverage

The project uses coverage.py to measure test coverage for the application code.

Run tests with coverage:

```powershell
python -m coverage run -m pytest
python -m coverage report -m
```

Current application coverage:

```text
94%
```

GitHub Actions checks that coverage does not fall below 85%.

## Linting

Run Ruff:

```powershell
python -m ruff check .
```

## Database Migrations

Create a new migration:

```powershell
alembic revision --autogenerate -m "migration message"
```

Apply migrations:

```powershell
alembic upgrade head
```

## Background Tasks

Celery is used for background weekly report generation.

Start all services:

```powershell
docker compose up -d --build
```

Generate weekly report asynchronously:

```text
POST /reports/weekly/generate-async?week_start=2026-07-05
```

Check task status:

```text
GET /reports/tasks/{task_id}
```

## CI Checks

GitHub Actions runs automated checks for the project:

- Ruff linting
- pytest test suite
- coverage check
- Docker image build

## Portfolio Summary

Fitness Tracker API is a backend-focused FastAPI project that demonstrates:

- REST API design
- JWT authentication
- Database modeling
- SQLAlchemy relationships
- Alembic migrations
- Business logic implementation
- Background jobs with Celery
- Redis integration
- Dockerized development environment
- Automated API tests
- Test coverage checks
- CI/CD checks with GitHub Actions