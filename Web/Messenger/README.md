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
