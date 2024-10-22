
#pragma once

// C++ STL
#include <chrono>

namespace HG::Core
{
class Application;
}

namespace HG::Physics::Base
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
    explicit PhysicsController(HG::Core::Application* application);

    // Disable copying
    PhysicsController(const PhysicsController&) = delete;
    PhysicsController& operator=(const PhysicsController&) = delete;

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
    [[nodiscard]] HG::Core::Application* application() const;

private:
    HG::Core::Application* m_parent;
};
} // namespace HG::Physics::Base
