from uuid import UUID

from fastapi.testclient import TestClient

from app.main import app


def test_request_id_is_generated() -> None:
    with TestClient(app) as client:
        response = client.get("/health")

    assert response.status_code == 200
    UUID(response.headers["X-Request-ID"])


def test_request_id_is_preserved() -> None:
    with TestClient(app) as client:
        response = client.get(
            "/health",
            headers={"X-Request-ID": "test-request-123"},
        )

    assert response.status_code == 200
    assert response.headers["X-Request-ID"] == "test-request-123"
