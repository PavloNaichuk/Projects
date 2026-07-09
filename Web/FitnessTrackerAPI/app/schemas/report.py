from datetime import date, datetime

from pydantic import BaseModel


class WeeklyReportRead(BaseModel):
    id: int
    user_id: int
    week_start: date
    week_end: date
    workouts_count: int
    total_sets: int
    total_reps: int
    total_volume: float
    new_records_count: int
    generated_at: datetime

    model_config = {
        "from_attributes": True
    }