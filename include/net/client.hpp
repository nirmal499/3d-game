#pragma once

#include <util/mmo_common.hpp>
#include <mmo/client_interface.hpp>

struct Client : public ClientInterface<CustomMsgTypes>
{
    Client();
    virtual ~Client();
    void PingServer();
    void MessageAll(const std::array<float, 6>& playerPositionInfo);
};
