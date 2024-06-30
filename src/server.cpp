#include <cstdint>
#include <net/server.hpp>

Server::Server(uint16_t PORT): ServerInterface<CustomMsgTypes>(PORT){}
Server::~Server()
{

}

bool Server::OnClientConnect(const std::shared_ptr<Connection<CustomMsgTypes>>& client)
{
    return true;
}

void Server::OnClientDisconnect(const std::vector<uint32_t>& disconnectClientsID)
{
    Message<CustomMsgTypes> msg;
    msg._header._id = CustomMsgTypes::ClientsDisconnected;
    uint32_t noOfDisconnectClientsID = disconnectClientsID.size();

    for(const auto& clientID: disconnectClientsID)
    {
        msg << clientID;
    }

    msg << noOfDisconnectClientsID;
    
    this->MessageAllClients(msg, nullptr);
}

void Server::OnMessageArrival(OwnedMessage<CustomMsgTypes>& msg)
{
    Message<CustomMsgTypes>& message = msg._msg;
    const std::shared_ptr<Connection<CustomMsgTypes>>& remote = msg._remote;

    switch(msg._msg._header._id)
    {
    case CustomMsgTypes::ServerPing:
        {
            std::cout << "[" << remote->GetID() << "] ServerPing\n";

            /* Simply bounce message back to client */
            remote->Send(message);
        }
        break;
    case CustomMsgTypes::MessageAll:
        {
            std::cout << "[" << remote->GetID() << "]: Message All\n";
            
            message._header._id = CustomMsgTypes::ServerMessage;
            message << remote->GetID();
            this->MessageAllClients(message, remote);
        }
        break;
    default:
        break;
    }
}

void Server::OnClientValidated(std::shared_ptr<Connection<CustomMsgTypes>> client)
{
    Message<CustomMsgTypes> msgToClient;
    msgToClient._header._id = CustomMsgTypes::ServerAccept;

    // std::vector<uint32_t> noOfClientConnectedRightNowList(_dequeConnections.size());
    // for(const auto& clientItem: _dequeConnections)
    // {
    //     noOfClientConnectedRightNowList.push_back(clientItem->GetID());
    // }

    // uint32_t noOfClientsConnectedRightNow = _dequeConnections.size();
    // for(const auto& clientItem: _dequeConnections)
    // {
    //     msgToClient << clientItem->GetID();
    // }

    // msgToClient << noOfClientsConnectedRightNow;


    client->Send(msgToClient); /* OR "this->MessageAClient(client, msgToClient);" */

    std::cout << "[" << client->GetID() << "]: Player Connected\n";
}