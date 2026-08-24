# Fitness Tracker API

## Project Overview

A backend-focused API for tracking workouts, exercises, body measurements, personal records, weekly progress, and generated weekly reports.

The project is built as a portfolio project using FastAPI, PostgreSQL, SQLAlchemy, Alembic, Redis, Celery, Docker Compose, pytest, Ruff, and GitHub Actions CI.

## Key Features

* User registration and login
* JWT authentication, password hashing, and protected user routes
* Exercise database with workout set relations
* Workout creation, history, and deletion
* Weekly workout progress calculation
* Personal exercise records and estimated one-rep max calculation
* Body measurements tracking
* Weekly report generation and retrieval
* Background weekly report generation with Celery and Redis
* PostgreSQL database with Alembic migrations
* Simple HTML dashboard for testing the main API features

## Demo UI

The project includes a simple HTML dashboard for testing the main API features.

After starting the server, open:

```text
http://127.0.0.1:8000/ui
```

Interactive API documentation is available at:

```text
http://127.0.0.1:8000/docs
```

## Architecture

* **Backend:** FastAPI + Pydantic
* **Database:** PostgreSQL + SQLAlchemy
* **Migrations:** Alembic
* **Authentication:** JWT-based authentication with password hashing
* **Task Queue:** Celery + Redis
* **Containerization:** Docker + Docker Compose
* **Testing:** pytest + coverage.py
* **Code Quality:** Ruff + GitHub Actions CI

## Quality and CI

The project includes automated checks to keep the codebase reliable and maintainable:

* API tests with pytest
* Test coverage reporting with coverage.py
* Ruff linting
* GitHub Actions CI
* Docker image build check in CI
* Coverage threshold of 85% in CI

### Current Test Result

```text
36 passed
```

### Current Application Coverage

```text
94%
```

## Running the Project

### Clone the repository

```text
git clone <repository-url>
cd Fitness-Tracker-API
```

### Create environment file

Create a local `.env` file from the example file:

```powershell
copy .env.example .env
```

Then update the values in `.env` if needed:

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

### Run locally

Create and activate a virtual environment:

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
```

Install dependencies:

```powershell
pip install -r requirements.txt
```

Start PostgreSQL and Redis:

```powershell
docker compose up -d db redis
```

Apply migrations:

```powershell
alembic upgrade head
```

Start the FastAPI server:

```powershell
uvicorn app.main:app --reload
```

### Run with Docker Compose

Build and start all services:

```powershell
docker compose up -d --build
```

Apply migrations inside the application container:

```powershell
docker compose exec app alembic upgrade head
```

This starts:

* FastAPI application
* PostgreSQL database
* Redis broker
* Celery worker

## API Endpoints

### Authentication

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

## Example Requests

### Register

```json
{
  "email": "test@example.com",
  "username": "testuser",
  "password": "password123"
}
```

### Login

The login endpoint uses OAuth2 form data:

```text
username: testuser
password: password123
```

Example response:

```json
{
  "access_token": "jwt-token",
  "token_type": "bearer"
}
```

### Exercise

```json
{
  "name": "Bench Press",
  "muscle_group": "Chest",
  "equipment": "Barbell"
}
```

### Workout

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

### Weekly Progress

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

The API calculates workout analytics using the following formulas:

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

## Tests and Local Checks

Run the test suite:

```powershell
python -m pytest -q
```

The test suite covers:

* Authentication API
* Exercises API
* Workouts API
* Progress API
* Reports API
* Measurements API
* Workout set volume calculation
* Estimated one-rep max calculation
* Weekly report generation and updates

Run tests with coverage:

```powershell
python -m coverage run -m pytest
python -m coverage report -m
```

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

Celery generates weekly reports in the background.

Start all services:

```powershell
docker compose up -d --build
```

Generate a weekly report asynchronously:

```text
POST /reports/weekly/generate-async?week_start=2026-07-05
```

Check the task status:

```text
GET /reports/tasks/{task_id}
```

## Portfolio Summary

Fitness Tracker API demonstrates:

* REST API design
* JWT authentication and authorization
* Database modeling and SQLAlchemy relationships
* Alembic migrations
* Backend business logic for workout analytics
* Background jobs with Celery and Redis
* Dockerized development environment
* Automated API tests, coverage checks, and CI/CD
