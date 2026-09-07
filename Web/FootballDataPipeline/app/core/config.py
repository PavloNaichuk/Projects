from functools import lru_cache
from pathlib import Path
from urllib.parse import quote

from pydantic import AnyHttpUrl, Field, SecretStr
from pydantic_settings import BaseSettings, SettingsConfigDict
from sqlalchemy.engine import URL

PROJECT_ROOT = Path(__file__).resolve().parents[2]


class Settings(BaseSettings):
    model_config = SettingsConfigDict(
        env_file=PROJECT_ROOT / ".env",
        env_file_encoding="utf-8",
        extra="ignore",
        hide_input_in_errors=True,
    )

    postgres_host: str = "127.0.0.1"
    postgres_port: int = Field(default=5433, ge=1, le=65535)
    postgres_db: str = "football"
    postgres_user: str = "football"
    postgres_password: SecretStr

    redis_host: str = "127.0.0.1"
    redis_port: int = Field(default=6380, ge=1, le=65535)
    redis_db: int = Field(default=0, ge=0)

    rabbitmq_host: str = "127.0.0.1"
    rabbitmq_port: int = Field(default=5673, ge=1, le=65535)
    rabbitmq_user: str = "football"
    rabbitmq_password: SecretStr

    api_football_base_url: AnyHttpUrl = "https://v3.football.api-sports.io"
    api_football_key: SecretStr
    api_football_timeout_seconds: float = Field(
        default=10.0,
        gt=0,
        le=60,
    )

    @property
    def database_url(self) -> URL:
        return URL.create(
            drivername="postgresql+asyncpg",
            username=self.postgres_user,
            password=self.postgres_password.get_secret_value(),
            host=self.postgres_host,
            port=self.postgres_port,
            database=self.postgres_db,
        )

    @property
    def celery_broker_url(self) -> str:
        username = quote(self.rabbitmq_user, safe="")
        password = quote(
            self.rabbitmq_password.get_secret_value(),
            safe="",
        )

        return (
            f"amqp://{username}:{password}@{self.rabbitmq_host}:{self.rabbitmq_port}//"
        )

    @property
    def celery_result_backend(self) -> str:
        return f"redis://{self.redis_host}:{self.redis_port}/{self.redis_db}"


@lru_cache(maxsize=1)
def get_settings() -> Settings:
    return Settings()
