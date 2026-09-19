import logging
from time import perf_counter
from uuid import uuid4

from fastapi import Request, Response
from starlette.middleware.base import (
    BaseHTTPMiddleware,
    RequestResponseEndpoint,
)
from starlette.types import ASGIApp

logger = logging.getLogger("football_data_pipeline.http")


class RequestLoggingMiddleware(BaseHTTPMiddleware):
    def __init__(self, app: ASGIApp) -> None:
        super().__init__(app)

    async def dispatch(
        self,
        request: Request,
        call_next: RequestResponseEndpoint,
    ) -> Response:
        request_id = request.headers.get("X-Request-ID") or str(uuid4())
        started_at = perf_counter()

        try:
            response = await call_next(request)
        except Exception:
            logger.exception(
                "request_failed",
                extra={
                    "request_id": request_id,
                    "method": request.method,
                    "path": request.url.path,
                    "duration_ms": round(
                        (perf_counter() - started_at) * 1000,
                        2,
                    ),
                },
            )
            raise

        duration_ms = round(
            (perf_counter() - started_at) * 1000,
            2,
        )
        response.headers["X-Request-ID"] = request_id

        logger.info(
            "request_completed",
            extra={
                "request_id": request_id,
                "method": request.method,
                "path": request.url.path,
                "status_code": response.status_code,
                "duration_ms": duration_ms,
            },
        )

        return response
