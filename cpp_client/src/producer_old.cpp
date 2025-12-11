#include <nats.h>
#include <iostream>

void
checkStatus(natsStatus status, const char* msg)
{
    if (status != NATS_OK)
    {
        std::cerr << " Ошибка [" << msg << "]: "
                  << natsStatus_GetText(status) << '\n';

        nats_Close();
        std::exit(1);
    }
}



int main()
{
    natsConnection* nc = nullptr;
    natsStatus status;

    const char* url = "nats://service_a:password_a@localhost:4222";

    status = natsConnection_ConnectTo(&nc, url);

    checkStatus(status, "Connect");

    status = natsConnection_PublishString(nc, "updates.s2", "hello world");

    checkStatus(status, "Publish");

    if (nc != nullptr)
    {
        natsConnection_Destroy(nc);
        std::cout << "Connection close success.\n";
    }

    nats_Close();
    return 0;
}