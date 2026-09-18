from typing import TYPE_CHECKING

from sqlalchemy import ForeignKey, String, UniqueConstraint
from sqlalchemy.dialects.postgresql import JSONB
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.fixture import Fixture
    from app.models.team import Team


class FixtureLineup(TimestampMixin, Base):
    __tablename__ = "fixture_lineups"
    __table_args__ = (
        UniqueConstraint(
            "fixture_id",
            "team_id",
            name="uq_fixture_lineups_fixture_id_team_id",
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

    formation: Mapped[str | None] = mapped_column(String(20))

    coach_api_id: Mapped[int | None]
    coach_name: Mapped[str | None] = mapped_column(String(150))
    coach_photo_url: Mapped[str | None] = mapped_column(
        String(500),
    )

    colors: Mapped[dict[str, object] | None] = mapped_column(
        JSONB,
    )
    starting_xi: Mapped[list[dict[str, object]]] = mapped_column(
        JSONB,
    )
    substitutes: Mapped[list[dict[str, object]]] = mapped_column(
        JSONB,
    )
    raw_payload: Mapped[dict[str, object]] = mapped_column(JSONB)

    fixture: Mapped["Fixture"] = relationship(
        back_populates="team_lineups",
    )
    team: Mapped["Team"] = relationship()
