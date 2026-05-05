import asyncio
import json

import requests
import websockets


USERNAME = "pavlo"
PASSWORD = "pavlik123"


def get_access_token():
    response = requests.post(
        "http://127.0.0.1:8000/api/auth/token/",
        json={
            "username": USERNAME,
            "password": PASSWORD,
        },
    )
    response.raise_for_status()
    return response.json()["access"]


async def main():
    access_token = get_access_token()

    uri = f"ws://127.0.0.1:8000/ws/conversations/1/?token={access_token}"

    async with websockets.connect(uri) as websocket:
        print("Connected to WebSocket with JWT")

        await websocket.send(
            json.dumps(
                {
                    "message": "Hello from authenticated WebSocket"
                }
            )
        )

        response = await websocket.recv()
        print("Received:", response)


asyncio.run(main())