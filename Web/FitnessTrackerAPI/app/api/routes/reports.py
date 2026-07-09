from datetime import date

from fastapi import APIRouter, Depends, Query, status
from sqlalchemy import select
from sqlalchemy.orm import Session

from app.api.deps import get_current_user
from app.core.database import get_db
from app.models.report import WeeklyReport
from app.models.user import User
from app.schemas.report import WeeklyReportRead
from app.services.report_service import generate_weekly_report


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