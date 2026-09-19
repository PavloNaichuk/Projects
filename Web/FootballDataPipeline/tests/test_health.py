from collections.abc import AsyncIterator
from unittest.mock import AsyncMock, MagicMock, patch

from fastapi.testclient import TestClient
from redis.asyncio import Redis
from redis.exceptions import RedisError
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.ext.asyncio import AsyncSession

from app.api.dependencies import get_db_session, get_redis_client
from app.main import app


def override_dependencies(
    session: AsyncSession,
    redis_client: Redis,
) -> None:
    async def override_db_session() -> AsyncIterator[AsyncSession]:
        yield session

    app.dependency_overrides[get_db_session] = override_db_session
    app.dependency_overrides[get_redis_client] = lambda: redis_client


def test_health_returns_ok() -> None:
    with TestClient(app) as client:
        response = client.get("/health")

    assert response.status_code == 200
    assert response.json() == {"status": "ok"}


def test_readiness_returns_ok() -> None:
    session = AsyncMock(spec=AsyncSession)
    redis_client = MagicMock(spec=Redis)
    redis_client.ping = AsyncMock(return_value=True)
    override_dependencies(session, redis_client)

    try:
        with patch(
            "app.api.routes.health.check_rabbitmq_connection",
        ) as check_rabbitmq:
            with TestClient(app) as client:
                response = client.get("/ready")
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 200
    assert response.json() == {
        "status": "ready",
        "database": "ok",
        "redis": "ok",
        "rabbitmq": "ok",
    }
    session.execute.assert_awaited_once()
    redis_client.ping.assert_awaited_once()
    check_rabbitmq.assert_called_once()


def test_readiness_returns_503_when_database_is_unavailable() -> None:
    session = AsyncMock(spec=AsyncSession)
    session.execute.side_effect = SQLAlchemyError("Connection failed")
    redis_client = MagicMock(spec=Redis)
    redis_client.ping = AsyncMock(return_value=True)
    override_dependencies(session, redis_client)

    try:
        with TestClient(app) as client:
            response = client.get("/ready")
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 503
    assert response.json() == {"detail": "Database unavailable"}
    redis_client.ping.assert_not_awaited()


def test_readiness_returns_503_when_redis_is_unavailable() -> None:
    session = AsyncMock(spec=AsyncSession)
    redis_client = MagicMock(spec=Redis)
    redis_client.ping = AsyncMock(
        side_effect=RedisError("Connection failed"),
    )
    override_dependencies(session, redis_client)

    try:
        with TestClient(app) as client:
            response = client.get("/ready")
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 503
    assert response.json() == {"detail": "Redis unavailable"}


def test_readiness_returns_503_when_rabbitmq_is_unavailable() -> None:
    session = AsyncMock(spec=AsyncSession)
    redis_client = MagicMock(spec=Redis)
    redis_client.ping = AsyncMock(return_value=True)
    override_dependencies(session, redis_client)

    try:
        with patch(
            "app.api.routes.health.check_rabbitmq_connection",
            side_effect=RuntimeError("Connection failed"),
        ):
            with TestClient(app) as client:
                response = client.get("/ready")
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 503
    assert response.json() == {"detail": "RabbitMQ unavailable"}
