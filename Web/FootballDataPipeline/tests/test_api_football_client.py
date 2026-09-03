import httpx
import pytest

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)


@pytest.mark.asyncio
async def test_get_returns_payload_and_sends_api_key() -> None:
    expected_payload = {
        "errors": [],
        "response": [{"league": {"id": 39}}],
    }

    async def handler(request: httpx.Request) -> httpx.Response:
        assert request.headers["x-apisports-key"] == "test-api-key"
        assert request.url.path == "/leagues"
        assert request.url.params["country"] == "England"

        return httpx.Response(
            status_code=200,
            json=expected_payload,
        )

    transport = httpx.MockTransport(handler)

    async with APIFootballClient(
        base_url="https://example.test",
        api_key="test-api-key",
        timeout_seconds=10,
        transport=transport,
    ) as client:
        payload = await client.get(
            "/leagues",
            params={"country": "England"},
        )

    assert payload == expected_payload


@pytest.mark.asyncio
async def test_get_raises_for_api_error() -> None:
    async def handler(_request: httpx.Request) -> httpx.Response:
        return httpx.Response(
            status_code=200,
            json={
                "errors": {"token": "Invalid API key"},
                "response": [],
            },
        )

    transport = httpx.MockTransport(handler)

    async with APIFootballClient(
        base_url="https://example.test",
        api_key="test-api-key",
        timeout_seconds=10,
        transport=transport,
    ) as client:
        with pytest.raises(
            APIFootballResponseError,
            match="API-Football returned errors",
        ):
            await client.get("/leagues")


@pytest.mark.asyncio
async def test_get_raises_for_http_error() -> None:
    async def handler(_request: httpx.Request) -> httpx.Response:
        return httpx.Response(
            status_code=401,
            json={"message": "Unauthorized"},
        )

    transport = httpx.MockTransport(handler)

    async with APIFootballClient(
        base_url="https://example.test",
        api_key="test-api-key",
        timeout_seconds=10,
        transport=transport,
    ) as client:
        with pytest.raises(httpx.HTTPStatusError):
            await client.get("/leagues")
