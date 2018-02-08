#pragma once

#include <chrono>
#include "Application.hpp"

namespace CORE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * application scene.
     */
    class Scene
    {
    public:

        /**
         * @brief Constructor.
         */
        Scene();

        /**
         * @brief Destructor.
         */
        virtual ~Scene() = default;

        /**
         * @brief Method for setting application
         * instance.
         * @param application Pointer to application.
         */
        void setApplication(Application* application);

        /**
         * @brief Method for getting current application
         * instance.
         * @return Pointer to application.
         */
        Application* application() const;

        /**
         * @brief
         */
        void update();

    private:
        Application* m_mainApplication;
    };
}

