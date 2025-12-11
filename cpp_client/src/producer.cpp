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
main()
{
    const char* url = "nats://@localhost:4222";

    natsStatus status;
    jsErrCode jerr;
    natsOptions* opts = nullptr;
    natsConnection* nc = nullptr;
    jsCtx* js = nullptr;

    // 1. базовые опции
    natsOptions_Create(&opts);
    natsOptions_SetURL(opts, url);
    natsOptions_SetUserInfo(opts, "service_a", "password_a");

    // 2. соединение и JetStream
    status = natsConnection_Connect(&nc, opts);
    check_status(status, "Connect");
    status = natsConnection_JetStream(&js, nc, nullptr);
    check_status(status, "JetStream");

    // 3. конфиг stream
    jsStreamConfig cfg;
    jsStreamConfig_Init(&cfg);
    cfg.Name        = "EVENTS";
    const char* subs[] = {"event.*", "test.*"};
    cfg.Subjects    = subs;
    cfg.SubjectsLen = 2;
    cfg.Storage     = js_FileStorage;
    cfg.MaxAge      = 24*3600*1'000'000'000LL;

    // 4. создаём stream
    jsStreamInfo* info = nullptr;
    status = js_AddStream(&info, js, &cfg, nullptr, &jerr);
    if (status == NATS_OK)
        std::cout << "stream ok\n";

    if (status != NATS_OK && jerr == 10058)
    {
        printf("стрим уже с другой конфигурацией – пропускаем\n");
        status = NATS_OK;   // считаем не ошибкой
    }
    if (status == NATS_OK && info) {
    std::cout << "stream subjects:\n";
    for (int i = 0; i < info->Config->SubjectsLen; ++i)
        std::cout << "  - " << info->Config->Subjects[i] << '\n';
    }

    jsStreamInfo_Destroy(info);

    natsMsg *m = nullptr;
    natsMsg_Create(&m , "event.subject", nullptr,
                             "payload", 7);

    status = js_PublishMsgAsync(js, &m, NULL);

    if (status == NATS_OK)
        std::cout << "message ok\n";

    // 5. публикуем
    // status = natsConnection_PublishString(nc, "test.test", "test_payload");
    // std::cout << "Publish status = " << natsStatus_GetText(status) << std::endl;
    //js_PublishAsyncComplete(js, 5000);
    // 6. чистим
    jsPubOptions opt = {0};
    opt.MaxWait = 3000;          // максимум 3 сек
    js_PublishAsyncComplete(js, &opt);

    jsCtx_Destroy(js);
    natsConnection_Destroy(nc);
    natsOptions_Destroy(opts);
    nats_Close();
    return 0;
}