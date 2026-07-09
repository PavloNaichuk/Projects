from datetime import date, timedelta

from sqlalchemy import func, select
from sqlalchemy.orm import Session

from app.models.report import WeeklyReport
from app.models.workout import Workout, WorkoutSet
from app.services.progress_service import calculate_set_volume


def generate_weekly_report(
    db: Session,
    user_id: int,
    week_start: date,
) -> WeeklyReport:
    week_end = week_start + timedelta(days=6)

    workouts_count = db.scalar(
        select(func.count(Workout.id)).where(
            Workout.user_id == user_id,
            Workout.workout_date >= week_start,
            Workout.workout_date <= week_end,
        )
    )

    workout_sets = db.scalars(
        select(WorkoutSet)
        .join(Workout)
        .where(
            Workout.user_id == user_id,
            Workout.workout_date >= week_start,
            Workout.workout_date <= week_end,
        )
    ).all()

    total_sets = len(workout_sets)
    total_reps = sum(workout_set.reps for workout_set in workout_sets)
    total_volume = sum(calculate_set_volume(workout_set) for workout_set in workout_sets)

    report = db.scalar(
        select(WeeklyReport).where(
            WeeklyReport.user_id == user_id,
            WeeklyReport.week_start == week_start,
        )
    )

    if report is None:
        report = WeeklyReport(
            user_id=user_id,
            week_start=week_start,
            week_end=week_end,
        )

    report.workouts_count = workouts_count or 0
    report.total_sets = total_sets
    report.total_reps = total_reps
    report.total_volume = total_volume
    report.new_records_count = 0

    db.add(report)
    db.commit()
    db.refresh(report)

    return report