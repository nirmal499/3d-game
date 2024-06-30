#pragma once

#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

enum class CustomMsgTypes : uint32_t
{
    /*
        Server send this to recently connected client informing them that they have been accepted
    */
    ServerAccept,

    /*
        Server send this to recently connected client informing them that they have been declined
        CustomMsgTypes::ServerDeny
    */
    // ServerDeny,

    /*
        ClientI send this to Server and Server reads it and as per the request it sends the
        info to all the other clients as a CustomMsgTypes::ServerMessage message and ignoring the ClientI.

        The ClientI sends it Position information and Server sends that Position information
        to all other connected clients
    */
    MessageAll,
    /*
        It goes both ways Client sends it and Server reads and respond back to that
        client only
    */
    ServerPing,
    /*
        Whenever server accepts a new client, it send the info{ID of the client}
        to all other connected clients informing them about the new accepted client.
        Ignoring the recently connected client informing them about the new accepted client.
    */
    // NewConnectionAdded,
    /*
        Server uses it to send data to a clients.
    */
    ServerMessage,
    /*
        Server used it to inform clients about the clients who are disconnected
    */
    ClientsDisconnected,
};