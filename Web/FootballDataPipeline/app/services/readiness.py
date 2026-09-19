from kombu import Connection


def check_rabbitmq_connection(
    broker_url: str,
    *,
    timeout_seconds: float = 3.0,
) -> None:
    with Connection(
        broker_url,
        connect_timeout=timeout_seconds,
    ) as connection:
        connection.ensure_connection(
            max_retries=0,
            timeout=timeout_seconds,
        )
