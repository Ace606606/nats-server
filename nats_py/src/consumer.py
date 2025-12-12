import asyncio
from os import wait
import nats
from nats.errors import TimeoutError 

url: str = "nats://@localhost:4222"
subject: str = "test.subject"

async def main():
    nc = await nats.connect(url)
    print(f"Connected to NATS at {url}")

    sub = await nc.subscribe(subject)

    print(f"Listening on subject: {subject}")
    print("Waiting for message from C++...")
    print("-" * 50)

    try:
        while True:
            try:
                msg = await sub.next_msg(timeout=10)

                print(f"Received: {msg.data.decode()}")
                print(f"Subject: {msg.subject}")
                print(f"Reply: {msg.reply}")
                print(f"-" * 30)
            except TimeoutError:
                print("No messages received for 10 seconds, still listening...")
    except KeyboardInterrupt:
        print("\nShuting down...")

    finally:
        await sub.unsubscribe()
        await nc.drain()
        await nc.close()


if __name__ == '__main__':
    asyncio.run(main())