from fastapi import APIRouter, status

from app.schemas.tasks import (
    LeagueSyncRequest,
    TaskStatusResponse,
    TaskSubmittedResponse,
)
from app.tasks.celery_app import celery_app
from app.tasks.league_tasks import sync_leagues_task

router = APIRouter(
    prefix="/tasks",
    tags=["Tasks"],
)


@router.post(
    "/leagues/sync",
    response_model=TaskSubmittedResponse,
    status_code=status.HTTP_202_ACCEPTED,
)
def submit_league_sync(
    payload: LeagueSyncRequest,
) -> TaskSubmittedResponse:
    task = sync_leagues_task.delay(payload.league_id)

    return TaskSubmittedResponse(
        task_id=task.id,
        status=task.state,
    )


@router.get(
    "/{task_id}",
    response_model=TaskStatusResponse,
)
def get_task_status(task_id: str) -> TaskStatusResponse:
    task = celery_app.AsyncResult(task_id)

    task_result = task.result if task.successful() else None
    error = str(task.result) if task.failed() else None

    return TaskStatusResponse(
        task_id=task_id,
        status=task.state,
        result=task_result if isinstance(task_result, dict) else None,
        error=error,
    )
