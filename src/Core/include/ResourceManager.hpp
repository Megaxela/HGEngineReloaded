#pragma once

// HG::Core
#include <Data.hpp> // Required, because of template `load` method.
#include <Application.hpp> // Required, because of template `load` method.
#include <ThreadPool.hpp>

// HG::Utils
#include <FutureHandler.hpp>

// ALogger
#include <CurrentLogger.hpp>

// C++ STL
#include <memory>
#include <thread>
#include <future>
#include <functional>
#include <queue>

namespace HG::Core
{
    class ResourceAccessor;

    /**
     * @brief Class, that describes resource manager.
     */
    class ResourceManager
    {
        friend class Application;

    public:

        // Disable copying
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        /**
         * @brief Constructor.
         */
        explicit ResourceManager(HG::Core::Application* parent);

        /**
         * @brief Destructor.
         */
        virtual ~ResourceManager();

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application();

        /**
         * @brief Method for getting resource accessor for
         * low level resources loading.
         * @return Pointer to resource accessor.
         */
        HG::Core::ResourceAccessor* resourceAccessor() const;

        /**
         * @brief Method for setting resource accessor for
         * resource manager. Implemented as template method
         * to derive creation to <code>ResourceManager</code>
         * class.
         * @param accessor Pointer to resource accessor
         * implementation.
         */
        void setResourceAccessor(HG::Core::ResourceAccessor* accessor);

        /**
         * @brief Method for loading some resource
         * with using of specified formatter.
         * @tparam Loader Formatter object.
         *
         * Formatter object concept:
         *
         * concept Loader
         * {
         * public:
         *     using ResultType;
         *
         *     ResultType load(const std::byte* data, std::size_t size);
         * }
         *
         * @param id Resource id for resource accessor.
         * @return Loaded resource or nullptr if error aquired.
         */
        template<typename Loader>
        typename HG::Utils::FutureHandler<
            typename Loader::ResultType
        > load(const std::string& id)
        {
            return application()->threadPool()->push(
                [this, id]() -> typename Loader::ResultType
                {
                    auto data = loadRawFromAccessor(id);

                    if (data == nullptr)
                    {
                        return nullptr;
                    }

                    Loader loader;

                    return loader.load(data->data(), data->size());
                },
                HG::Core::ThreadPool::Type::FileLoadingThread
            );
        }

    private:

        /**
         * @brief Method for loading data from accessor and
         * log errors if smth will go wrong.
         * @param id Resource id for resource accessor.
         * @return Pointer to raw data, or nullptr if
         * some error acquired.
         */
        HG::Core::DataPtr loadRawFromAccessor(const std::string& id);

        HG::Core::ResourceAccessor* m_accessor;
        HG::Core::Application* m_application;
    };
}

