from datetime import date, datetime

from pydantic import BaseModel, Field


class WorkoutSetCreate(BaseModel):
    exercise_id: int
    set_number: int = Field(ge=1)
    weight: float = Field(ge=0)
    reps: int = Field(ge=1)


class WorkoutCreate(BaseModel):
    title: str = Field(min_length=2, max_length=150)
    workout_date: date
    notes: str | None = None
    sets: list[WorkoutSetCreate] = []


class WorkoutSetRead(BaseModel):
    id: int
    exercise_id: int
    set_number: int
    weight: float
    reps: int
    created_at: datetime

    model_config = {
        "from_attributes": True
    }


class WorkoutRead(BaseModel):
    id: int
    user_id: int
    title: str
    workout_date: date
    notes: str | None
    created_at: datetime
    sets: list[WorkoutSetRead]

    model_config = {
        "from_attributes": True
    }