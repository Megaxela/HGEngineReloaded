#include <ResourceManager.hpp>
#include <ResourceAccessor.hpp>

HG::Core::ResourceManager::ResourceManager() :
    m_accessor(nullptr),
    m_loaderThread(),
    m_running(true),
    m_loaderNotifier(),
    m_loaderJobs(),
    m_loaderJobsMutex()
{
    m_loaderThread = std::thread(
        &ResourceManager::loaderThread,
        this
    );
}

HG::Core::ResourceManager::~ResourceManager()
{
    m_running = false;
    m_loaderNotifier.notify_all();

    if (m_loaderThread.joinable())
    {
        m_loaderThread.join();
    }

    delete m_accessor;
}

HG::Core::ResourceAccessor* HG::Core::ResourceManager::resourceAccessor() const
{
    return m_accessor;
}

HG::Core::DataPtr HG::Core::ResourceManager::loadRawFromAccessor(const std::string &id)
{
    Info() << "Loading resource \"" << id << "\"";

    if (m_accessor == nullptr)
    {
        Error() << "Trying to load \"" << id << "\" resource, without ResourceAccessor.";
        return nullptr;
    }

    auto data = m_accessor->loadRaw(id);

    if (data == nullptr)
    {
        Error() << "Can't load \"" << id << "\" resource. See errors above.";

        return nullptr;
    }

    return data;
}

void HG::Core::ResourceManager::loaderThread()
{
    while (m_running)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> queueLock(m_loaderJobsMutex);

            while (m_loaderJobs.empty() &&
                   m_running)
            {
                m_loaderNotifier.wait(queueLock);
            }

            if (!m_running)
            {
                break;
            }

            job = m_loaderJobs.front();
            m_loaderJobs.pop();
        }

        job();
    }
}
