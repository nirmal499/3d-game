#pragma once

#include <util/mmo_common.hpp>

template<typename T>
struct ThreadSafeQueue
{
    std::deque<T> _deQ;
    std::condition_variable _cv;
    std::mutex _mux;

    ThreadSafeQueue<T>() = default;
    ThreadSafeQueue<T>(const ThreadSafeQueue<T>&) = delete;
    virtual ~ThreadSafeQueue<T>();

    const T& Front();
    const T& Back();

    void PushBack(const T& item);

    void PushFront(const T& item);

    void Clear();
    /* Do not use "PopFrontWithSleep()". Instead of it use "PopFrontWithoutSleep()" +  "Wait()" */
    T PopFrontWithSleep();

    T PopFrontWithoutSleep();
    void Wait();

    bool Empty();

};

template<typename T>
inline ThreadSafeQueue<T>::~ThreadSafeQueue<T>()
{
    this->Clear();
}

template<typename T>
inline const T& ThreadSafeQueue<T>::Front()
{
    std::scoped_lock lock(_mux);
    return _deQ.front();
}

template<typename T>
inline const T& ThreadSafeQueue<T>::Back()
{
    std::scoped_lock lock(_mux);
    return _deQ.front();
}

template<typename T>
inline void ThreadSafeQueue<T>::PushBack(const T& item)
{
    {
        std::unique_lock<std::mutex> ul(_mux);
        _deQ.push_back(item);
    }

    /*  Notifies using condition variable that item is available to be consumed */
    _cv.notify_one(); 
}

template<typename T>
inline void ThreadSafeQueue<T>::PushFront(const T& item)
{
    {
        std::unique_lock<std::mutex> ul(_mux);
        _deQ.push_front(item);
    }

    /*  Notifies using condition variable that item is available to be consumed */
    _cv.notify_one();
}

template<typename T>
inline void ThreadSafeQueue<T>::Clear()
{
    std::scoped_lock lock(_mux);
    return _deQ.clear();
}

template<typename T>
inline T ThreadSafeQueue<T>::PopFrontWithSleep()
{
    std::unique_lock<std::mutex> gaurd{_mux};

    _cv.wait(gaurd, [&](){
        return !_deQ.empty();
    });
    /* wait{aka sleep} till deQueue is not empty */

    auto t = std::move(_deQ.front());
    _deQ.pop_front();

    return t;
}

template<typename T>
void ThreadSafeQueue<T>::Wait()
{
    std::unique_lock<std::mutex> gaurd{_mux};
    _cv.wait(gaurd, [&](){
        return !_deQ.empty();
    });
    /* wait{aka sleep} till deQueue is not empty */
}

template<typename T>
inline T ThreadSafeQueue<T>::PopFrontWithoutSleep()
{
    std::scoped_lock lock(_mux);
	auto t = std::move(_deQ.front());
	_deQ.pop_front();
	return t;
}

template<typename T>
inline bool ThreadSafeQueue<T>::Empty()
{
    std::scoped_lock lock(_mux);
    return _deQ.empty();
}

