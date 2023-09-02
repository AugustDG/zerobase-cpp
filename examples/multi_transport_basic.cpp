#include <iostream>
#include <thread>
#include <chrono>

#include "zmq.hpp"

#include "zerobase.h"

ZeroBase *zb;

bool BaseMain()
{
    std::cout << "Main loop running..." << std::endl;

    auto msg = 2;

    zb->Send("A", "Hello", 6);
    zb->Send("B", &msg, sizeof(msg));

    msg++;
    zb->Send("C", &msg, sizeof(msg));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
}

void Terminated()
{
    std::cout << "Program is terminating..." << std::endl;
}

void MsgReceived(const std::string &topic, const void *data)
{
    if (topic == "A")
        std::cout << "Received: " << topic << " " << (const char *)data << std::endl;
    else
        std::cout << "Received: " << topic << " " << *(int *)data << std::endl;
}

int main()
{
    Result result;

    ZbPubSocketConfig pub_config_1 = ZbPubSocketConfig("tcp://*:5555");
    ZbPubSocketConfig pub_config_2 = ZbPubSocketConfig("tcp://127.0.0.1:5556");

    ZbSubSocketConfig sub_config_1 = ZbSubSocketConfig("tcp://127.0.0.1:5555", {"A"});
    ZbSubSocketConfig sub_config_2 = ZbSubSocketConfig("tcp://localhost:5555", {"B"});
    ZbSubSocketConfig sub_config_3 = ZbSubSocketConfig("tcp://127.0.0.1:5556", {"B"});

    zb = new ZeroBase(BaseMain, Terminated, MsgReceived);

    // the library follows the result patten, so all public methods return a Result object
    // that can be checked for success or failure (along with a message)
    result = zb->Init({pub_config_1, pub_config_2}, {sub_config_1, sub_config_2, sub_config_3});
    if (!result)
    {
        std::cerr << result.msg << std::endl;
        return -1;
    }

    result = zb->Run();
    if (!result)
    {
        std::cerr << result.msg << std::endl;
        return -1;
    }

    delete zb;

    return 0;
}