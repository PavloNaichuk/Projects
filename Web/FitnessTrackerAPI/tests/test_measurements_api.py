from tests.helpers import get_auth_headers


def test_create_measurement(client):
    headers = get_auth_headers(client)

    response = client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-05",
            "body_weight": 82.5,
            "body_fat": 18.5,
            "waist": 88,
        },
        headers=headers,
    )

    assert response.status_code == 201

    data = response.json()

    assert data["measured_at"] == "2026-07-05"
    assert data["body_weight"] == 82.5
    assert data["body_fat"] == 18.5
    assert data["waist"] == 88
    assert "id" in data
    assert "created_at" in data


def test_create_measurement_without_token(client):
    response = client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-05",
            "body_weight": 82.5,
            "body_fat": 18.5,
            "waist": 88,
        },
    )

    assert response.status_code == 401


def test_get_measurements(client):
    headers = get_auth_headers(client)

    client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-05",
            "body_weight": 82.5,
            "body_fat": 18.5,
            "waist": 88,
        },
        headers=headers,
    )

    client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-12",
            "body_weight": 81.8,
            "body_fat": 18.0,
            "waist": 87,
        },
        headers=headers,
    )

    response = client.get(
        "/measurements",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert len(data) == 2

    body_weights = [measurement["body_weight"] for measurement in data]

    assert 82.5 in body_weights
    assert 81.8 in body_weights


def test_get_latest_measurement(client):
    headers = get_auth_headers(client)

    client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-05",
            "body_weight": 82.5,
            "body_fat": 18.5,
            "waist": 88,
        },
        headers=headers,
    )

    client.post(
        "/measurements",
        json={
            "measured_at": "2026-07-12",
            "body_weight": 81.8,
            "body_fat": 18.0,
            "waist": 87,
        },
        headers=headers,
    )

    response = client.get(
        "/measurements/latest",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert data["measured_at"] == "2026-07-12"
    assert data["body_weight"] == 81.8
    assert data["body_fat"] == 18.0
    assert data["waist"] == 87