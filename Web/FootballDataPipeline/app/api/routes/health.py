import asyncio

from fastapi import APIRouter, HTTPException, status
from redis.exceptions import RedisError
from sqlalchemy import text
from sqlalchemy.exc import SQLAlchemyError

from app.api.dependencies import CacheClient, DBSession
from app.core.config import get_settings
from app.services.readiness import check_rabbitmq_connection

router = APIRouter(tags=["Health"])


@router.get("/health")
async def health_check() -> dict[str, str]:
    return {"status": "ok"}


@router.get("/ready")
async def readiness_check(
    session: DBSession,
    cache_client: CacheClient,
) -> dict[str, str]:
    try:
        await session.execute(text("SELECT 1"))
    except SQLAlchemyError as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Database unavailable",
        ) from exc

    try:
        await cache_client.ping()
    except (RedisError, OSError) as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Redis unavailable",
        ) from exc

    try:
        await asyncio.to_thread(
            check_rabbitmq_connection,
            get_settings().celery_broker_url,
        )
    except Exception as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="RabbitMQ unavailable",
        ) from exc

    return {
        "status": "ready",
        "database": "ok",
        "redis": "ok",
        "rabbitmq": "ok",
    }
