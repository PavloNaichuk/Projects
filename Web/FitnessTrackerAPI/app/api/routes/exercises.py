from fastapi import APIRouter, Depends, HTTPException, Query, status
from sqlalchemy import select
from sqlalchemy.orm import Session

from app.api.deps import get_current_user
from app.core.database import get_db
from app.models.exercise import Exercise
from app.models.user import User
from app.schemas.exercise import ExerciseCreate, ExerciseRead


router = APIRouter(
    prefix="/exercises",
    tags=["Exercises"],
)


@router.post(
    "",
    response_model=ExerciseRead,
    status_code=status.HTTP_201_CREATED,
)
def create_exercise(
    exercise_data: ExerciseCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    existing_exercise = db.scalar(
        select(Exercise).where(Exercise.name == exercise_data.name)
    )

    if existing_exercise:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Exercise already exists",
        )

    exercise = Exercise(
        name=exercise_data.name,
        muscle_group=exercise_data.muscle_group,
        equipment=exercise_data.equipment,
    )

    db.add(exercise)
    db.commit()
    db.refresh(exercise)

    return exercise


@router.get(
    "",
    response_model=list[ExerciseRead],
)
def get_exercises(
    muscle_group: str | None = Query(default=None),
    equipment: str | None = Query(default=None),
    db: Session = Depends(get_db),
):
    query = select(Exercise)

    if muscle_group:
        query = query.where(Exercise.muscle_group == muscle_group)

    if equipment:
        query = query.where(Exercise.equipment == equipment)

    return db.scalars(query).all()


@router.get(
    "/{exercise_id}",
    response_model=ExerciseRead,
)
def get_exercise(
    exercise_id: int,
    db: Session = Depends(get_db),
):
    exercise = db.get(Exercise, exercise_id)

    if exercise is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Exercise not found",
        )

    return exercise