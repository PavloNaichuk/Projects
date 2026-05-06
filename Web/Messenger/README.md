## Local development

### Start Redis

WebSocket messaging uses Redis as a Channels layer.

Start Redis with Docker Compose:

```bash
docker compose up -d redis

# Messenger

Django + DRF + Channels backend for a real-time messenger.

## Local development

### 1. Create `.env`

Copy example env file:

```bash
cp .env.example .env