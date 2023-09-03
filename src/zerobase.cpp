#include <iostream>

#include "zmq.hpp"
#include "zmq_addon.hpp"

#include "macros.h"
#include "zerobase.h"

/**
 * @brief Constructor for ZeroBase class.
 *
 * @param _main A std::function<bool()> representing the main function of the program.
 * @param _msgReceived A std::function<void(const std::string &, const void *)> representing the function to be called when a message is received.
 */
ZeroBase::ZeroBase(const std::function<bool()> &_main, const std::function<void(const std::string &, const void *)> &_msgReceived)
{
    main = _main;
    msg_received = _msgReceived;

    has_init = false;
}

/**
 * @brief Destructor for the ZeroBase class.
 *
 * Calls the Uninit() function if the has_init flag is set to true.
 */
ZeroBase::~ZeroBase()
{
    if (has_init)
        Uninit();
}

/**
 * Initializes ZeroBase with the given publisher and subscriber socket configurations.
 *
 * @param _pubConfigs A vector of ZbPubSocketConfig objects representing the publisher socket configurations.
 * @param _subConfigs A vector of ZbSubSocketConfig objects representing the subscriber socket configurations.
 * @return A Result object indicating whether the initialization was successful or not.
 */
Result ZeroBase::Init(const std::vector<ZbPubSocketConfig> &_pubConfigs, const std::vector<ZbSubSocketConfig> &_subConfigs)
{
    if (has_init)
        return Result("Comms already running!");

    INFO("Initializing ZeroBase...");

    // Initialize ZeroMQ context & ZeroBase sockets
    context = zmq::context_t(1);

    for (const auto &config : _pubConfigs)
        pub_sockets.emplace_back(new ZbPubSocket(&context, config));

    for (const auto &config : _subConfigs)
        sub_sockets.emplace_back(new ZbSubSocket(&context, config));

    // Start the receive loop thread
    receive_loop_future = std::async(std::launch::async, ZeroBase::ReceiveLoop, this);

    has_init = true;

    return Result(true);
}

/**
 * @brief This function is the main entry point for the ZeroBase class (and it should be for the program as well)! It will assume that this is the main thread and run the flow in the appropriate order.
 *
 * @return A Result object indicating whether the function was successful or not.
 */
Result ZeroBase::Run()
{
    if (!has_init)
        return Result("ZeroBase not initialized!");

    while (has_init && main())
        ;

    Result result = Uninit();
    IF_INVALID_RESULT_RETURN(result);

    return Result(true);
}

/**
 * @brief Stops and cleans up this instance. Must be called before the program exits!
 *
 * @return A Result object indicating whether the function was successful or not.
 */
Result ZeroBase::Uninit()
{
    if (!has_init)
        return Result("ZeroBase not initialized!");

    INFO("Stopping ZeroBase...");

    Result result = receive_loop_future.get();
    IF_INVALID_RESULT_RETURN(result);

    for (const auto pub_socket : pub_sockets)
        delete pub_socket;

    for (const auto sub_socket : sub_sockets)
        delete sub_socket;

    pub_sockets.clear();
    sub_sockets.clear();

    context.shutdown();
    context.close();

    has_init = false;

    return Result(true);
}

/**
 * @brief Sends a message with the given topic and data to all publisher sockets.
 *
 * @param _topic A string representing the topic of the message.
 * @param _data A void pointer to the data to be sent.
 * @param _size A size_t representing the size of the data to be sent.
 * @return A Result object indicating whether the function was successful or not.
 */
Result ZeroBase::Send(const std::string &_topic, const void *_data, size_t _size)
{
    if (!has_init)
        return Result("ZeroBase not initialized!");

    std::array<zmq::const_buffer, 2> msgs = {
        zmq::buffer(std::string_view(_topic)),
        zmq::buffer(_data, _size)};

    for (const auto socket : pub_sockets)
    {
        if (!zmq::send_multipart(socket->socket, msgs))
            return Result("Failed to send message! " + std::string(zmq_strerror(errno)));
    }

    return Result(true);
}

/**
 * @brief The receive loop function for ZeroBase.
 *
 * @param _zb A pointer to the ZeroBase instance.
 * @return A Result object indicating whether the function was successful or not.
 */
Result ZeroBase::ReceiveLoop(const ZeroBase *_zb)
{
    Result result;

    zmq::poller_t<> poller = zmq::poller_t();

    INFO("Registering sockets...");

    for (const auto &socket : _zb->sub_sockets)
    {
        INFO("Registering sub socket on address " + socket->config.addr);

        poller.add(socket->socket, zmq::event_flags::pollin);
    }

    if (poller.size() == 0)
        return Result("No sockets to register!");

    INFO("Registered " + std::to_string(poller.size()) + " sockets!");

    const auto pollTimeout = std::chrono::milliseconds(100);
    auto events = std::vector<zmq::poller_event<>>(poller.size());

    INFO("Starting receive loop...");

    while (_zb->has_init)
    {
        try
        {
            const auto numEvents = poller.wait_all(events, pollTimeout);

            if (!numEvents)
                continue;

            ProcessPoll(events, _zb);
        }
        catch (const zmq::error_t &e)
        {
            ERROR("ZMQ error: " + std::string(e.what()));
            continue;
        }
    }

    return Result(true);
}

/**
 * @brief Processes the poll events for ZeroBase.
 *
 * @param _events A vector of zmq::poller_event<> objects representing the events to be processed.
 * @param _zb A pointer to the ZeroBase instance.
 * @return A Result object indicating whether the function was successful or not.
 */
Result ZeroBase::ProcessPoll(std::vector<zmq::poller_event<>> &_events, const ZeroBase *_zb)
{
    for (auto &event : _events)
    {
        // If the socket is not readable as a poll in, skip it
        if ((event.events & zmq::event_flags::pollin) == zmq::event_flags::none)
            continue;

        std::vector<zmq::message_t> msgs;

        zmq::recv_result_t recvResult = zmq::recv_multipart(event.socket, std::back_inserter(msgs));
        if (!recvResult.has_value())
        {
            return Result("Failed to read message!");
        }

        if (msgs.size() < 2 || msgs[1].size() == 0)
        {
            return Result("Received empty message!");
        }

        _zb->msg_received(msgs[0].to_string(), msgs[1].data());
    }

    return Result(true);
}