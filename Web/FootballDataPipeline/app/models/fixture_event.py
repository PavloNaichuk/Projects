from typing import TYPE_CHECKING

from sqlalchemy import (
    CheckConstraint,
    ForeignKey,
    String,
    Text,
    UniqueConstraint,
)
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.fixture import Fixture
    from app.models.team import Team


class FixtureEvent(TimestampMixin, Base):
    __tablename__ = "fixture_events"
    __table_args__ = (
        UniqueConstraint(
            "fixture_id",
            "event_order",
            name="uq_fixture_events_fixture_id_event_order",
        ),
        CheckConstraint(
            "event_order >= 0",
            name="ck_fixture_events_nonnegative_order",
        ),
        CheckConstraint(
            "elapsed >= 0",
            name="ck_fixture_events_nonnegative_elapsed",
        ),
        CheckConstraint(
            "extra_time IS NULL OR extra_time >= 0",
            name="ck_fixture_events_nonnegative_extra_time",
        ),
    )

    id: Mapped[int] = mapped_column(primary_key=True)

    fixture_id: Mapped[int] = mapped_column(
        ForeignKey("fixtures.id", ondelete="CASCADE"),
        index=True,
    )
    team_id: Mapped[int] = mapped_column(
        ForeignKey("teams.id", ondelete="RESTRICT"),
        index=True,
    )

    event_order: Mapped[int]
    elapsed: Mapped[int]
    extra_time: Mapped[int | None]

    player_api_id: Mapped[int | None]
    player_name: Mapped[str | None] = mapped_column(String(150))
    assist_api_id: Mapped[int | None]
    assist_name: Mapped[str | None] = mapped_column(String(150))

    event_type: Mapped[str] = mapped_column(
        String(50),
        index=True,
    )
    detail: Mapped[str] = mapped_column(String(100))
    comments: Mapped[str | None] = mapped_column(Text)

    raw_payload: Mapped[dict[str, object]] = mapped_column(JSONB)

    fixture: Mapped["Fixture"] = relationship(
        back_populates="events",
    )
    team: Mapped["Team"] = relationship()
