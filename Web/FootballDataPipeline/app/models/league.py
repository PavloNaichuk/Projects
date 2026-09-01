from typing import TYPE_CHECKING

from sqlalchemy import String
from sqlalchemy.orm import Mapped, mapped_column, relationship

from app.db.base import Base
from app.db.mixins import TimestampMixin

if TYPE_CHECKING:
    from app.models.season import Season


class League(TimestampMixin, Base):
    __tablename__ = "leagues"

    id: Mapped[int] = mapped_column(primary_key=True)
    api_id: Mapped[int] = mapped_column(unique=True)
    name: Mapped[str] = mapped_column(String(150))
    league_type: Mapped[str] = mapped_column(String(50))
    country_name: Mapped[str | None] = mapped_column(String(100))
    country_code: Mapped[str | None] = mapped_column(String(10))
    logo_url: Mapped[str | None] = mapped_column(String(500))
    country_flag_url: Mapped[str | None] = mapped_column(String(500))

    seasons: Mapped[list["Season"]] = relationship(
        back_populates="league",
        cascade="all, delete-orphan",
        passive_deletes=True,
    )
