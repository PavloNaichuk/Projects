from datetime import datetime
from typing import TYPE_CHECKING

from sqlalchemy import (
    CheckConstraint,
    DateTime,
    ForeignKey,
    String,
)
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.season import Season
    from app.models.team import Team


class Fixture(TimestampMixin, Base):
    __tablename__ = "fixtures"
    __table_args__ = (
        CheckConstraint(
            "home_team_id <> away_team_id",
            name="ck_fixtures_different_teams",
        ),
    )

    id: Mapped[int] = mapped_column(primary_key=True)
    api_id: Mapped[int] = mapped_column(unique=True)

    season_id: Mapped[int] = mapped_column(
        ForeignKey("seasons.id", ondelete="CASCADE"),
        index=True,
    )
    home_team_id: Mapped[int] = mapped_column(
        ForeignKey("teams.id", ondelete="RESTRICT"),
        index=True,
    )
    away_team_id: Mapped[int] = mapped_column(
        ForeignKey("teams.id", ondelete="RESTRICT"),
        index=True,
    )

    referee: Mapped[str | None] = mapped_column(String(150))
    kickoff_at: Mapped[datetime] = mapped_column(
        DateTime(timezone=True),
        index=True,
    )
    timezone: Mapped[str] = mapped_column(String(50))
    round_name: Mapped[str | None] = mapped_column(String(100))

    venue_api_id: Mapped[int | None]
    venue_name: Mapped[str | None] = mapped_column(String(150))
    venue_city: Mapped[str | None] = mapped_column(String(100))

    status_long: Mapped[str] = mapped_column(String(50))
    status_short: Mapped[str] = mapped_column(
        String(10),
        index=True,
    )
    elapsed: Mapped[int | None]
    extra_time: Mapped[int | None]

    home_goals: Mapped[int | None]
    away_goals: Mapped[int | None]

    halftime_home: Mapped[int | None]
    halftime_away: Mapped[int | None]
    fulltime_home: Mapped[int | None]
    fulltime_away: Mapped[int | None]
    extra_home: Mapped[int | None]
    extra_away: Mapped[int | None]
    penalty_home: Mapped[int | None]
    penalty_away: Mapped[int | None]

    raw_payload: Mapped[dict[str, object]] = mapped_column(JSONB)

    season: Mapped["Season"] = relationship()
    home_team: Mapped["Team"] = relationship(
        foreign_keys=[home_team_id],
    )
    away_team: Mapped["Team"] = relationship(
        foreign_keys=[away_team_id],
    )
