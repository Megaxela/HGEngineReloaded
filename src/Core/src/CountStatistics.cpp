// HG::Core
#include <CountStatistics.hpp>

HG::Core::CountStatistics::CountStatistics() :
    m_counters()
{

}

void HG::Core::CountStatistics::addCounter(int counter, HG::Core::CountStatistics::CounterType type)
{
    auto iterator = m_counters.find(counter);

    if (iterator != m_counters.end())
    {
        throw std::invalid_argument("Counter with id " + std::to_string(counter) + " is already presented.");
    }

    AbstractCounter* counterObject = nullptr;

    switch (type)
    {
    case CounterType::Cumulative: counterObject = new CummulativeCounter(); break;
    case CounterType::LastFrame:  counterObject = new LastFrameCounter(); break;
    }

    m_counters[counter] = counterObject;
}

void HG::Core::CountStatistics::removeCounter(int counter)
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is no counter with id " + std::to_string(counter));
    }

    delete iterator->second;
    m_counters.erase(counter);
}

bool HG::Core::CountStatistics::hasCounter(int counter) const
{
    return m_counters.find(counter) != m_counters.end();
}

HG::Core::CountStatistics::ValueType HG::Core::CountStatistics::value(int counter) const
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is not counter with id " + std::to_string(counter));
    }

    return iterator->second->value();
}

void HG::Core::CountStatistics::reset(int counter)
{
    auto iterator = m_counters.find(counter);

    if (iterator == m_counters.end())
    {
        throw std::invalid_argument("There is not counter with id " + std::to_string(counter));
    }

    return iterator->second->reset();
}

void HG::Core::CountStatistics::frameChanged()
{
    for (auto&& [id, counter] : m_counters)
    {
        counter->frameChanged();
    }
}

HG::Core::CountStatistics::AbstractCounter::AbstractCounter(HG::Core::CountStatistics::CounterType type) :
    m_type(type)
{

}

HG::Core::CountStatistics::CounterType HG::Core::CountStatistics::AbstractCounter::type() const
{
    return m_type;
}

void HG::Core::CountStatistics::AbstractCounter::frameChanged()
{

}

HG::Core::CountStatistics::CummulativeCounter::CummulativeCounter() :
    AbstractCounter(CounterType::Cumulative),
    m_value(0)
{

}

void HG::Core::CountStatistics::CummulativeCounter::reset()
{
    m_value = 0;
}

void HG::Core::CountStatistics::CummulativeCounter::add(HG::Core::CountStatistics::ValueType value)
{
    m_value += value;
}

HG::Core::CountStatistics::ValueType HG::Core::CountStatistics::CummulativeCounter::value() const
{
    return m_value;
}

HG::Core::CountStatistics::LastFrameCounter::LastFrameCounter() :
    AbstractCounter(CounterType::LastFrame),
    m_previousFrameValue(0),
    m_currentFrameValue(0)
{

}

void HG::Core::CountStatistics::LastFrameCounter::frameChanged()
{
    m_previousFrameValue = m_currentFrameValue;
    m_currentFrameValue = 0;
}

void HG::Core::CountStatistics::LastFrameCounter::reset()
{
    m_previousFrameValue = 0;
}

void HG::Core::CountStatistics::LastFrameCounter::add(HG::Core::CountStatistics::ValueType value)
{
    m_currentFrameValue += value;
}

HG::Core::CountStatistics::ValueType HG::Core::CountStatistics::LastFrameCounter::value() const
{
    return m_previousFrameValue;
}
