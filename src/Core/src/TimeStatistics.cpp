#include <stdexcept>
#include "TimeStatistics.hpp"
#include <algorithm>

CORE_MODULE_NS::TimeStatistics::TimeStatistics() :
    m_timers()
{
    addTimer(Timers::FrameTime);
    changeEstimateBuffer(Timers::FrameTime,  60);
    addTimer(Timers::RenderTime);
    changeEstimateBuffer(Timers::RenderTime, 60);
    addTimer(Timers::UpdateTime);
    changeEstimateBuffer(Timers::UpdateTime, 60);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::frameDeltaTime()
{
    return getTimerEstimate(FrameTime);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::lastFrameDeltaTime()
{
    return getTimerLastFrame(FrameTime);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::renderTime()
{
    return getTimerEstimate(RenderTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::lastFrameRenderTime()
{
    return getTimerLastFrame(RenderTime);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::updateTime()
{
    return getTimerEstimate(UpdateTime);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::lastFrameUpdateTime()
{
    return getTimerLastFrame(UpdateTime);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::getTimerEstimate(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.estimateTime();
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::getTimerLastFrame(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.lastFrameTime();
}

void CORE_MODULE_NS::TimeStatistics::changeEstimateBuffer(int timer, std::size_t numberOfFrames)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.changeEstimateBuffer(numberOfFrames);
}

void CORE_MODULE_NS::TimeStatistics::tickTimer(int timer, std::chrono::microseconds microseconds)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tick(microseconds);
}

void CORE_MODULE_NS::TimeStatistics::tickTimerBegin(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tickBegin();
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::tickTimerEnd(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.tickEnd();
}

void CORE_MODULE_NS::TimeStatistics::addTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator != m_timers.end())
    {
        throw std::invalid_argument("Timer with id " + std::to_string(timer) + " is already presented.");
    }

    m_timers[timer] = Timer();
}

void CORE_MODULE_NS::TimeStatistics::removeTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    m_timers.erase(iterator);
}

bool CORE_MODULE_NS::TimeStatistics::hasTimer(int timer) const
{
    return m_timers.find(timer) != m_timers.end();
}

CORE_MODULE_NS::TimeStatistics::Timer::Timer() :
    m_buffer(),
    m_insertPosition(0),
    m_bufferSize(0),
    m_timerStarted(false),
    m_timerStart()
{
    changeEstimateBuffer(30);
}

void CORE_MODULE_NS::TimeStatistics::Timer::tick(std::chrono::microseconds mcs)
{
    m_buffer[m_insertPosition++] = mcs;

    if (m_insertPosition >= m_bufferSize)
    {
        m_insertPosition = 0;
    }
}

void CORE_MODULE_NS::TimeStatistics::Timer::tickBegin()
{
    m_timerStarted = true;
    m_timerStart = std::chrono::steady_clock::now();
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::Timer::tickEnd()
{
    if (!m_timerStarted)
    {
        throw std::runtime_error("Trying to finish timer without running timer.");
    }

    auto diff = std::chrono::duration_cast<
        std::chrono::microseconds
    >(std::chrono::steady_clock::now() - m_timerStart);

    m_timerStarted = false;

    tick(diff);

    return diff;
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::Timer::estimateTime() const
{
    int64_t currentEstimate = 0;

    for (std::size_t i = 0; i < m_bufferSize; ++i)
    {
        currentEstimate += double(m_buffer[i].count() - currentEstimate) / (i + 1);
    }

    return std::chrono::microseconds(currentEstimate);
}

std::chrono::microseconds CORE_MODULE_NS::TimeStatistics::Timer::lastFrameTime() const
{
    if (m_insertPosition == 0)
    {
        return m_buffer[m_bufferSize - 1];
    }

    return m_buffer[m_insertPosition - 1];
}

void CORE_MODULE_NS::TimeStatistics::Timer::changeEstimateBuffer(std::size_t count)
{
    // todo: implement documentation behaviour
    if (count > m_buffer.size())
    {
        m_buffer.resize(count, std::chrono::microseconds(0));
    }

    m_bufferSize = count;
}
