from datetime import date

from app.core.celery_app import celery_app
from app.core.database import SessionLocal
from app.services.report_service import generate_weekly_report


@celery_app.task(name="generate_weekly_report")
def generate_weekly_report_task(user_id: int, week_start: str) -> dict:
    db = SessionLocal()

    try:
        report = generate_weekly_report(
            db=db,
            user_id=user_id,
            week_start=date.fromisoformat(week_start),
        )

        return {
            "report_id": report.id,
            "user_id": report.user_id,
            "week_start": report.week_start.isoformat(),
            "week_end": report.week_end.isoformat(),
            "total_volume": report.total_volume,
            "workouts_count": report.workouts_count,
        }
    finally:
        db.close()