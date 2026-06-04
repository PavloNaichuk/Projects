 ## Project Overview
 A full-stack real-time messaging application built with Django, Django REST Framework, Django Channels, Redis, React, TypeScript, and Vite.

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

## How to Run Locally

### Clone the repository
```bash
git clone <repository-url>
cd Web/Messenger
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

### Redis

```bash
cd Web/Messenger
docker compose up -d redis
```

### PostgreSQL

```bash
cd Web/Messenger
docker compose up -d postgres
```

### Docker Compose
Run the project
```bash
cd Web/Messenger
docker compose up --build
```
This command starts:

* PostgreSQL
* Redis
* Django backend
* React frontend

Stop the project
```bash
cd Web/Messenger
docker compose down
```

### Tests
Run backend tests inside the backend container:
```bash
docker compose exec backend python manage.py test
```
Or locally, if you run the backend outside Docker:
```bash
cd Web/Messenger/backend
python manage.py test
```
