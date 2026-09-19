import json
import logging
from datetime import datetime, timezone


class JsonFormatter(logging.Formatter):
    def format(self, record: logging.LogRecord) -> str:
        payload: dict[str, object] = {
            "timestamp": datetime.now(timezone.utc).isoformat(),
            "level": record.levelname,
            "logger": record.name,
            "message": record.getMessage(),
        }

        for field_name in (
            "request_id",
            "method",
            "path",
            "status_code",
            "duration_ms",
            "task_name",
            "league_id",
            "season",
        ):
            field_value = getattr(record, field_name, None)
            if field_value is not None:
                payload[field_name] = field_value

        if record.exc_info:
            payload["exception"] = self.formatException(record.exc_info)

        return json.dumps(
            payload,
            ensure_ascii=False,
            default=str,
        )


def configure_logging(level: str) -> None:
    logger = logging.getLogger("football_data_pipeline")
    logger.handlers.clear()

    handler = logging.StreamHandler()
    handler.setFormatter(JsonFormatter())

    logger.addHandler(handler)
    logger.setLevel(level)
    logger.propagate = False
