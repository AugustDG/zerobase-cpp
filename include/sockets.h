#pragma once

#include "zmq.hpp"

#include "configs.h"

/// @brief ZeroBase publisher socket
struct ZbPubSocket
{
    zmq::socket_t socket;
    ZbPubSocketConfig config; // This config contains information about the socket, it's solely a reflection of the config used to create the socket

    ZbPubSocket() = delete;
    ZbPubSocket(ZbPubSocket &_socket) = delete;
    ZbPubSocket(const ZbPubSocket &_socket) = delete;

    ZbPubSocket(zmq::context_t *_ctx, const ZbPubSocketConfig &_config);
    ~ZbPubSocket();
};

/// @brief ZeroBase subscriber socket
struct ZbSubSocket
{
    zmq::socket_t socket;
    ZbSubSocketConfig config; // This config contains information about the socket, it's solely a reflection of the config used to create the socket

    ZbSubSocket() = delete;
    ZbSubSocket(const ZbSubSocket &_socket) = delete;

    ZbSubSocket(zmq::context_t *_ctx, const ZbSubSocketConfig &_config);
    ~ZbSubSocket();
};