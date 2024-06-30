#pragma once

#include <util/mmo_common.hpp>

template<typename T>
struct MessageHeader
{
    T _id{};
    /* The size attribute will indicate the size of the message body */
    uint32_t _size = 0;
};

template<typename T>
struct Message
{
    MessageHeader<T> _header{};
    std::vector<uint8_t> _body;

    /* returns size of entire message packet in bytes */
    std::size_t size() const;

    /* https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Making_New_Friends */
    friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg)
    {
        os << "ID: " << int(msg.header.id) << " Size: " << msg.size();
        return os;
    }
    
    template<typename DataType>
    Message<T>& operator<<(const DataType& data);
    
    template<typename DataType>
    Message<T>& operator>>(DataType& data);
};

template<typename T>
struct Connection;

template<typename T>
struct OwnedMessage
{
    std::shared_ptr<Connection<T>> _remote = nullptr;
    Message<T> _msg;

    /* https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Making_New_Friends */
    friend std::ostream& operator<<(std::ostream& os,const OwnedMessage<T>& msg)
    {
        os << msg.msg;
        return os;
    }
};

template<typename T>
template<typename DataType>
inline Message<T>& Message<T>::operator<<(const DataType& data)
{
    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

    std::size_t i = this->_body.size();

    this->_body.resize(this->_body.size()+sizeof(DataType));

    /*  memcpy(dest, src, count) */
    std::memcpy(this->_body.data() + i,&data,sizeof(DataType));

    this->_header._size = this->_body.size();

    return *this;

}

template<typename T>
template<typename DataType>
inline Message<T>& Message<T>::operator>>(DataType& data)
{
    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be poped");

    std::size_t i = this->_body.size() - sizeof(DataType);

    /*  memcpy(dest, src, count) */
    std::memcpy(&data, this->_body.data() + i ,sizeof(DataType));

    this->_body.resize(i);

    this->_header._size = this->_body.size();

    return *this;

}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
{
    os << msg.msg;
    return os;
}