#pragma once

#include <util/mmo_common.hpp>
#include <mmo/thread_safe_queue.hpp>
#include <mmo/message.hpp>
#include <mmo/connection.hpp>

template<typename T>
struct ServerInterface
{
    ThreadSafeQueue<OwnedMessage<T>> _qmessagesIn;

    boost::asio::io_service _ios;
    std::thread _thread;

    std::deque<std::shared_ptr<Connection<T>>> _dequeConnections;
    boost::asio::ip::tcp::acceptor _acceptor;

    uint32_t _IDCounter = 10000;

    ServerInterface(uint16_t port);
    virtual ~ServerInterface();

    void Update(bool toWait, std::size_t nMaxMessages = -1);
    void MessageAllClients(const Message<T>& msg, const std::shared_ptr<Connection<T>>& IgnoreClient = nullptr);
    void MessageAClient(std::shared_ptr<Connection<T>>& client, const Message<T>& msg);
    void WaitForClientConnection();
    void Stop();
    bool Start();

    virtual bool OnClientConnect(const std::shared_ptr<Connection<T>>& client) = 0;
    virtual void OnClientDisconnect(const std::vector<uint32_t>& disconnectClientsID) = 0;
    virtual void OnMessageArrival(OwnedMessage<T>& msg) = 0;
    virtual void OnClientValidated(std::shared_ptr<Connection<T>> client) = 0;

};

template<typename T>
inline void ServerInterface<T>::MessageAClient(std::shared_ptr<Connection<T>>& client, const Message<T>& msg)
{
    bool isDisconnected = false;
    uint32_t IDofClientDisconnected = 0;

    if(client && client->IsConnected())
    {
        client->Send(msg);
    }else
    {
        /* 
            On of the limitations using the TCP protocol is we don't necessarily know when a client
            has disconnected, so it is only when we try to manipulate the client and that manipulation
            fails then we know that client is disconnected
        */
        isDisconnected = true;
        IDofClientDisconnected = client->GetID();
        client.reset(); /* Release the memory allocated */
        /* reset() function requires the object to non const */
        _dequeConnections.erase(std::remove(_dequeConnections.begin(), _dequeConnections.end(), client), _dequeConnections.end());
    }

    if(isDisconnected)
    {
        this->OnClientDisconnect(std::vector<uint32_t>{IDofClientDisconnected});
    }

    /*
        We need to make sure that "OnClientDisconnect" is called only after _dequeConnections.erase() function becoz it can cause a loop
    */
}

template<typename T>
inline void ServerInterface<T>::MessageAllClients(const Message<T>& msg, const std::shared_ptr<Connection<T>>& IgnoreClient)
{
    bool flagForInvalidClientExists = false;
    std::vector<uint32_t> IDsofClientDisconnected;

    for(auto& client: _dequeConnections)
    {
        if(client && client->IsConnected())
        {
            if(client != IgnoreClient)
            {
                client->Send(msg);
            }
        }else
        {   
            IDsofClientDisconnected.push_back(client->GetID());
            /* Client couldn't be contacted, so assume is has disconnected*/
            client.reset(); /* Here the client is being set to nullptr */
            flagForInvalidClientExists = true;
        }
    }

    if(flagForInvalidClientExists)
    {
        _dequeConnections.erase(std::remove(_dequeConnections.begin(), _dequeConnections.end(), nullptr), _dequeConnections.end());
        this->OnClientDisconnect(IDsofClientDisconnected);
    }

    /*
        We need to make sure that "OnClientDisconnect" is called only after _dequeConnections.erase() function becoz it can cause a loop
    */
}

template<typename T>
inline void ServerInterface<T>::Update(bool toWait, std::size_t nMaxMessages)
{
    /* 
        No need to wait becoz we are apply condition variable in tsqueue for waiting during pop_front()
        Setting nMaxMessages to maximum number becoz it a unsigned variable
    */
    if(toWait)
    {
        _qmessagesIn.Wait();
    }

    /* Process as many requests as you can up to the value specified */
    std::size_t nMessagesCount = 0;
    while(nMessagesCount < nMaxMessages && !_qmessagesIn.Empty())
    {
        /* Grab the front message */
        auto msg = _qmessagesIn.PopFrontWithoutSleep();

        /* Pass to message handler for processing of the message */
        this->OnMessageArrival(msg);

        nMessagesCount++;
    }

}

template<typename T>
inline void ServerInterface<T>::WaitForClientConnection()
{
    /* Whenever the async_accept is fired the passed lambda function is called */
    _acceptor.async_accept([this](boost::system::error_code ec,boost::asio::ip::tcp::socket socket)
    {
        if(!ec){
            std::cout << "[SERVER] new connection : " << socket.remote_endpoint() << "\n";

            std::shared_ptr<Connection<T>> newconn =
                std::make_shared<Connection<T>>(Connection<T>::OWNERTYPE::SERVER,
                                                _ios,
                                                std::move(socket),
                                                _qmessagesIn, this, nullptr);
            /* _ios and _qmessagesIn are passed as reference */

            /* Give the server a chance to deny connection */
            if(this->OnClientConnect(newconn))
            {
                /* 
                    We add the connection to the container eventhough it is not validated.
                    Incase it was found that the connection is not validated then, its _socket will be closed
                    And it will be removed later in functions like "MessageAllClients()"
                
                */
                _dequeConnections.emplace_back(std::move(newconn));
                
                /* Now Connection will be checked for validation in ConnectToClient */
                _dequeConnections.back()->ConnectToClient(_IDCounter++);

                // std::cout << "[" << _dequeConnections.back()->GetID() << "] Connection Approved By Server\n";

                /* Here the reference_count of shared_ptr of newconn becomes to 1 */
            }else
            {
                std::cout << "[-------------] Connection Denied By Server\n";
            }
            /*
                If the connection is rejected by the server, then the pointer will go out of scope
                and the memory will be deallocated as it a smart pointer
            */
        }else
        {
            std::cout << "[SERVER] new connection error " << ec.message() << "\n";
        }

        /* Simply wait for another connection */
        this->WaitForClientConnection();
    });
}

template<typename T>
inline ServerInterface<T>::ServerInterface(uint16_t port)
    : _acceptor(_ios,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
{
    
}

template<typename T>
inline ServerInterface<T>::~ServerInterface()
{
    this->Stop();
}

template<typename T>
inline void ServerInterface<T>::Stop()
{
    /* Stop the io_service */
    _ios.stop();

    if(_thread.joinable())
    {

        /* Join the Thread that was tasked to run io_service */
        _thread.join();
    }

    std::cout << "[SERVER] Stopped\n";
}

template<typename T>
inline bool ServerInterface<T>::Start()
{
    try
    {
        this->WaitForClientConnection();

        _thread = std::thread([this]()
        {
            _ios.run();
        });

    }catch(std::exception& e)
    {
        std::cerr << "[SERVER] Exception: " << e.what() << "\n";
        return false;
    }

    std::cout << "[SERVER] started\n";
    return true;
}
