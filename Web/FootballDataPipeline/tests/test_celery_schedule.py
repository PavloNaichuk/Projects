from app.tasks.celery_app import (
    build_beat_schedule,
    celery_app,
    settings,
)


def test_periodic_sync_schedule() -> None:
    schedule = celery_app.conf.beat_schedule

    fixture_schedule = schedule["sync-fixtures-periodically"]
    standing_schedule = schedule["sync-standings-periodically"]

    assert fixture_schedule["task"] == ("football.sync_fixtures")
    assert fixture_schedule["schedule"] == (settings.fixture_sync_interval_seconds)
    assert fixture_schedule["args"] == (
        settings.sync_league_id,
        settings.sync_season,
    )

    assert standing_schedule["task"] == ("football.sync_standings")
    assert standing_schedule["schedule"] == (settings.standing_sync_interval_seconds)
    assert standing_schedule["args"] == (
        settings.sync_league_id,
        settings.sync_season,
    )


def test_live_sync_is_not_scheduled_when_disabled() -> None:
    disabled_settings = settings.model_copy(
        update={
            "live_fixture_sync_enabled": False,
        },
    )

    schedule = build_beat_schedule(
        disabled_settings,
    )

    assert "sync-live-fixtures-periodically" not in schedule


def test_live_sync_is_scheduled_when_enabled() -> None:
    enabled_settings = settings.model_copy(
        update={
            "live_fixture_sync_enabled": True,
            "live_fixture_sync_interval_seconds": 30,
        },
    )

    schedule = build_beat_schedule(
        enabled_settings,
    )
    live_schedule = schedule["sync-live-fixtures-periodically"]

    assert live_schedule["task"] == ("football.sync_live_fixtures")
    assert live_schedule["schedule"] == 30
    assert live_schedule["args"] == (
        settings.sync_league_id,
        settings.sync_season,
    )
