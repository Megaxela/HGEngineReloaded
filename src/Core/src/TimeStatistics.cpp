// C++ STL
#include <algorithm>
#include <stdexcept>

// HG::Core
#include <HG/Core/TimeStatistics.hpp>

HG::Core::TimeStatistics::TimeStatistics() : m_timers()
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

std::chrono::microseconds HG::Core::TimeStatistics::frameDeltaTime()
{
    return getTimerEstimate(FrameTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::lastFrameDeltaTime()
{
    return getTimerLastFrame(FrameTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::physicsTime()
{
    return getTimerEstimate(PhysicsTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::renderTime()
{
    return getTimerEstimate(RenderTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::lastFrameRenderTime()
{
    return getTimerLastFrame(RenderTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::updateTime()
{
    return getTimerEstimate(UpdateTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::lastFrameUpdateTime()
{
    return getTimerLastFrame(UpdateTime);
}

std::chrono::microseconds HG::Core::TimeStatistics::getTimerEstimate(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.estimateTime();
}

std::chrono::microseconds HG::Core::TimeStatistics::getTimerLastFrame(int timer) const
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.lastFrameTime();
}

void HG::Core::TimeStatistics::changeEstimateBuffer(int timer, std::size_t numberOfFrames)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.changeEstimateBuffer(numberOfFrames);
}

void HG::Core::TimeStatistics::tickTimer(int timer, std::chrono::microseconds microseconds)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tick(microseconds);
}

void HG::Core::TimeStatistics::tickTimerBegin(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    iterator->second.tickBegin();
}

std::chrono::microseconds HG::Core::TimeStatistics::tickTimerEnd(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    return iterator->second.tickEnd();
}

std::chrono::microseconds HG::Core::TimeStatistics::tickTimerAtomic(int timer)
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

void HG::Core::TimeStatistics::addTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator != m_timers.end())
    {
        throw std::invalid_argument("Timer with id " + std::to_string(timer) + " is already presented.");
    }

    m_timers[timer] = Timer();
}

void HG::Core::TimeStatistics::removeTimer(int timer)
{
    auto iterator = m_timers.find(timer);

    if (iterator == m_timers.end())
    {
        throw std::invalid_argument("There is no timer with id " + std::to_string(timer));
    }

    m_timers.erase(iterator);
}

bool HG::Core::TimeStatistics::hasTimer(int timer) const
{
    return m_timers.find(timer) != m_timers.end();
}

HG::Core::TimeStatistics::Timer::Timer() :
    m_buffer(),
    m_insertPosition(0),
    m_bufferSize(0),
    m_timerStarted(false),
    m_timerStart()
{
    changeEstimateBuffer(30);
}

void HG::Core::TimeStatistics::Timer::tick(std::chrono::microseconds mcs)
{
    m_buffer[m_insertPosition++] = mcs;

    if (m_insertPosition >= m_bufferSize)
    {
        m_insertPosition = 0;
    }
}

void HG::Core::TimeStatistics::Timer::tickBegin()
{
    m_timerStarted = true;
    m_timerStart   = std::chrono::steady_clock::now();
}

std::chrono::microseconds HG::Core::TimeStatistics::Timer::tickEnd()
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

std::chrono::microseconds HG::Core::TimeStatistics::Timer::estimateTime() const
{
    int64_t currentEstimate = 0;

    for (std::size_t i = 0; i < m_bufferSize; ++i)
    {
        currentEstimate += double(m_buffer[i].count() - currentEstimate) / (i + 1);
    }

    return std::chrono::microseconds(currentEstimate);
}

std::chrono::microseconds HG::Core::TimeStatistics::Timer::lastFrameTime() const
{
    if (m_insertPosition == 0)
    {
        return m_buffer[m_bufferSize - 1];
    }

    return m_buffer[m_insertPosition - 1];
}

void HG::Core::TimeStatistics::Timer::changeEstimateBuffer(std::size_t count)
{
    // todo: implement documentation behaviour
    if (count > m_buffer.size())
    {
        m_buffer.resize(count, std::chrono::microseconds(0));
    }

    m_bufferSize = count;
}

bool HG::Core::TimeStatistics::Timer::isTimerStarted() const
{
    return m_timerStarted;
}
