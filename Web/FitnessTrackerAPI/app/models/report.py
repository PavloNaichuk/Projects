from datetime import date, datetime

from sqlalchemy import Date, DateTime, Float, ForeignKey, Integer, UniqueConstraint, func
from sqlalchemy.orm import Mapped, mapped_column

from app.core.database import Base


class WeeklyReport(Base):
    __tablename__ = "weekly_reports"

    __table_args__ = (
        UniqueConstraint(
            "user_id",
            "week_start",
            name="uq_weekly_report_user_week",
        ),
    )

    id: Mapped[int] = mapped_column(primary_key=True, index=True)

    user_id: Mapped[int] = mapped_column(
        ForeignKey("users.id", ondelete="CASCADE"),
        index=True,
        nullable=False,
    )

    week_start: Mapped[date] = mapped_column(
        Date,
        index=True,
        nullable=False,
    )

    week_end: Mapped[date] = mapped_column(
        Date,
        nullable=False,
    )

    workouts_count: Mapped[int] = mapped_column(
        Integer,
        nullable=False,
        default=0,
    )

    total_sets: Mapped[int] = mapped_column(
        Integer,
        nullable=False,
        default=0,
    )

    total_reps: Mapped[int] = mapped_column(
        Integer,
        nullable=False,
        default=0,
    )

    total_volume: Mapped[float] = mapped_column(
        Float,
        nullable=False,
        default=0,
    )

    new_records_count: Mapped[int] = mapped_column(
        Integer,
        nullable=False,
        default=0,
    )

    generated_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True),
        server_default=func.now(),
        nullable=False,
    )