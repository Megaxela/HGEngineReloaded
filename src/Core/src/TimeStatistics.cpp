// C++ STL
#include <algorithm>
#include <stdexcept>
#include <string>

// HG::Core
#include <HG/Core/TimeStatistics.hpp>

namespace HG::Core
{
TimeStatistics::TimeStatistics() : m_timers()
{
    addTimer(Timers::FrameTime);
    changeEstimateBuffer(Timers::FrameTime, 60);
    addTimer(Timers::RenderTime);
    changeEstimateBuffer(Timers::RenderTime, 60);
    addTimer(Timers::UpdateTime);
    changeEstimateBuffer(Timers::UpdateTime, 60);
    addTimer(Timers::PhysicsTime);
    changeEstimateBuffer(Timers::PhysicsTime, 60);
}

std::chrono::microseconds TimeStatistics::frameDeltaTime() const
{
    return getTimerEstimate(FrameTime);
}

std::chrono::microseconds TimeStatistics::lastFrameDeltaTime() const
{
    return getTimerLastFrame(FrameTime);
}

std::chrono::microseconds TimeStatistics::physicsTime() const
{
    return getTimerEstimate(PhysicsTime);
}

std::chrono::microseconds TimeStatistics::renderTime() const
{
    return getTimerEstimate(RenderTime);
}

std::chrono::microseconds TimeStatistics::lastFrameRenderTime() const
{
    return getTimerLastFrame(RenderTime);
}

std::chrono::microseconds TimeStatistics::updateTime() const
{
    return getTimerEstimate(UpdateTime);
}

std::chrono::microseconds TimeStatistics::lastFrameUpdateTime() const
{
    return getTimerLastFrame(UpdateTime);
}

std::chrono::microseconds TimeStatistics::getTimerEstimate(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.estimateTime();
}

std::chrono::microseconds TimeStatistics::getTimerLastFrame(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.lastFrameTime();
}

void TimeStatistics::changeEstimateBuffer(int timer, std::size_t numberOfFrames)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.changeEstimateBuffer(numberOfFrames);
}

void TimeStatistics::tickTimer(int timer, std::chrono::microseconds microseconds)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tick(microseconds);
}

void TimeStatistics::tickTimerBegin(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tickBegin();
}

std::chrono::microseconds TimeStatistics::tickTimerEnd(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.tickEnd();
}

std::chrono::microseconds TimeStatistics::tickTimerAtomic(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    std::chrono::microseconds time(0);

    if (iterator->second.isTimerStarted())
    {
        time = iterator->second.tickEnd();
    }

    iterator->second.tickBegin();

    return time;
}

void TimeStatistics::addTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator != m_timers.end())
    {
        throw std::invalid_argument("Timer with id " + std::to_string(timer) + " is already presented.");
    }

    m_timers[timer] = Timer();
}

void TimeStatistics::removeTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    m_timers.erase(iterator);
}

bool TimeStatistics::hasTimer(int timer) const
{
    return m_timers.find(timer) != m_timers.end();
}

TimeStatistics::Timer::Timer() : m_buffer(), m_insertPosition(0), m_bufferSize(0), m_timerStarted(false), m_timerStart()
{
    changeEstimateBuffer(30);
}

void TimeStatistics::Timer::tick(std::chrono::microseconds mcs)
{
    m_buffer[m_insertPosition++] = mcs;

    if (m_insertPosition >= m_bufferSize)
    {
        m_insertPosition = 0;
    }
}

void TimeStatistics::Timer::tickBegin()
{
    m_timerStarted = true;
    m_timerStart   = std::chrono::steady_clock::now();
}

std::chrono::microseconds TimeStatistics::Timer::tickEnd()
{
    if (!m_timerStarted)
    {
        throw std::runtime_error("Trying to finish timer without running timer.");
    }

    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_timerStart);

    m_timerStarted = false;

    tick(diff);

    return diff;
}

std::chrono::microseconds TimeStatistics::Timer::estimateTime() const
{
    std::int64_t currentEstimate = 0;

    for (std::size_t i = 0; i < m_bufferSize; ++i)
    {
        currentEstimate += double(m_buffer[i].count() - currentEstimate) / (i + 1);
    }

    return std::chrono::microseconds(currentEstimate);
}

std::chrono::microseconds TimeStatistics::Timer::lastFrameTime() const
{
    if (m_insertPosition == 0)
    {
        return m_buffer[m_bufferSize - 1];
    }

    return m_buffer[m_insertPosition - 1];
}

void TimeStatistics::Timer::changeEstimateBuffer(std::size_t count)
{
    // todo: implement documentation behaviour
    if (count > m_buffer.size())
    {
        m_buffer.resize(count, std::chrono::microseconds(0));
    }

    m_bufferSize = count;
}

bool TimeStatistics::Timer::isTimerStarted() const
{
    return m_timerStarted;
}
} // namespace HG::Core
