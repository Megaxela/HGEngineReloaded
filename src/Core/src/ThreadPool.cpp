// HG::Core
#include <HG/Core/ThreadPool.hpp>

// HG::Utils

// ALogger
#include <CurrentLogger.hpp>

HG::Core::ThreadPool::ThreadPool() :
    m_data()
{
    auto hw = std::thread::hardware_concurrency();

    // Starting file loading thread
    startPool(Type::FileLoadingThread, 1);

    // Starting user thread
    if (hw > 2)
    {
        startPool(Type::UserThread, hw - 2);
    }
    else
    {
        startPool(Type::UserThread, 1);
    }
}

HG::Core::ThreadPool::~ThreadPool()
{
    for (auto&& [type, data] : m_data)
    {
        stopPool(type);
    }

    for (auto&& [type, data] : m_data)
    {
        joinPool(type);
    }
}

void HG::Core::ThreadPool::threadFunction(std::size_t id,
                                          HG::Core::ThreadPool::Type type)
{
    auto data = getPoolData(type);

    if (data == nullptr) // WTF??
    {
        Error() << "Thread function can't get it's pool data. Closing thread.";
        return;
    }

    while (data->running)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> queueLock(data->jobsMutex);

            while (data->jobs.empty() &&
                   data->running)
            {
                data->jobsNotifier.wait(queueLock);
            }

            if (!data->running)
            {
                break;
            }

            job = data->jobs.front();
            data->jobs.pop();
        }

        try
        {
            job();
        }
        catch (std::exception& exception)
        {
            Error() << "Thread from pool with [id=" << id << ", type=" << type << " received exception: " << exception.what();
        }
    }

    // Removing thread from data
//    std::unique_lock<std::mutex> lock(data->threadsMutex);
//    data->threads.erase(id);
}

void HG::Core::ThreadPool::pushInternal(std::function<void()> job,
                                        HG::Core::ThreadPool::Type type)
{
    auto data = getPoolData(type);

    if (data == nullptr)
    {
        return;
    }

    {
        std::unique_lock<std::mutex> lock (data->jobsMutex);
        data->jobs.push(std::move(job));
    }

    data->jobsNotifier.notify_all();
}

void HG::Core::ThreadPool::startPool(HG::Core::ThreadPool::Type type,
                                     std::size_t numberOfThreads)
{
    std::shared_ptr<PoolData> data = nullptr;

    m_dataMutex.lock();
    {
        auto iter = m_data.find(type);

        if (iter == m_data.end())
        {
            data = std::make_shared<PoolData>();
            m_data[type] = data;
        }
        else
        {
            data = iter->second;
        }
    }
    m_dataMutex.unlock();

    // Can't rerun pool
    if (data->running)
    {
        return;
    }

    data->running = true;

    for (std::size_t i = 0; i < numberOfThreads; ++i)
    {
        std::unique_lock<std::mutex> lock(data->threadsMutex);
        data->threads[data->nextId] = std::thread(
            &ThreadPool::threadFunction,
            this,
            std::size_t(data->nextId),
            type
        );

        data->nextId++;
    }
}

void HG::Core::ThreadPool::stopPool(HG::Core::ThreadPool::Type type)
{
    std::shared_ptr<PoolData> data = getPoolData(type);

    if (data == nullptr)
    {
        return;
    }

    data->running = false;
    data->jobsNotifier.notify_all();
}

void HG::Core::ThreadPool::joinPool(HG::Core::ThreadPool::Type type)
{
    std::shared_ptr<PoolData> data = getPoolData(type);

    for (auto&& [id, thread] : data->threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

std::shared_ptr<HG::Core::ThreadPool::PoolData> HG::Core::ThreadPool::getPoolData(HG::Core::ThreadPool::Type type) const
{
    std::shared_lock lock(m_dataMutex);

    auto iter = m_data.find(type);

    if (iter == m_data.end())
    {
        // Can't stop unexisting pool
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

std::size_t HG::Core::ThreadPool::numberOfJobs(HG::Core::ThreadPool::Type type) const
{
    auto data = getPoolData(type);

    if (data == nullptr)
    {
        return 0;
    }

    return data->jobs.size();
}