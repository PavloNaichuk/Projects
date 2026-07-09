def get_auth_headers(client):
    client.post(
        "/auth/register",
        json={
            "email": "test@example.com",
            "username": "testuser",
            "password": "password123",
        },
    )

    login_response = client.post(
        "/auth/login",
        data={
            "username": "testuser",
            "password": "password123",
        },
    )

    token = login_response.json()["access_token"]

    return {
        "Authorization": f"Bearer {token}",
    }


def create_exercise(client, headers):
    response = client.post(
        "/exercises",
        json={
            "name": "Bench Press",
            "muscle_group": "Chest",
            "equipment": "Barbell",
        },
        headers=headers,
    )

    return response.json()["id"]


def create_workout(client, headers, exercise_id):
    response = client.post(
        "/workouts",
        json={
            "title": "Chest Day",
            "workout_date": "2026-07-05",
            "notes": "Bench press training",
            "sets": [
                {
                    "exercise_id": exercise_id,
                    "set_number": 1,
                    "weight": 80,
                    "reps": 10,
                },
                {
                    "exercise_id": exercise_id,
                    "set_number": 2,
                    "weight": 85,
                    "reps": 8,
                },
            ],
        },
        headers=headers,
    )

    return response.json()["id"]


def test_generate_weekly_report(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)
    create_workout(client, headers, exercise_id)

    response = client.post(
        "/reports/weekly/generate?week_start=2026-07-05",
        headers=headers,
    )

    assert response.status_code == 201

    data = response.json()

    assert data["week_start"] == "2026-07-05"
    assert data["week_end"] == "2026-07-11"
    assert data["workouts_count"] == 1
    assert data["total_sets"] == 2
    assert data["total_reps"] == 18
    assert data["total_volume"] == 1480
    assert data["new_records_count"] == 0


def test_generate_weekly_report_without_token(client):
    response = client.post("/reports/weekly/generate?week_start=2026-07-05")

    assert response.status_code == 401


def test_get_weekly_reports(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)
    create_workout(client, headers, exercise_id)

    client.post(
        "/reports/weekly/generate?week_start=2026-07-05",
        headers=headers,
    )

    response = client.get(
        "/reports/weekly",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert len(data) == 1
    assert data[0]["week_start"] == "2026-07-05"
    assert data[0]["total_volume"] == 1480


def test_get_weekly_reports_empty(client):
    headers = get_auth_headers(client)

    response = client.get(
        "/reports/weekly",
        headers=headers,
    )

    assert response.status_code == 200
    assert response.json() == []


def test_generate_weekly_report_updates_existing_report(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)
    create_workout(client, headers, exercise_id)

    first_response = client.post(
        "/reports/weekly/generate?week_start=2026-07-05",
        headers=headers,
    )

    second_response = client.post(
        "/reports/weekly/generate?week_start=2026-07-05",
        headers=headers,
    )

    assert first_response.status_code == 201
    assert second_response.status_code == 201

    first_data = first_response.json()
    second_data = second_response.json()

    assert first_data["id"] == second_data["id"]
    assert second_data["total_volume"] == 1480
    assert second_data["workouts_count"] == 1