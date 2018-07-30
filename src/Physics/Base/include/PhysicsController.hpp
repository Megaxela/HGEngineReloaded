
#pragma once

#include <chrono>

namespace CORE_MODULE_NS
{
    class Application;
}

namespace PHYSICS_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes abstract physics controller.
     */
    class PhysicsController
    {
    public:

        /**
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit PhysicsController(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Destructor.
         */
        virtual ~PhysicsController() = default;

        /**
         * @brief Method for ticking physics engine for defined time.
         * @param deltaTime Delta time in milliseconds.
         */
        virtual void tick(std::chrono::microseconds deltaTime) = 0;

        /**
         * @brief Method for getting pointer to parent application.
         * @return Pointer to parent application.
         */
        ::CORE_MODULE_NS::Application* application() const;

    private:

        ::CORE_MODULE_NS::Application* m_parent;
    };
}



