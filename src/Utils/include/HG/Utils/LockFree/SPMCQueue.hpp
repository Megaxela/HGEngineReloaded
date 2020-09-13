#pragma once

// STL
#include <cstdint>

namespace HG::Utils::LockFree
{
template <typename T>
class SPMCQueue
{
public:
    explicit SPMCQueue(std::size_t size) : m_buffer(new T[size]()), m_size(size), m_first(0), m_last(0)
    {
    }

    ~SPMCQueue()
    {
        delete[] m_buffer;
    }

    void clear()
    {
        m_first.store(0, std::memory_order_relaxed);
        m_last.store(0, std::memory_order_relaxed);
    }

    bool push(T&& value)
    { // only for single producer
        auto last = m_last.load(std::memory_order_relaxed);
        auto next = inc(last);
        if (next != m_first.load(std::memory_order_acquire))
        {
            return false;
        }
        m_buffer[last] = std::move(value);
        //            ASSERT(next != m_first.load(std::memory_order_relaxed), "Queue overflow");
        m_last.store(next, std::memory_order_release);

        return true;
    }

    bool isEmpty()
    {
        return m_first.load(std::memory_order_relaxed) == m_last.load(std::memory_order_relaxed);
    }

    void pop(T& result)
    {   // for single consumer. check isEmpty() before
        //            ASSERT(!isEmpty(), "Queue is empty");
        auto first = m_first.load(std::memory_order_acquire);
        result     = m_buffer[first];
        m_first.store(inc(first), std::memory_order_release);
    }

    bool tryPop(T& result)
    { // for multiple consumers. don't use isEmpty()
        while (true)
        {
            std::size_t first = m_first.load(std::memory_order_relaxed);
            if (first == m_last.load(std::memory_order_acquire))
                return false;
            result = m_buffer[first];
            if (m_first.compare_exchange_weak(first, inc(first), std::memory_order_release))
                return true;
        }
    }

private:
    std::size_t inc(std::size_t n)
    {
        return (n + 1) % m_size;
    }

    T* m_buffer;
    std::size_t m_size;
    std::atomic<std::size_t> m_first;
    std::atomic<std::size_t> m_last;
};
} // namespace HG::Utils::LockFree
