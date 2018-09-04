#pragma once

#include <Data.hpp> // Required, because of template `load` method
#include <CurrentLogger.hpp>
#include <memory>
#include <thread>
#include <future>
#include <functional>
#include <FutureHandler.hpp>

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
        ResourceManager();

        /**
         * @brief Destructor.
         */
        virtual ~ResourceManager();

        /**
         * @brief Method for getting resource accessor for
         * low level resources loading.
         * @return Pointer to resource accessor.
         */
        ResourceAccessor* resourceAccessor() const;

        /**
         * @brief Method for setting resource accessor for
         * resource manager. Implemented as template method
         * to derive creation to <code>ResourceManager</code>
         * class.
         * @param accessor Pointer to resource accessor
         * implementation.
         */
        template<typename Accessor>
        typename std::enable_if<
            std::is_base_of<ResourceAccessor, Accessor>::value
        >::type setResourceAccessor()
        {
            if (m_accessor != nullptr)
            {
                Warning() << "Redefining ResourceAccessor accessor is bad practice.";
                delete m_accessor;
                m_accessor = nullptr;
            }

            m_accessor = new Accessor;
        }

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
        >::Ptr load(const std::string& id)
        {
            auto promise = std::promise<typename Loader::ResultType>();

            auto future = promise.get_future();

            std::thread performer(
                std::bind(
                    [this, id](std::promise<typename Loader::ResultType> &promise)
                    {
                        auto data = loadRawFromAccessor(id);

                        if (data == nullptr)
                        {
                            return promise.set_value(nullptr);
                        }

                        Loader loader;

                        promise.set_value(loader.load(data->data(), data->size()));
                    },
                    std::move(promise)
                )
            );

            performer.detach();

            return std::make_shared<
                typename HG::Utils::FutureHandler<
                    typename Loader::ResultType
                >
           >(std::move(future));
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

        ResourceAccessor* m_accessor;
    };
}

