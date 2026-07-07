from datetime import date

from fastapi import APIRouter, Depends, HTTPException, Query, status
from sqlalchemy import select
from sqlalchemy.orm import Session

from app.api.deps import get_current_user
from app.core.database import get_db
from app.models.measurement import BodyMeasurement
from app.models.user import User
from app.schemas.measurement import BodyMeasurementCreate, BodyMeasurementRead


router = APIRouter(
    prefix="/measurements",
    tags=["Measurements"],
)


@router.post(
    "",
    response_model=BodyMeasurementRead,
    status_code=status.HTTP_201_CREATED,
)
def create_measurement(
    measurement_data: BodyMeasurementCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    measurement = BodyMeasurement(
        user_id=current_user.id,
        measured_at=measurement_data.measured_at,
        body_weight=measurement_data.body_weight,
        body_fat=measurement_data.body_fat,
        waist=measurement_data.waist,
    )

    db.add(measurement)
    db.commit()
    db.refresh(measurement)

    return measurement


@router.get(
    "",
    response_model=list[BodyMeasurementRead],
)
def get_measurements(
    date_from: date | None = Query(default=None),
    date_to: date | None = Query(default=None),
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    query = (
        select(BodyMeasurement)
        .where(BodyMeasurement.user_id == current_user.id)
        .order_by(BodyMeasurement.measured_at.desc(), BodyMeasurement.id.desc())
    )

    if date_from:
        query = query.where(BodyMeasurement.measured_at >= date_from)

    if date_to:
        query = query.where(BodyMeasurement.measured_at <= date_to)

    return db.scalars(query).all()


@router.get(
    "/latest",
    response_model=BodyMeasurementRead,
)
def get_latest_measurement(
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    measurement = db.scalar(
        select(BodyMeasurement)
        .where(BodyMeasurement.user_id == current_user.id)
        .order_by(BodyMeasurement.measured_at.desc(), BodyMeasurement.id.desc())
    )

    if measurement is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="No body measurements found",
        )

    return measurement