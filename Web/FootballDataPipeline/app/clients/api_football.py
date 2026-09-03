from __future__ import annotations

from collections.abc import Mapping
from types import TracebackType
from typing import Any

import httpx
from tenacity import (
    retry,
    retry_if_exception_type,
    stop_after_attempt,
    wait_exponential,
)

from app.core.config import get_settings


class APIFootballError(RuntimeError):
    """Base exception for API-Football client errors."""


class APIFootballResponseError(APIFootballError):
    """Raised when API-Football reports an application error."""


class APIFootballClient:
    def __init__(
        self,
        *,
        base_url: str,
        api_key: str,
        timeout_seconds: float,
        transport: httpx.AsyncBaseTransport | None = None,
    ) -> None:
        self._client = httpx.AsyncClient(
            base_url=base_url,
            headers={
                "Accept": "application/json",
                "x-apisports-key": api_key,
            },
            timeout=httpx.Timeout(timeout_seconds),
            transport=transport,
        )

    async def __aenter__(self) -> APIFootballClient:
        return self

    async def __aexit__(
        self,
        _exc_type: type[BaseException] | None,
        _exc_value: BaseException | None,
        _traceback: TracebackType | None,
    ) -> None:
        await self.close()

    @retry(
        retry=retry_if_exception_type(httpx.TransportError),
        stop=stop_after_attempt(3),
        wait=wait_exponential(
            multiplier=0.5,
            min=0.5,
            max=4,
        ),
        reraise=True,
    )
    async def get(
        self,
        endpoint: str,
        *,
        params: Mapping[str, str | int | bool] | None = None,
    ) -> dict[str, Any]:
        response = await self._client.get(
            endpoint,
            params=params,
        )
        response.raise_for_status()

        payload = response.json()

        if not isinstance(payload, dict):
            raise APIFootballResponseError(
                "API-Football returned an invalid response",
            )

        errors = payload.get("errors")
        if errors:
            raise APIFootballResponseError(
                f"API-Football returned errors: {errors}",
            )

        return payload

    async def close(self) -> None:
        await self._client.aclose()


def create_api_football_client() -> APIFootballClient:
    settings = get_settings()

    return APIFootballClient(
        base_url=str(settings.api_football_base_url),
        api_key=settings.api_football_key.get_secret_value(),
        timeout_seconds=settings.api_football_timeout_seconds,
    )
