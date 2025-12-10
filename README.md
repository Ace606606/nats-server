# nats-server

nats.c (Официальный клиент на C)

Производительность: Это нативная библиотека, написанная на чистом C. Она обеспечивает минимальные накладные расходы, что критично для HighLoad систем, где обычно и применяют C++.

Поддержка: Это официальная библиотека, поддерживаемая командой NATS (Synadia). Все новые фичи (например, JetStream, который заменяет очереди RabbitMQ) появляются здесь сразу.

Стабильность ABI: C-интерфейс стабилен.

C++ совместимость: Любой C++ проект легко линкуется с C. Часто опытные C++ разработчики предпочитают написать свою тонкую RAII-обертку (используя std::unique_ptr для хендлов) вокруг nats.c, чем использовать сторонние тяжелые C++ библиотеки.

git clone https://github.com/nats-io/nats.c.git
cd nats.c
mkdir build && cd build
cmake .. -DNATS_BUILD_STREAMING=OFF # Streaming устарел, Core + JetStream
make
sudo make install

#include <nats/nats.h>


nats-py

Асинхронность (AsyncIO): NATS по своей природе очень быстр. Блокирующий код в Python убьет всю производительность. nats-py построен на базе asyncio, что является стандартом для современного сетевого Python.

Официальный статус: Это клиент, поддерживаемый разработчиками NATS.

JetStream: Полная поддержка JetStream (persistence layer), что важно при миграции с RabbitMQ.

pip install nats-py

# RUN
./nats-server -c server.conf

# LIBS
./config --prefix=$HOME/.local --openssldir=$HOME/.local/openssl \
         no-tests shared zlib
make -j$(nproc)
make install

mkdir build && cd build
cmake .. \
  -DOPENSSL_ROOT_DIR=$HOME/.local \
  -DOPENSSL_CRYPTO_LIBRARY=$HOME/.local/lib/libcrypto.so \
  -DOPENSSL_SSL_LIBRARY=$HOME/.local/lib/libssl.so \
  -DOPENSSL_INCLUDE_DIR=$HOME/.local/include
