#include "sockets.h"

/**
 * Constructor for ZbPubSocket class.
 * @param _ctx zmq context object pointer.
 * @param _config configuration object for the publisher socket.
 */
ZbPubSocket::ZbPubSocket(zmq::context_t *_ctx, const ZbPubSocketConfig &_config)
{
    config = _config;

    socket = zmq::socket_t(*_ctx, zmq::socket_type::pub);
    socket.bind(config.addr);
}

/// @brief Destructor for ZbPubSocket class.
ZbPubSocket::~ZbPubSocket()
{
    socket.close();
}

/**
 * Constructor for ZbSubSocket class.
 * @param _ctx zmq context object pointer.
 * @param _config configuration object for the subscriber socket.
 */
ZbSubSocket::ZbSubSocket(zmq::context_t *_ctx, const ZbSubSocketConfig &_config)
{
    config = _config;

    socket = zmq::socket_t(*_ctx, zmq::socket_type::sub);
    socket.connect(config.addr);

    for (const auto &topic : config.topics)
    {
        socket.set(zmq::sockopt::subscribe, topic);
    }
}

/// @brief Destructor for ZbSubSocket class.
ZbSubSocket::~ZbSubSocket()
{
    socket.close();
}