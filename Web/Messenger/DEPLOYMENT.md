# Deployment Guide

This guide describes how to run the Messenger project with production-style Docker
Compose settings.

## Files

Use these files for production deployment:

```text
.env.production.example
docker-compose.prod.example.yml
frontend/Dockerfile.prod
frontend/nginx.conf
backend/Dockerfile
```

Create your real production environment file from the example:

```bash
cp .env.production.example .env.production
```

Do not commit `.env.production`.

## Production environment variables

Edit `.env.production` before deployment.

Required values to change:

```env
SECRET_KEY=change-me-to-long-random-secret-key
DEBUG=False
ALLOWED_HOSTS=your-backend-domain.com
CORS_ALLOWED_ORIGINS=https://your-frontend-domain.com

DB_PASSWORD=change-me-to-strong-production-password

FRONTEND_URL=https://your-frontend-domain.com
```

Generate a strong Django secret key:

```bash
python -c "import secrets; print(secrets.token_urlsafe(64))"
```

## SMTP configuration

For production email verification, use a real SMTP provider:

```env
EMAIL_BACKEND=django.core.mail.backends.smtp.EmailBackend
EMAIL_HOST=smtp.example.com
EMAIL_PORT=587
EMAIL_HOST_USER=your-smtp-user@example.com
EMAIL_HOST_PASSWORD=your-smtp-password-or-app-password
EMAIL_USE_TLS=True
EMAIL_USE_SSL=False
EMAIL_TIMEOUT=20
DEFAULT_FROM_EMAIL="Messenger <no-reply@example.com>"
```

For Gmail SMTP over SSL, use:

```env
EMAIL_HOST=smtp.gmail.com
EMAIL_PORT=465
EMAIL_USE_TLS=False
EMAIL_USE_SSL=True
EMAIL_HOST_USER=your-email@gmail.com
EMAIL_HOST_PASSWORD=your-gmail-app-password
DEFAULT_FROM_EMAIL="Messenger <your-email@gmail.com>"
```

The password must be an app password, not the normal Gmail account password.

## HTTPS security settings

Production should use:

```env
DEBUG=False
SECURE_SSL_REDIRECT=True
SESSION_COOKIE_SECURE=True
CSRF_COOKIE_SECURE=True
SECURE_HSTS_SECONDS=31536000
SECURE_HSTS_INCLUDE_SUBDOMAINS=True
SECURE_HSTS_PRELOAD=True
```

Before enabling HSTS in a real public deployment, make sure the site is fully
served over HTTPS.

## Frontend API and WebSocket URLs

The production nginx container proxies backend traffic:

```text
/api/  -> backend:8000
/ws/   -> backend:8000
/media/ -> backend_media volume
```

For a single-domain Docker deployment, frontend build args can use relative URLs:

```env
VITE_API_BASE_URL=/api
VITE_WS_BASE_URL=/ws
```

For a split frontend/backend deployment, use full public URLs:

```env
VITE_API_BASE_URL=https://your-backend-domain.com/api
VITE_WS_BASE_URL=wss://your-backend-domain.com/ws
```

## Validate production config

Run this from the project root:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml config
```

## Start production compose

Run this from the project root:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml up -d --build
```

This starts:

```text
postgres
redis
backend
frontend nginx
```

The backend runs with Daphne:

```text
daphne -b 0.0.0.0 -p 8000 config.asgi:application
```

The frontend runs with nginx on port `80`.

## Check service status

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml ps
```

Backend logs:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml logs backend --tail=100
```

Frontend logs:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml logs frontend --tail=100
```

## Health check

Backend health endpoint:

```text
GET /api/health/
```

Expected response:

```json
{"status": "ok"}
```

If nginx is running publicly, check:

```bash
curl http://your-domain.com/api/health/
```

For local production-compose testing:

```bash
curl http://127.0.0.1/api/health/
```

## Media files

Uploaded media files are stored in the `backend_media` Docker volume.

The backend writes files to:

```text
/app/media
```

The frontend nginx container serves them read-only from:

```text
/usr/share/nginx/html/media
```

Public media URLs are served under:

```text
/media/
```

## Stop production compose

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml down
```

Do not use `-v` unless you intentionally want to delete PostgreSQL and media
volumes:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml down -v
```

## Files that must not be committed

Never commit:

```text
.env
.env.production
.env.local
.env.development
```

Only commit example files:

```text
.env.example
.env.production.example
```

## Final production check

Inside the backend container:

```bash
docker compose --env-file .env.production -f docker-compose.prod.example.yml exec backend python manage.py check --deploy
```

Expected result:

```text
System check identified no issues (0 silenced).
```
