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

### Tests

```bash
cd Web/Messenger/backend
python manage.py test
```
