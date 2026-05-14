# Messenger

Real-time messenger built with Django, Django REST Framework, Django Channels, Redis, and React.

## Features

- User registration and login with JWT
- Real-time messaging with WebSocket
- Conversation list updates without refresh
- Online / offline status
- Typing indicator
- Read status
- Message edit and delete
- Delete conversation for me / for everyone
- Message search inside conversation
- Message pagination / infinite scroll
- Image and file attachments
- Delete attachment from message

## Tech Stack

### Backend

- Python
- Django
- Django REST Framework
- Django Channels
- Redis
- SQLite
- Simple JWT
- Pytest / Django tests

### Frontend

- React
- TypeScript
- Vite
- CSS

## Local Development

### 1. Create `.env`

Copy example environment file:

```bash
cp .env.example .env