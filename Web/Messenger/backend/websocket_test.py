import asyncio
import json

import websockets


async def main():
    uri = "ws://127.0.0.1:8000/ws/conversations/1/"

    async with websockets.connect(uri) as websocket:
        print("Connected to WebSocket")

        await websocket.send(
            json.dumps(
                {
                    "message": "Hello from WebSocket test"
                }
            )
        )

        response = await websocket.recv()
        print("Received:", response)


asyncio.run(main())