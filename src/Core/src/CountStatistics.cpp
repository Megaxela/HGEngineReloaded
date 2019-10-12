// HG::Core
#include <HG/Core/CountStatistics.hpp>

namespace HG::Core
{
CountStatistics::CountStatistics() : m_counters()
{
}

CountStatistics::~CountStatistics()
{
    for (auto&& [id, counter] : m_counters)
    {
        delete counter;
    }

    m_counters.clear();
}

void CountStatistics::addCounter(int counter, CountStatistics::CounterType type)
{
    auto iterator = m_counters.find(counter);

    if (iterator != m_counters.end())
    {
        throw std::invalid_argument("Counter with id " + std::to_string(counter) + " is already presented.");
    }

    AbstractCounter* counterObject = nullptr;

    switch (type)
    {
    case CounterType::Cumulative:
        counterObject = new CummulativeCounter();
        break;
    case CounterType::LastFrame:
        counterObject = new LastFrameCounter();
        break;
    }

    m_counters[counter] = counterObject;
}

void CountStatistics::removeCounter(int counter)
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is no counter with id " + std::to_string(counter));
    }

    delete iterator->second;
    m_counters.erase(counter);
}

bool CountStatistics::hasCounter(int counter) const
{
    return m_counters.find(counter) != m_counters.end();
}

CountStatistics::ValueType CountStatistics::value(int counter) const
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is not counter with id " + std::to_string(counter));
    }

    return iterator->second->value();
}

void CountStatistics::add(int counter, CountStatistics::ValueType value)
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is not counter with id " + std::to_string(counter));
    }

    return iterator->second->add(value);
}

void CountStatistics::reset(int counter)
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is not counter with id " + std::to_string(counter));
    }

    return iterator->second->reset();
}

void CountStatistics::frameChanged()
{
    for (auto&& [id, counter] : m_counters)
    {
        counter->frameChanged();
    }
}

CountStatistics::AbstractCounter::AbstractCounter(CountStatistics::CounterType type) : m_type(type)
{
}

CountStatistics::CounterType CountStatistics::AbstractCounter::type() const
{
    return m_type;
}

void CountStatistics::AbstractCounter::frameChanged()
{
}

CountStatistics::CummulativeCounter::CummulativeCounter() : AbstractCounter(CounterType::Cumulative), m_value(0)
{
}

void CountStatistics::CummulativeCounter::reset()
{
    m_value = 0;
}

void CountStatistics::CummulativeCounter::add(CountStatistics::ValueType value)
{
    m_value += value;
}

CountStatistics::ValueType CountStatistics::CummulativeCounter::value() const
{
    return m_value;
}

CountStatistics::LastFrameCounter::LastFrameCounter() :
    AbstractCounter(CounterType::LastFrame),
    m_previousFrameValue(0),
    m_currentFrameValue(0)
{
}

void CountStatistics::LastFrameCounter::frameChanged()
{
    m_previousFrameValue = m_currentFrameValue;
    m_currentFrameValue  = 0;
}

void CountStatistics::LastFrameCounter::reset()
{
    m_previousFrameValue = 0;
}

void CountStatistics::LastFrameCounter::add(CountStatistics::ValueType value)
{
    m_currentFrameValue += value;
}

CountStatistics::ValueType CountStatistics::LastFrameCounter::value() const
{
    return m_previousFrameValue;
}
} // namespace HG::Core
