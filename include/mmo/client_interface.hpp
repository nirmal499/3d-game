#pragma once

#include <stdexcept>
#include <util/mmo_common.hpp>
#include <mmo/message.hpp>
#include <mmo/thread_safe_queue.hpp>
#include <mmo/connection.hpp>

template<typename T>
struct ClientInterface
{
    boost::asio::io_service _ios;
    std::thread _thread;
    boost::asio::ip::tcp::socket _socket;
    std::unique_ptr<Connection<T>> _conn;
    ThreadSafeQueue<OwnedMessage<T>> _qmessagesIn;
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::results_type _endpoint;

    ClientInterface();
    virtual ~ClientInterface();

    bool Connect(const std::string& host, const uint16_t& port);
    void Reconnect();
    void Disconnect();
    bool IsConnected();
    ThreadSafeQueue<OwnedMessage<T>>& IncomingMsgQ();
    void Send(const Message<T>& msg);
};

template<typename T>
inline ClientInterface<T>::ClientInterface()
    : _socket(_ios)
{
    
}

template<typename T>
inline ClientInterface<T>::~ClientInterface()
{
    this->Disconnect();
}

template<typename T>
inline bool ClientInterface<T>::Connect(const std::string& host, const uint16_t& port)
{
    try{

        boost::asio::ip::tcp::resolver resolver(_ios);
        _endpoint = resolver.resolve(host,std::to_string(port));

        /* Here the _ios and _qmessagesIn are passed as reference */
        _conn = std::make_unique<Connection<T>>(Connection<T>::OWNERTYPE::CLIENT,
                                                    _ios,
                                                    boost::asio::ip::tcp::socket(_ios),
                                                    _qmessagesIn,
                                                    nullptr,
                                                    this
                                                );
        /* Tell the connection object to connect to the server synchronously */ 
        // if(!_conn->ConnectToServerSynchronous(_endpoint))
        // {
        //     throw std::runtime_error("Server is not available");
        // }

        /* Tell the connection object to connect to the server asynchronously */
        _conn->ConnectToServer(_endpoint);

        /* Start the thread that will run io_service */
        _thread = std::thread([this]()
        {
            _ios.run();
        });
        
    }catch(std::exception& e)
    {
        std::cerr << "Client Exception : " << e.what() << "\n";
        return false;
    }

    return true;
}

template<typename T>
inline void ClientInterface<T>::Reconnect()
{
    if(_conn)
    {
        /* Tell the connection object to connect to the server */
        _conn->ConnectToServer(_endpoint);
    }
}

template<typename T>
inline void ClientInterface<T>::Disconnect()
{
    /* If connection exists, and it's connected then... */
    if(this->IsConnected())
    {

        /* ....disconnect from server gracefully */
        _conn->Disconnect();
    }

    /* We are done with asio io_service */
    _ios.stop();

    if(_thread.joinable())
    {
        _thread.join();
    }

    (void)_conn.release();
}

template<typename T>
inline bool ClientInterface<T>::IsConnected()
{
    if(_conn)
    {
        return _conn->IsConnected();
    }

    return false;
}

template<typename T>
inline ThreadSafeQueue<OwnedMessage<T>>& ClientInterface<T>::IncomingMsgQ()
{
    return _qmessagesIn;
}

template<typename T>
inline void ClientInterface<T>::Send(const Message<T>& msg)
{
    if(this->IsConnected())
    {
        _conn->Send(msg);
    }
}