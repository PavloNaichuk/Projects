from typing import TYPE_CHECKING

from sqlalchemy import ForeignKey, UniqueConstraint
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.fixture import Fixture
    from app.models.team import Team


class FixtureStatistic(TimestampMixin, Base):
    __tablename__ = "fixture_statistics"
    __table_args__ = (
        UniqueConstraint(
            "fixture_id",
            "team_id",
            name="uq_fixture_statistics_fixture_id_team_id",
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

    statistics: Mapped[dict[str, object]] = mapped_column(JSONB)
    raw_payload: Mapped[dict[str, object]] = mapped_column(JSONB)

    fixture: Mapped["Fixture"] = relationship(
        back_populates="team_statistics",
    )
    team: Mapped["Team"] = relationship()
