from datetime import date, datetime

from pydantic import BaseModel, Field


class BodyMeasurementCreate(BaseModel):
    measured_at: date
    body_weight: float = Field(gt=0)
    body_fat: float | None = Field(default=None, ge=0, le=100)
    waist: float | None = Field(default=None, gt=0)


class BodyMeasurementRead(BaseModel):
    id: int
    user_id: int
    measured_at: date
    body_weight: float
    body_fat: float | None
    waist: float | None
    created_at: datetime

    model_config = {
        "from_attributes": True
    }