from datetime import date, timedelta

from fastapi import APIRouter, Depends, HTTPException, Query, status
from sqlalchemy import func, select
from sqlalchemy.orm import Session

from app.api.deps import get_current_user
from app.core.database import get_db
from app.models.exercise import Exercise
from app.models.user import User
from app.models.workout import Workout, WorkoutSet
from app.schemas.progress import ExerciseRecordsRead, WeeklyProgressRead
from app.services.progress_service import (
    calculate_set_volume,
    estimate_one_rep_max,
)


router = APIRouter(
    prefix="/progress",
    tags=["Progress"],
)


@router.get(
    "/weekly",
    response_model=WeeklyProgressRead,
)
def get_weekly_progress(
    week_start: date = Query(...),
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    week_end = week_start + timedelta(days=6)

    workouts_count = db.scalar(
        select(func.count(Workout.id)).where(
            Workout.user_id == current_user.id,
            Workout.workout_date >= week_start,
            Workout.workout_date <= week_end,
        )
    )

    workout_sets = db.scalars(
        select(WorkoutSet)
        .join(Workout)
        .where(
            Workout.user_id == current_user.id,
            Workout.workout_date >= week_start,
            Workout.workout_date <= week_end,
        )
    ).all()

    total_sets = len(workout_sets)
    total_reps = sum(workout_set.reps for workout_set in workout_sets)
    total_volume = sum(calculate_set_volume(workout_set) for workout_set in workout_sets)

    return WeeklyProgressRead(
        week_start=week_start,
        week_end=week_end,
        workouts_count=workouts_count or 0,
        total_sets=total_sets,
        total_reps=total_reps,
        total_volume=total_volume,
    )


@router.get(
    "/exercises/{exercise_id}/records",
    response_model=ExerciseRecordsRead,
)
def get_exercise_records(
    exercise_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    exercise = db.get(Exercise, exercise_id)

    if exercise is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Exercise not found",
        )

    workout_sets = db.scalars(
        select(WorkoutSet)
        .join(Workout)
        .where(
            Workout.user_id == current_user.id,
            WorkoutSet.exercise_id == exercise_id,
        )
    ).all()

    if not workout_sets:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="No workout sets found for this exercise",
        )

    max_weight = max(workout_set.weight for workout_set in workout_sets)
    max_reps = max(workout_set.reps for workout_set in workout_sets)
    max_volume = max(calculate_set_volume(workout_set) for workout_set in workout_sets)
    estimated_one_rep_max = max(
        estimate_one_rep_max(workout_set.weight, workout_set.reps)
        for workout_set in workout_sets
    )

    return ExerciseRecordsRead(
        exercise_id=exercise_id,
        max_weight=max_weight,
        max_reps=max_reps,
        max_volume=max_volume,
        estimated_one_rep_max=round(estimated_one_rep_max, 2),
    )