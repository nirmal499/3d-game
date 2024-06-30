#include <net/client.hpp>

Client::Client()
{
    
}

Client::~Client()
{
    
}

void Client::PingServer()
{
    Message<CustomMsgTypes> msg;
    msg._header._id = CustomMsgTypes::ServerPing;

    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

    msg << timeNow; /* Here we are adding data into msg body */

    this->Send(msg);
}

void Client::MessageAll(const std::array<float, 6>& playerPositionInfo)
{
    Message<CustomMsgTypes> msg;
    msg._header._id = CustomMsgTypes::MessageAll;
    msg << playerPositionInfo;
    this->Send(msg);
}