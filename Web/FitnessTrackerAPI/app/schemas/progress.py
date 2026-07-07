from datetime import date

from pydantic import BaseModel


class WeeklyProgressRead(BaseModel):
    week_start: date
    week_end: date
    workouts_count: int
    total_sets: int
    total_reps: int
    total_volume: float


class ExerciseRecordsRead(BaseModel):
    exercise_id: int
    max_weight: float
    max_reps: int
    max_volume: float
    estimated_one_rep_max: float