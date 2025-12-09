import asyncio
import nats
from nats.errors import TimeoutError

async def main():
    nc = await nats.connect("nats://service_a:password_a@localhost:4222")

    js = nc.jetstream()

    try:
        await js.add_stream(name="STREAM", subjects=["updates.*"])
        print("Stream 'STREAM' создан или уже существует.")
    except Exception as e:
        print(f"Ошибка при создание стрима: {e}")

    psub = await js.pull_subscribe("updates.s1", durable="my_consumer")
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