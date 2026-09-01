from sqlalchemy import Boolean, String, false
from sqlalchemy.orm import Mapped, mapped_column

from app.db.base import Base
from app.db.mixins import TimestampMixin


class Team(TimestampMixin, Base):
    __tablename__ = "teams"

    id: Mapped[int] = mapped_column(primary_key=True)
    api_id: Mapped[int] = mapped_column(unique=True)
    name: Mapped[str] = mapped_column(String(150))
    code: Mapped[str | None] = mapped_column(String(10))
    country_name: Mapped[str | None] = mapped_column(String(100))
    founded: Mapped[int | None]
    is_national: Mapped[bool] = mapped_column(
        Boolean,
        default=False,
        server_default=false(),
    )
    logo_url: Mapped[str | None] = mapped_column(String(500))
