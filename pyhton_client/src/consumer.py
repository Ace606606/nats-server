import asyncio
import nats
from nats.errors import TimeoutError

async def main():
    nc = await nats.connect("nats://service_a:password_a@localhost:4222")

    js = nc.jetstream()

    psub = await js.pull_subscribe("events.test", durable="my_consumer")
    print("Ожидание сообщение...")

    while True:
        try:
            msgs = await psub.fetch(1, timeout=5)
            for msg in msgs:
                data = msg.data.decode()
                print(f"Получено сообщение: {data}")
                await msg.ack()
        
        except TimeoutError:
            pass
        except KeyboardInterrupt:
            break

    await nc.close()

if __name__ == '__main__':
    asyncio.run(main())