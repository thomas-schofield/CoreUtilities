#pragma once

#include <queue>
#include <mutex>
#include <cstdlib>
#include <iostream>

const std::size_t UNLIMITED_QUEUE_SIZE = -1;

namespace core_utilities
{
template <class T, class Allocator = std::allocator<T>>
class ThreadSafeDeque
{
  public:
    ThreadSafeDeque(std::size_t in_queue_size) :
      queue_size(in_queue_size)
    {
    }

    ThreadSafeDeque() :
        queue_size(UNLIMITED_QUEUE_SIZE)
    {
    }

    void setQueueSize(std::size_t queue_size)
    {
        this->queue_size = queue_size;
    }

    bool push_back(const T& value)
    {
        if (UNLIMITED_QUEUE_SIZE == queue_size || deque.size() <= queue_size)
        {
            std::lock_guard<std::mutex> lock(deque_mutex);
            deque.push_back(value);
            return true;
        }
        else
        {
            std::cout << "Queue was full!!!\n";
        }

        return false;
    }

    bool pop_front(T& out)
    {
        std::lock_guard<std::mutex> lock(deque_mutex);
        if (deque.empty())
        {
            return false;
        }

        out = std::move(deque.front());
        deque.pop_front();
        return true;
    }

  private:
    std::deque<T> deque;
    mutable std::mutex deque_mutex;
    std::size_t queue_size;
};
}
