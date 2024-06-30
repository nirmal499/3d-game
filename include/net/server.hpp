#pragma once

#include <util/mmo_common.hpp>
#include <mmo/server_interface.hpp>

struct Server : public ServerInterface<CustomMsgTypes>
{
    Server(uint16_t PORT);
    virtual ~Server();

    bool OnClientConnect(const std::shared_ptr<Connection<CustomMsgTypes>>& client) override;
    void OnClientDisconnect(const std::vector<uint32_t>& disconnectClientsID) override;
    void OnMessageArrival(OwnedMessage<CustomMsgTypes>& msg) override;
    void OnClientValidated(std::shared_ptr<Connection<CustomMsgTypes>> client) override;
};