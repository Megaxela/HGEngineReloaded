#pragma once

#include <CurrentLogger.hpp>
#include "ResourceAccessor.hpp"

namespace CORE_MODULE_NS
{
    /**
     * @brief Class, that describes resource manager.
     * @tparam Accessor Files accessor type. (Can't be changed on runtime)
     */
    class ResourceManager
    {
        friend class Application;

    public:

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
        void setResourceAccessor()
        {
            if (m_accessor != nullptr)
            {
                Warning() << "Redefining ResourceManager accessor is bad practice.";
                delete m_accessor;
                m_accessor = nullptr;
            }

            m_accessor = new Accessor;
        }



    private:

        ResourceAccessor* m_accessor;
    };
}

