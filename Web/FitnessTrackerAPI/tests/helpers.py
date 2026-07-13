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


def create_exercise(
    client,
    headers,
    name="Bench Press",
    muscle_group="Chest",
    equipment="Barbell",
):
    response = client.post(
        "/exercises",
        json={
            "name": name,
            "muscle_group": muscle_group,
            "equipment": equipment,
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
