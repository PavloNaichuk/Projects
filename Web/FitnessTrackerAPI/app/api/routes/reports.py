from datetime import date

from celery.result import AsyncResult
from fastapi import APIRouter, Depends, Query, status
from sqlalchemy import select
from sqlalchemy.orm import Session

from app.api.deps import get_current_user
from app.core.celery_app import celery_app
from app.core.database import get_db
from app.models.report import WeeklyReport
from app.models.user import User
from app.schemas.report import TaskRead, TaskStatusRead, WeeklyReportRead
from app.services.report_service import generate_weekly_report
from app.tasks.weekly_reports import generate_weekly_report_task


router = APIRouter(
    prefix="/reports",
    tags=["Reports"],
)


@router.post(
    "/weekly/generate",
    response_model=WeeklyReportRead,
    status_code=status.HTTP_201_CREATED,
)
def create_weekly_report(
    week_start: date = Query(...),
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    return generate_weekly_report(
        db=db,
        user_id=current_user.id,
        week_start=week_start,
    )


@router.post(
    "/weekly/generate-async",
    response_model=TaskRead,
    status_code=status.HTTP_202_ACCEPTED,
)
def create_weekly_report_async(
    week_start: date = Query(...),
    current_user: User = Depends(get_current_user),
):
    task = generate_weekly_report_task.delay(
        current_user.id,
        week_start.isoformat(),
    )

    return TaskRead(
        task_id=task.id,
        status="PENDING",
    )


@router.get(
    "/tasks/{task_id}",
    response_model=TaskStatusRead,
)
def get_report_task_status(task_id: str):
    task_result = AsyncResult(
        task_id,
        app=celery_app,
    )

    result = task_result.result if task_result.successful() else None

    return TaskStatusRead(
        task_id=task_id,
        status=task_result.status,
        result=result,
    )


@router.get(
    "/weekly",
    response_model=list[WeeklyReportRead],
)
def get_weekly_reports(
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    query = (
        select(WeeklyReport)
        .where(WeeklyReport.user_id == current_user.id)
        .order_by(WeeklyReport.week_start.desc())
    )

    return db.scalars(query).all()