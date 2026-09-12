from celery import Celery

from app.core.config import get_settings

settings = get_settings()

celery_app = Celery(
    "football_data_pipeline",
    broker=settings.celery_broker_url,
    backend=settings.celery_result_backend,
    include=[
        "app.tasks.league_tasks",
        "app.tasks.team_tasks",
        "app.tasks.fixture_tasks",
        "app.tasks.standing_tasks",
    ],
)

celery_app.conf.update(
    accept_content=["json"],
    task_serializer="json",
    result_serializer="json",
    timezone="UTC",
    enable_utc=True,
    task_track_started=True,
    task_acks_late=True,
    task_reject_on_worker_lost=True,
    worker_prefetch_multiplier=1,
    worker_enable_remote_control=False,
    worker_cancel_long_running_tasks_on_connection_loss=True,
    broker_connection_retry_on_startup=True,
    result_expires=3600,
)
