#pragma once

#include <util/mmo_common.hpp>
#include <mmo/thread_safe_queue.hpp>
#include <mmo/message.hpp>
#include <cstdint>
#include <mmo/server_interface.hpp>

template<typename T>
struct ServerInterface;

template<typename T>
struct ClientInterface;

template<typename T>
struct Connection : public std::enable_shared_from_this<Connection<T>>
{
    enum class OWNERTYPE
    {
        SERVER,
        CLIENT
    };

    boost::asio::ip::tcp::socket _socket;
    boost::asio::io_service& _ios;
    ThreadSafeQueue<Message<T>> _qmessagesOut;
    ThreadSafeQueue<OwnedMessage<T>>& _qmessagesIn;
    
    OWNERTYPE _ownerType;
    ServerInterface<T>* _serverPointer = nullptr;
    ClientInterface<T>* _clientPointer = nullptr;

    uint32_t _ID = 0;
    Message<T> _tempMessage;

    uint64_t _handShakeOut = 0;
    uint64_t _handShakeIn = 0;
    uint64_t _handShakeCheck = 0;

    Connection(
        OWNERTYPE ownerType,
        boost::asio::io_service& ios,
        boost::asio::ip::tcp::socket socket,
        ThreadSafeQueue<OwnedMessage<T>>& qIncoming,
        ServerInterface<T>* serverPointer,
        ClientInterface<T>* clientPointer
    );
    virtual ~Connection();

    uint32_t GetID() const;
    void ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void Disconnect();
    bool IsConnected() const;

    void ConnectToClient(int IDofClient);
    void Send(const Message<T>& msg);

    void ReadHeader();
    void ReadBody();
    void WriteHeader();
    void WriteBody();

    void AddToIncomingMsgQ();

    uint64_t Scramble(uint64_t nInput);

    void WriteValidation();
    void ReadValidation();

};

// User-defined literal operator for uint64_t
inline uint64_t operator""_u64(const char* str, size_t)
{
    return std::stoull(str, nullptr, 16);
}

template<typename T>
inline uint32_t Connection<T>::GetID() const
{
    return _ID;
}

template<typename T>
inline Connection<T>::Connection(
                          OWNERTYPE ownerType,
                          boost::asio::io_service& ios,
                          boost::asio::ip::tcp::socket socket,
                          ThreadSafeQueue<OwnedMessage<T>>& qIncoming,
                          ServerInterface<T>* serverPointer,
                          ClientInterface<T>* clientPointer
    )
    : 
      _socket(std::move(socket)),
      _ios(ios),
      _qmessagesIn(qIncoming),
      _ownerType(ownerType),
      _serverPointer(serverPointer),
      _clientPointer(clientPointer)
{
    if(ownerType == OWNERTYPE::SERVER)
    {
        _handShakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
        _handShakeCheck = this->Scramble(_handShakeOut);
    }
    else
    {
        _handShakeIn = 0;
        _handShakeCheck = 0;
    }
}

template<typename T>
inline Connection<T>::~Connection()
{
    
}

template<typename T>
inline void Connection<T>::ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints)
{
    if(_ownerType == OWNERTYPE::CLIENT)
    {
        boost::asio::async_connect(_socket, endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint)
        {
            if(!ec)
            {
                this->ReadValidation();
            }
        });
    }
}

template<typename T>
inline void Connection<T>::Disconnect()
{
     if(this->IsConnected())
    {
        boost::asio::post(_ios,[this]()
        {
            _socket.close();
        });
    }
}

template<typename T>
inline bool Connection<T>::IsConnected() const
{
    return _socket.is_open();
}

template<typename T>
inline void Connection<T>::ConnectToClient(int IDofClient)
{
    if(_ownerType == OWNERTYPE::SERVER)
    {
        if(_socket.is_open())
        {
            _ID = IDofClient;

            this->WriteValidation();
        }
    }
}

template<typename T>
inline void Connection<T>::Send(const Message<T>& msg)
{
    boost::asio::post(_ios,[this,msg]()
    {
        bool empty_flag = _qmessagesOut.Empty();
        _qmessagesOut.PushBack(msg);

        if(empty_flag)
        {
            this->WriteHeader();
        }
    });
}

template<typename T>
inline void Connection<T>::ReadHeader()
{
    boost::asio::async_read(_socket,boost::asio::buffer(&_tempMessage._header,sizeof(MessageHeader<T>)),
                                 [this](boost::system::error_code ec,std::size_t length)
                                 {
                                    if(!ec)
                                    {
                                        if(_tempMessage._header._size > 0)
                                        {
                                            _tempMessage._body.resize(_tempMessage._header._size);
                                            this->ReadBody();
                                        }
                                        else
                                        {
                                            /*
                                                Here m_temp.header.size is 0 BYTES
                                                m_tem   p.header.size denotes the size of the msg body
                                                We can have msg which have NO body e.g CustomMsgTypes::ServerAccept
                                            */
                                            this->AddToIncomingMsgQ();
                                        }
                                    }
                                    else
                                    {
                                        std::cout << "[" << _ID << "] Read Header Fail\n";
                                        _socket.close();
                                    }
                                 });
}

template<typename T>
inline void Connection<T>::ReadBody()
{
    boost::asio::async_read(_socket,boost::asio::buffer(_tempMessage._body.data(),_tempMessage._body.size()),
                        [this](boost::system::error_code ec,std::size_t length)
                        {
                            if(!ec)
                            {
                                this->AddToIncomingMsgQ();
                            }else
                            {
                                std::cout << "[" << _ID << "] Read Body Fail\n";
                                _socket.close();
                            }
                        });
}

template<typename T>
inline void Connection<T>::WriteHeader()
{
    if(!_qmessagesOut.Empty())
                {
                    boost::asio::async_write(_socket,boost::asio::buffer(
                                                            &_qmessagesOut.Front()._header,
                                                            sizeof(MessageHeader<T>)),
                    [this](boost::system::error_code ec,std::size_t length)
                    {
                        if(!ec)
                        {
                            if(_qmessagesOut.Front()._header._size > 0){
                                /* We have a message body */
                                this->WriteBody();
                            }else
                            {
                                /*
                                    Here the size of msg body is 0
                                    We can have a msg which have NO body e.g CustomMsgTypes::ServerAccept
                                */
                                _qmessagesOut.PopFrontWithoutSleep();
                                this->WriteHeader();
                            }
                        }else
                        {
                            std::cout << "[" << _ID << "] Write Header Fail\n";
                            _socket.close();
                        }
                    });
                }
}

template<typename T>
inline void Connection<T>::WriteBody()
{
    if(!_qmessagesOut.Empty())
                {
                        boost::asio::async_write(_socket, boost::asio::buffer(
                                                            _qmessagesOut.Front()._body.data(),
                                                            _qmessagesOut.Front()._body.size()),
                                    [this](boost::system::error_code ec,std::size_t length)
                        {
                            if(!ec)
                            {
                                _qmessagesOut.PopFrontWithoutSleep();
                                this->WriteHeader();
                            }
                            else
                            {
                                std::cout << "[" << _ID << "] Write Body Fail\n";
                                _socket.close();
                            }
                    });
                }
}

template<typename T>
inline void Connection<T>::AddToIncomingMsgQ()
{
    /* specifying "OwnedMessage<T>" is IMP becoz we are using uvref{forward} in PushBack() */
    if(_ownerType == OWNERTYPE::SERVER)
    {
        /* We are constructing OwnedMessage while pushing */
        _qmessagesIn.PushBack(OwnedMessage<T>{ this->shared_from_this() , _tempMessage });
    }else
    {
        _qmessagesIn.PushBack(OwnedMessage<T>{ nullptr , _tempMessage });
    }

    this->ReadHeader();
}

template<typename T>
inline uint64_t Connection<T>::Scramble(uint64_t nInput)
{
    uint64_t out = nInput ^ "0xIAMDEADFROMINSIDE"_u64;
    out = (out & "0xYOUWILLDIEALONE"_u64) >> 4 | (out & "0xDEALWITHITYOUDESERVERIT"_u64) << 4;
    return out ^ "0xUSELESSPEICEOFCRAP"_u64;
}

template<typename T>
inline void Connection<T>::WriteValidation()
{
    boost::asio::async_write(_socket,boost::asio::buffer(&_handShakeOut,sizeof(uint64_t)),
                                [this](boost::system::error_code ec,std::size_t length)
                                {
                                    if(!ec)
                                    {
                                        /* Validation data sent, clients should sit and wait for a response */
                                        if(_ownerType == OWNERTYPE::CLIENT)
                                        {
                                            this->ReadHeader();
                                        }
                                        else
                                        {
                                            /* Incase of server after WritingValidation, it start ReadingValidation from client */
                                            this->ReadValidation();
                                        }
                                    }else
                                    {
                                        _socket.close();
                                    }
                                  });
}

template<typename T>
inline void Connection<T>::ReadValidation()
{
    boost::asio::async_read(_socket,boost::asio::buffer(&_handShakeIn,sizeof(uint64_t)),
                                [this](boost::system::error_code ec,std::size_t length)
                                {
                                    if(!ec)
                                    {
                                        if(_ownerType == OWNERTYPE::SERVER)
                                        {
                                            // std::cout << _handShakeIn << "\n";
                                            // std::cout << _handShakeCheck << "\n";
                                            if(_handShakeIn == _handShakeCheck)
                                            {
                                                std::cout << "Client Validated\n";

                                                _serverPointer->OnClientValidated(this->shared_from_this());
                                                // _serverPointer->_dequeConnections.emplace_back(this->shared_from_this());
                                                
                                                this->ReadHeader();
                                            }else
                                            {
                                                /* Client gave incorrect data,so disconnect */
                                                std::cout << "Client Disconnected (Fail Validation)\n";
                                                _socket.close();
                                            }
                                        }else
                                        {
                                            // std::cout << _handShakeIn << "\n";
                                            // std::cout << _handShakeCheck << "\n";

                                            /* Connection is a client, so solve puzzle */
                                            _handShakeOut = this->Scramble(_handShakeIn);

                                            /* Write the result */
                                            this->WriteValidation();
                                        }
                                    }else
                                    {
                                        // Some failure has occurred.
                                        std::cout << "Client Disconnected (ReadValidation)\n";
                                        _socket.close();
                                    }
                                });
}