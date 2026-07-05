from datetime import datetime

from pydantic import BaseModel, Field


class ExerciseCreate(BaseModel):
    name: str = Field(min_length=2, max_length=150)
    muscle_group: str = Field(min_length=2, max_length=100)
    equipment: str | None = Field(default=None, max_length=100)


class ExerciseRead(BaseModel):
    id: int
    name: str
    muscle_group: str
    equipment: str | None
    created_at: datetime

    model_config = {
        "from_attributes": True
    }