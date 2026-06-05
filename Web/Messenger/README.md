 ## Project Overview
A full-stack real-time messaging application built with Django, Django REST Framework, Django Channels, Redis, PostgreSQL, React, TypeScript, Vite, and Docker Compose.

## Key features

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

## Screenshots

## Architecture

* **Frontend:** React + TypeScript + Vite
* **Backend:** Django + Django REST Framework + Django Channels
* **Realtime:** WebSockets + Redis
* **Database:** PostgreSQL via Docker Compose
* **Containerization:** Docker + Docker Compose
* **Authentication:** JWT-based authentication
* **API:** REST API + WebSocket communication

## Running the Project

### Clone the repository
```bash
git clone <repository-url>
cd Web/Messenger
```
### Create environment file

Create a ```.env ``` file in the project root based on ```.env.example.```

For Docker Compose, use service names as hosts:
```bash
DB_ENGINE=django.db.backends.postgresql
DB_NAME=messenger_db
DB_USER=messenger_user
DB_PASSWORD=messenger_password
DB_HOST=postgres
DB_PORT=5432

REDIS_HOST=redis
REDIS_PORT=6379
```
For local backend development outside Docker, use localhost:
```bash
DB_ENGINE=django.db.backends.postgresql
DB_NAME=messenger_db 
DB_USER=messenger_user 
DB_PASSWORD=messenger_password
DB_HOST=127.0.0.1 
DB_PORT=5432

REDIS_HOST=127.0.0.1
REDIS_PORT=6379
```
## Docker Compose
Run the project:
```bash
docker compose up --build
```
This command starts:

* PostgreSQL
* Redis
* Django backend
* React frontend

Stop the project:
```bash
docker compose down
```
Do not use ``` docker compose down -v ``` unless you intentionally want to remove the PostgreSQL volume and delete local database data.
### Open the application

Frontend:
```bash
http://localhost:5173/
```
Backend:
```bash
http://localhost:8000/
```
## Local Backend and Frontend with Docker Services

You can also run backend and frontend locally outside Docker. In this case, PostgreSQL and Redis can still be started with Docker Compose.
### Start PostgreSQL and Redis:

```bash
docker compose up -d postgres redis
```

### Backend

```bash
cd Web/Messenger/backend
python -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
python manage.py migrate
python manage.py runserver
```

### Frontend

```bash
cd Web/Messenger/frontend
npm install
npm run dev
```

## Tests
### Docker Compose
Run backend tests inside the backend container:
```bash
docker compose exec backend python manage.py test
```
Run frontend tests inside the frontend container:
```bash
docker compose exec frontend npm test -- --run
```
### Local backend and frontend

If you run the backend outside Docker, start PostgreSQL and Redis first:
```bash
docker compose up -d postgres redis
```
Run backend tests locally:
```bash
cd Web/Messenger/backend
python manage.py test
```
Run frontend tests locally:
```bash
cd Web/Messenger/frontend
npm test -- --run
```
