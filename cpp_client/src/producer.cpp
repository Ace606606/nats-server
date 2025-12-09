#include <iostream>
#include <cstring>
#include <nats.h>

void checkStatus(natsStatus s, const char* msg)
{
    if (s != NATS_OK) {
        std::cerr << "Ошибка [" << msg << "]: "
                  << natsStatus_GetText(s) << std::endl;
        exit(1);
    }
}

int main()
{
    natsConnection* nc = nullptr;
    jsCtx* js          = nullptr;
    jsPubOptions pubOpts{};
    jsPubAck* pa       = nullptr;
    natsStatus s;

    std::cout << "Подключение к NATS...\n";
    s = natsConnection_ConnectTo(&nc, "nats://service_a:password_a@localhost:4222");
    checkStatus(s, "Connect");

    std::cout << "Получаем JetStream контекст...\n";
    s = natsConnection_JetStream(&js, nc, nullptr);   // 1-ым идёт **jsCtx**
    checkStatus(s, "JetStream");

    const char* subject = "updates.s1";
    const char* data    = "Hello from C++!";

    s = jsPubOptions_Init(&pubOpts);
    checkStatus(s, "InitPubOpts");

    std::cout << "Публикуем...\n";
    s = js_Publish(&pa, js, subject, data, (int)std::strlen(data),
                   &pubOpts, nullptr);   // 1-ым идёт **jsPubAck**
    checkStatus(s, "Publish");

    std::cout << "Сообщение отправлено! Seq: " << pa->Sequence << '\n';

    jsPubAck_Destroy(pa);
    jsCtx_Destroy(js);
    natsConnection_Destroy(nc);
    return 0;
}