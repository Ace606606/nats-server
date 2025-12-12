#include <iostream>
#include <nats.h>
#include <string>
#include <chrono>
#include <thread>

static volatile bool drained = false;


const char* url = "nats://@localhost:4222";
const char* subject = "test.subject";
const char* reply = NULL;

void
onClosed(natsConnection *, void *)
{
    drained = true;
}

void
check_status(natsStatus status, const char* msg)
{
    if (status != NATS_OK)
    {
        std::cerr << "Error [ " << msg << " ]: "
                << natsStatus_GetText(status);
    } else {
        std::cout << "Info [ " << msg << " ]: " << "success" << std::endl;
        return;
    }

    nats_Close();
    std::exit(1);
}

natsOptions *
init_options()
{
    natsOptions* opts = NULL;
    natsOptions_Create(&opts);
    natsOptions_SetURL(opts, url);
    natsOptions_SetClosedCB(opts, onClosed, nullptr);
    return opts;
}

void
publish_message(natsConnection* nc, const std::string& message)
{
    std::cout << "Run publish message" << std::endl;
    natsStatus status = natsConnection_PublishString(nc, subject, message.c_str());
    check_status(status, "Publish message");
}

int
main()
{
    natsStatus status;
    natsConnection* nc = NULL;
    natsOptions* opts = init_options();

    status = natsConnection_Connect(&nc, opts);
    check_status(status, "Connection to server");

    publish_message(nc, "Hello from C++!");

    for (int i = 0; i < 5; ++i)
    {
        std::string msg = "Message #" + std::to_string(i) + " from C++ producer";
        publish_message(nc, msg);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    publish_message(nc, "Goodbye from C++!");


    status = natsConnection_DrainTimeout(nc, 5000);
    if (status !=  NATS_OK)
    {
        std::cerr << "Drain error\n";
    }

    while (!drained)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    natsConnection_Destroy(nc);
    natsOptions_Destroy(opts);
    nats_Close();
    std::cout << "Exit..." << std::endl;

    return 0;
}