from datetime import date

from fastapi import APIRouter, Depends, HTTPException, Query, status
from sqlalchemy import select
from sqlalchemy.orm import Session, selectinload

from app.api.deps import get_current_user
from app.core.database import get_db
from app.models.exercise import Exercise
from app.models.user import User
from app.models.workout import Workout, WorkoutSet
from app.schemas.workout import WorkoutCreate, WorkoutRead


router = APIRouter(
    prefix="/workouts",
    tags=["Workouts"],
)


@router.post(
    "",
    response_model=WorkoutRead,
    status_code=status.HTTP_201_CREATED,
)
def create_workout(
    workout_data: WorkoutCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    workout = Workout(
        user_id=current_user.id,
        title=workout_data.title,
        workout_date=workout_data.workout_date,
        notes=workout_data.notes,
    )

    db.add(workout)
    db.flush()

    for set_data in workout_data.sets:
        exercise = db.get(Exercise, set_data.exercise_id)

        if exercise is None:
            raise HTTPException(
                status_code=status.HTTP_404_NOT_FOUND,
                detail=f"Exercise with id {set_data.exercise_id} not found",
            )

        workout_set = WorkoutSet(
            workout_id=workout.id,
            exercise_id=set_data.exercise_id,
            set_number=set_data.set_number,
            weight=set_data.weight,
            reps=set_data.reps,
        )

        db.add(workout_set)

    db.commit()
    db.refresh(workout)

    return workout


@router.get(
    "",
    response_model=list[WorkoutRead],
)
def get_workouts(
    date_from: date | None = Query(default=None),
    date_to: date | None = Query(default=None),
    exercise_id: int | None = Query(default=None),
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    query = (
        select(Workout)
        .where(Workout.user_id == current_user.id)
        .options(selectinload(Workout.sets))
        .order_by(Workout.workout_date.desc(), Workout.id.desc())
    )

    if date_from:
        query = query.where(Workout.workout_date >= date_from)

    if date_to:
        query = query.where(Workout.workout_date <= date_to)

    if exercise_id:
        query = (
            query.join(WorkoutSet)
            .where(WorkoutSet.exercise_id == exercise_id)
            .distinct()
        )

    return db.scalars(query).all()


@router.get(
    "/{workout_id}",
    response_model=WorkoutRead,
)
def get_workout(
    workout_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    workout = db.scalar(
        select(Workout)
        .where(
            Workout.id == workout_id,
            Workout.user_id == current_user.id,
        )
        .options(selectinload(Workout.sets))
    )

    if workout is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Workout not found",
        )

    return workout


@router.delete(
    "/{workout_id}",
    status_code=status.HTTP_204_NO_CONTENT,
)
def delete_workout(
    workout_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    workout = db.scalar(
        select(Workout).where(
            Workout.id == workout_id,
            Workout.user_id == current_user.id,
        )
    )

    if workout is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Workout not found",
        )

    db.delete(workout)
    db.commit()