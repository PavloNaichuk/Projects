import asyncio

from app.clients.api_football import create_api_football_client


async def main() -> None:
    async with create_api_football_client() as client:
        payload = await client.get("/status")

    subscription = payload["response"]["subscription"]
    usage = payload["response"]["requests"]

    print("API-Football connection OK")
    print(f"Plan: {subscription['plan']}")
    print(f"Active: {subscription['active']}")
    print(f"Requests used: {usage['current']}/{usage['limit_day']}")


if __name__ == "__main__":
    asyncio.run(main())
