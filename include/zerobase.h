#pragma once

#include <functional>
#include <future>
#include <any>

#include "zmq.hpp"

#include "configs.h"
#include "sockets.h"
#include "result.hpp"

class ZeroBase
{
private:
    zmq::context_t context;

    std::function<bool()> main;
    std::function<void(const std::string &, const void *)> msg_received;

    std::vector<ZbPubSocket *> pub_sockets;
    std::vector<ZbSubSocket *> sub_sockets;

    std::future<Result> receive_loop_future;

    bool has_init;

public:
    ZeroBase(const std::function<bool()> &_main, const std::function<void(const std::string &, const void *)> &_msgReceived);
    ~ZeroBase();

    Result Init(const std::vector<ZbPubSocketConfig> &_pubConfigs, const std::vector<ZbSubSocketConfig> &_subConfigs);
    Result Run();
    Result Uninit();

    Result Send(const std::string &_topic, const void *_data, size_t _size);

private:
    static Result ReceiveLoop(const ZeroBase *_zb);
    static Result ProcessPoll(std::vector<zmq::poller_event<>> &_events, const ZeroBase *_zb);
};
