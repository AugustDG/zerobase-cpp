#pragma once

#include <vector>
#include <string>

/// @brief Configuration for a ZeroBase publisher socket
struct ZbPubSocketConfig
{
    std::string addr;

    ZbPubSocketConfig() : addr(""){};
    ZbPubSocketConfig(const std::string &_addr) : addr(_addr) {}
};

/// @brief Configuration for a ZeroBase subscriber socket
struct ZbSubSocketConfig
{
    std::string addr;
    std::vector<std::string> topics;

    ZbSubSocketConfig() : addr(""), topics(){};
    ZbSubSocketConfig(const std::string &_addr, const std::vector<std::string> &_topics) : addr(_addr), topics(_topics) {}
};