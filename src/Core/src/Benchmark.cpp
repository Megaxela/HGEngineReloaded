// HG::Core
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Application.hpp>

HG::Core::Benchmark::ScopeJob::ScopeJob(HG::Core::Application *application, std::string name) :
    m_benchmark(application->benchmark())
{
    m_benchmark->startJob(std::move(name));
}

HG::Core::Benchmark::ScopeJob::~ScopeJob()
{
    m_benchmark->stopJob();
}

HG::Core::Benchmark::Benchmark() :
    m_timingsMutex(),
    m_timings(),
    m_start(),
    m_finish(),
    m_isRunning(false)
{

}

void HG::Core::Benchmark::startJob(std::string name)
{
    if (!m_isRunning)
    {
        return;
    }

    const auto thisThreadId = std::this_thread::get_id();

    m_timingsMutex.lock_shared();
    auto timingsIterator = m_timings.find(thisThreadId);
    m_timingsMutex.unlock_shared();

    // If it's first thread register
    if (timingsIterator == m_timings.end())
    {
        std::unique_lock<std::shared_mutex> lock(m_timingsMutex);

        timingsIterator = m_timings.insert({thisThreadId, ThreadContainer()}).first;
    }

    if (!timingsIterator->second.countingJobs.empty())
    {
        timingsIterator->second.countingJobs.top().subjobs += 1;
    }

    // Getting time
    timingsIterator->second.countingJobs.emplace(
            std::move(name),
            std::chrono::steady_clock::now()
    );
}

void HG::Core::Benchmark::stopJob()
{
    if (!m_isRunning)
    {
        return;
    }

    const auto thisThreadId = std::this_thread::get_id();

    auto time = std::chrono::steady_clock::now();

    m_timingsMutex.lock_shared();
    auto timingsIterator = m_timings.find(thisThreadId);
    m_timingsMutex.unlock_shared();

    if (timingsIterator == m_timings.end() ||
        timingsIterator->second.countingJobs.empty())
    {
        throw std::runtime_error("No job to stop");
    }

    timingsIterator->second.countingJobs.top().finishTime = time;
    timingsIterator->second.doneJobs.emplace(
        timingsIterator->second.countingJobs.top()
    );
    timingsIterator->second.countingJobs.pop();
}

std::size_t HG::Core::Benchmark::numberOfActiveJobs()
{
    const auto thisThreadId = std::this_thread::get_id();

    m_timingsMutex.lock_shared();
    auto timingsIterator = m_timings.find(thisThreadId);
    m_timingsMutex.unlock_shared();

    if (timingsIterator == m_timings.end())
    {
        return 0;
    }

    return timingsIterator->second.countingJobs.size();
}

void HG::Core::Benchmark::getThreadsId(std::vector<std::thread::id> &ids)
{
    std::shared_lock<std::shared_mutex> lock(m_timingsMutex);

    ids.reserve(m_timings.size());

    for (const auto& [key, value] : m_timings)
    {
        ids.push_back(key);
    }
}

std::stack<HG::Core::Benchmark::JobContainer> HG::Core::Benchmark::getClosedJobs(std::thread::id id)
{
    std::shared_lock<std::shared_mutex> lock(m_timingsMutex);

    auto iter = m_timings.find(id);

    if (iter == m_timings.end())
    {
        throw std::invalid_argument("No job container for given thread id");
    }

    return iter->second.doneJobs;
}

HG::Core::Benchmark::TimeType HG::Core::Benchmark::startTime() const
{
    return m_start;
}

HG::Core::Benchmark::TimeType HG::Core::Benchmark::finishTime() const
{
    if (!m_isRunning)
    {
        return m_finish;
    }

    return std::chrono::steady_clock::now();
}

void HG::Core::Benchmark::clear()
{
    std::unique_lock<std::shared_mutex> lock(m_timingsMutex);

    m_timings.clear();
    m_start = std::chrono::steady_clock::time_point();
    m_finish = std::chrono::steady_clock::time_point();
}

void HG::Core::Benchmark::tick()
{
    if (m_isRunning != m_wantRunning)
    {
        m_isRunning = m_wantRunning;

        if (m_wantRunning)
        {
            m_start = std::chrono::steady_clock::now();
        }
        else
        {
            m_finish = std::chrono::steady_clock::now();
        }
    }
    else if (!m_isRunning)
    {
        return;
    }

    m_frameTimes.emplace_back(
        std::chrono::steady_clock::now()
    );
}

void HG::Core::Benchmark::start()
{
    m_wantRunning = true;
}

void HG::Core::Benchmark::finish()
{
    m_wantRunning = false;
}

bool HG::Core::Benchmark::isRunning() const
{
    return m_isRunning;
}

std::vector<HG::Core::Benchmark::TimeType> HG::Core::Benchmark::frameTimes() const
{
    return m_frameTimes;
}
