#include <nats.h>
#include <iostream>

class NatsProduser
{
public:
    void connect();
    void initContext();
    void sendMessage(const std::string& msg);
};

void
check_status(natsStatus status, const char* msg)
{
    if (status != NATS_OK)
    {
        std::cerr << "Error [ " << msg << "]: "
                  << natsStatus_GetText(status) << '\n';

        // jsCtx_Destroy(js);
        // natsConnection_Destroy(nc);
        // natsOptions_Destroy(opts);
        nats_Close();
        std::exit(1);
    }
}

int
main() {
    const char* url = "nats://@localhost:4222";
    natsStatus status;
    natsOptions* opts = NULL;
    natsConnection* nc = NULL;
    jsCtx* js = NULL;

    natsOptions_Create(&opts);
    natsOptions_SetURL(opts, url);
    natsOptions_SetUserInfo(opts, "service_a", "password_a");

    status = natsConnection_Connect(&nc, opts);
    check_status(status, "Connect to server");
    std::cout << "Connect to " << url << " success." << std::endl;
    
    status = natsConnection_JetStream(&js, nc, NULL);
    check_status(status, "Init JetStream");
    std::cout << "JetStream init success." << std::endl;

    // publish
    status = natsConnection_PublishString(nc, "update.test", "test_payload");
    std::cout << "Status = " << natsStatus_GetText(status) << std::endl;
    // destroy
    jsCtx_Destroy(js);
    natsConnection_Destroy(nc);
    natsOptions_Destroy(opts);
    nats_Close();
    return 0; 
}