# Fitness Tracker API

## Project Overview

A backend-focused fitness tracking application for managing workouts, exercises, body measurements, personal exercise records, weekly progress, and generated weekly reports.

The project demonstrates REST API design, JWT authentication, relational database modeling, background task processing, Dockerized development, automated testing, and CI checks.

## Key Features

* User registration with unique email and username validation
* Login using either username or email
* JWT authentication with configurable token expiration
* Secure password hashing with pwdlib
* Protected user-specific routes and data isolation
* Public exercise catalog with filtering by muscle group and equipment
* Protected exercise creation
* Workout creation with multiple exercise sets
* Workout history filtered by date range or exercise
* Workout deletion with automatic workout-set cleanup
* Weekly progress calculation
* Personal exercise records:
  * maximum weight
  * maximum repetitions
  * maximum set volume
  * estimated one-rep max
* Body measurement tracking for weight, body fat, and waist size
* Body measurement history filtered by date range
* Latest body measurement retrieval
* Synchronous weekly report generation
* Background weekly report generation with Celery
* Celery task status tracking through Redis
* Existing weekly report updates for the same user and week
* Simple HTML, CSS, and JavaScript dashboard
* Interactive Swagger and ReDoc API documentation
* Root and health-check endpoints

## Demo UI

The project includes a lightweight dashboard for demonstrating the main API features without requiring a separate frontend application.

The dashboard supports:

* Registration, login, and logout
* Exercise creation and listing
* Workout creation and history
* Body measurement creation, history, and latest measurement
* Weekly progress calculation
* Exercise record calculation
* Weekly report generation and history

After starting the application, open:

```text
http://127.0.0.1:8000/ui
```

## Architecture

* **Runtime:** Python 3.13
* **Backend:** FastAPI + Pydantic
* **Database:** PostgreSQL
* **ORM:** SQLAlchemy
* **Migrations:** Alembic
* **Authentication:** PyJWT + OAuth2 password flow
* **Password Hashing:** pwdlib
* **Task Queue:** Celery
* **Message Broker and Result Backend:** Redis
* **Containerization:** Docker + Docker Compose
* **Testing:** pytest + FastAPI TestClient
* **Test Database:** In-memory SQLite
* **Coverage:** coverage.py
* **Linting:** Ruff
* **CI:** GitHub Actions

## Database Structure

The application uses the following main database models:

* **User:** email, username, password hash, and creation date
* **Exercise:** name, muscle group, equipment, and creation date
* **Workout:** user, title, workout date, notes, and workout sets
* **WorkoutSet:** exercise, set number, weight, and repetitions
* **BodyMeasurement:** user, date, body weight, body fat, and waist size
* **WeeklyReport:** user, week range, workout count, sets, repetitions, volume, and generation date

Important database rules:

* Email addresses and usernames are unique
* Workouts, body measurements, and reports belong to a specific user
* Deleting a workout also deletes its workout sets
* User-related records use cascading foreign-key deletion
* Only one weekly report can exist for each user and week
* Generating the same weekly report again updates the existing record

## Authentication and Data Protection

The API uses bearer JWT access tokens.

* Passwords are stored as secure hashes
* Tokens contain the user ID and expiration time
* Invalid, expired, or malformed tokens return `401 Unauthorized`
* Registration prevents duplicate email addresses and usernames
* Workouts can only be viewed or deleted by their owner
* Progress calculations only use the current user's workouts
* Measurements and weekly reports are isolated by user
* Exercise reading is public, while exercise creation requires authentication
* Secrets and service URLs are loaded from environment variables

## Input Validation

Pydantic schemas validate incoming API data:

* Email addresses must use a valid email format
* Usernames must contain between 3 and 100 characters
* Passwords must contain between 6 and 100 characters
* Exercise names and muscle groups have length limits
* Set numbers must be at least 1
* Weight cannot be negative
* Repetitions must be at least 1
* Body weight and waist size must be greater than 0
* Body-fat percentage must be between 0 and 100

## Quality and CI

The project includes automated checks to keep the codebase reliable and maintainable.

### Quality Checks

* Ruff linting
* pytest API and service tests
* Application test coverage with coverage.py
* Docker image build check

### GitHub Actions Pipeline

The CI pipeline runs:

* Ruff linting
* pytest test suite
* Coverage verification
* Docker image build

The CI coverage threshold is **85%**.

### Current Test Result

```text
36 passed
```

### Current Application Coverage

```text
94%
```

The stored coverage data reports 525 statements with 34 missed statements, which equals 93.52% and is rounded to 94%.

## Running the Project

### Clone the Repository

```powershell
git clone https://github.com/PavloNaichuk/Projects.git
cd Projects/Web/FitnessTrackerAPI
```

### Create the Environment File

Create a local `.env` file from the example:

```powershell
copy .env.example .env
```

Example configuration:

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

The `.env` file is ignored by Git and must not be committed.

## Local Development

Create and activate a virtual environment:

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
```

Install dependencies:

```powershell
python -m pip install -r requirements.txt
```

Start PostgreSQL and Redis:

```powershell
docker compose up -d db redis
```

Apply database migrations:

```powershell
alembic upgrade head
```

Start the FastAPI development server:

```powershell
uvicorn app.main:app --reload
```

## Docker Compose

Build and start all services:

```powershell
docker compose up -d --build
```

Apply migrations inside the application container:

```powershell
docker compose exec app alembic upgrade head
```

Docker Compose starts:

* FastAPI application
* PostgreSQL 16 database
* Redis 7 broker
* Celery worker

Stop the services:

```powershell
docker compose down
```

Do not use `docker compose down -v` unless you intentionally want to delete the PostgreSQL volume and all local database data.

## Application URLs

| Resource | URL |
| --- | --- |
| Demo dashboard | `http://127.0.0.1:8000/ui` |
| Swagger documentation | `http://127.0.0.1:8000/docs` |
| ReDoc documentation | `http://127.0.0.1:8000/redoc` |
| Root endpoint | `http://127.0.0.1:8000/` |
| Health endpoint | `http://127.0.0.1:8000/health` |

Expected health response:

```json
{
  "status": "ok"
}
```

## API Endpoints

### General

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| GET | `/` | No | API status message |
| GET | `/health` | No | Health check |

### Authentication

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| POST | `/auth/register` | No | Register a new user |
| POST | `/auth/login` | No | Log in with username or email |
| GET | `/auth/me` | Yes | Return the current user |

### Exercises

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| POST | `/exercises` | Yes | Create an exercise |
| GET | `/exercises` | No | List and filter exercises |
| GET | `/exercises/{exercise_id}` | No | Return one exercise |

Optional exercise filters:

```text
GET /exercises?muscle_group=Chest&equipment=Barbell
```

### Workouts

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| POST | `/workouts` | Yes | Create a workout with sets |
| GET | `/workouts` | Yes | Return the current user's workouts |
| GET | `/workouts/{workout_id}` | Yes | Return one owned workout |
| DELETE | `/workouts/{workout_id}` | Yes | Delete one owned workout |

Optional workout filters:

```text
GET /workouts?date_from=2026-07-01&date_to=2026-07-31&exercise_id=1
```

### Progress

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| GET | `/progress/weekly?week_start={date}` | Yes | Calculate progress for a seven-day period |
| GET | `/progress/exercises/{exercise_id}/records` | Yes | Calculate personal exercise records |

### Body Measurements

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| POST | `/measurements` | Yes | Create a body measurement |
| GET | `/measurements` | Yes | Return measurement history |
| GET | `/measurements/latest` | Yes | Return the latest measurement |

Optional measurement filters:

```text
GET /measurements?date_from=2026-07-01&date_to=2026-07-31
```

### Weekly Reports

| Method | Endpoint | Authentication | Description |
| --- | --- | --- | --- |
| POST | `/reports/weekly/generate?week_start={date}` | Yes | Generate or update a report synchronously |
| POST | `/reports/weekly/generate-async?week_start={date}` | Yes | Queue report generation with Celery |
| GET | `/reports/tasks/{task_id}` | No | Return Celery task status and result |
| GET | `/reports/weekly` | Yes | Return the current user's reports |

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

The login endpoint uses OAuth2 form data. The `username` field accepts either a username or an email address.

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

### Create an Exercise

```json
{
  "name": "Bench Press",
  "muscle_group": "Chest",
  "equipment": "Barbell"
}
```

### Create a Workout

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

### Create a Body Measurement

```json
{
  "measured_at": "2026-07-05",
  "body_weight": 82.5,
  "body_fat": 18.5,
  "waist": 88
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

### Workout Volume

```text
set volume = weight * reps
weekly volume = sum of all set volumes during the week
```

Example:

```text
80 kg * 10 reps = 800 volume
```

### Estimated One-Rep Max

The project uses the Epley formula:

```text
estimated 1RM = weight * (1 + reps / 30)
```

Example:

```text
80 * (1 + 10 / 30) = 106.67 estimated 1RM
```

### Weekly Reports

A weekly report covers seven days beginning with the supplied `week_start` date and contains:

* Number of workouts
* Number of sets
* Total repetitions
* Total training volume

If a report already exists for the same user and week, its values are recalculated and updated instead of creating a duplicate.

## Tests

Run the complete test suite:

```powershell
python -m pytest -q
```

The 36 tests cover:

* User registration, login, and current-user authentication
* Unauthorized access handling
* Exercise creation, public listing, muscle-group filtering, duplicate names, and missing exercises
* Workout creation, listing, retrieval, and deletion
* Missing exercise and missing workout handling
* Body measurement creation, history, and latest measurement
* Weekly progress API
* Exercise records API
* Set-volume calculation
* Estimated one-rep max calculation and rounding
* Weekly report generation and history
* Updating existing weekly reports

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

The current migration history creates:

* Users
* Exercises
* Workouts and workout sets
* Body measurements
* Weekly reports
* Exercise name indexing with duplicate names allowed

## Background Tasks

Celery generates weekly reports outside the API request cycle.

Start all services:

```powershell
docker compose up -d --build
```

Queue a weekly report:

```text
POST /reports/weekly/generate-async?week_start=2026-07-05
```

The endpoint returns `202 Accepted` with a task ID:

```json
{
  "task_id": "celery-task-id",
  "status": "PENDING"
}
```

Check the task:

```text
GET /reports/tasks/{task_id}
```

## Portfolio Summary

Fitness Tracker API demonstrates:

* REST API design with FastAPI
* JWT authentication and ownership-based data access
* PostgreSQL database modeling with SQLAlchemy
* Schema migrations with Alembic
* Pydantic request validation
* Workout analytics and personal-record calculations
* Background processing with Celery and Redis
* Dockerized development with PostgreSQL and Redis
* Automated API and service testing
* High application test coverage
* CI checks with GitHub Actions
