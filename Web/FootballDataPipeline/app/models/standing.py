from datetime import datetime
from typing import TYPE_CHECKING

from sqlalchemy import (
    CheckConstraint,
    DateTime,
    ForeignKey,
    String,
    UniqueConstraint,
)
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.season import Season
    from app.models.team import Team


class Standing(TimestampMixin, Base):
    __tablename__ = "standings"
    __table_args__ = (
        UniqueConstraint(
            "season_id",
            "team_id",
            name="uq_standings_season_id_team_id",
        ),
        CheckConstraint(
            "position > 0",
            name="ck_standings_positive_position",
        ),
        CheckConstraint(
            (
                "played >= 0 AND wins >= 0 AND draws >= 0 "
                "AND losses >= 0 AND goals_for >= 0 "
                "AND goals_against >= 0"
            ),
            name="ck_standings_nonnegative_stats",
        ),
    )

    id: Mapped[int] = mapped_column(primary_key=True)

    season_id: Mapped[int] = mapped_column(
        ForeignKey(
            "seasons.id",
            ondelete="CASCADE",
        ),
        index=True,
    )
    team_id: Mapped[int] = mapped_column(
        ForeignKey(
            "teams.id",
            ondelete="RESTRICT",
        ),
        index=True,
    )

    position: Mapped[int]
    points: Mapped[int]
    goals_diff: Mapped[int]

    group_name: Mapped[str | None] = mapped_column(
        String(100),
    )
    form: Mapped[str | None] = mapped_column(
        String(100),
    )
    status: Mapped[str | None] = mapped_column(
        String(50),
    )
    description: Mapped[str | None] = mapped_column(
        String(255),
    )

    played: Mapped[int]
    wins: Mapped[int]
    draws: Mapped[int]
    losses: Mapped[int]
    goals_for: Mapped[int]
    goals_against: Mapped[int]

    home_stats: Mapped[dict[str, object]] = mapped_column(
        JSONB,
    )
    away_stats: Mapped[dict[str, object]] = mapped_column(
        JSONB,
    )
    raw_payload: Mapped[dict[str, object]] = mapped_column(
        JSONB,
    )

    source_updated_at: Mapped[datetime | None] = mapped_column(
        DateTime(timezone=True),
    )

    season: Mapped["Season"] = relationship()
    team: Mapped["Team"] = relationship()
