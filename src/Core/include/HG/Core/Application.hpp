#pragma once

// C++ STL
#include <string>

namespace HG::Physics::Base
{
class PhysicsController;
}

namespace HG::Rendering::Base
{
class SystemController;
class Renderer;
} // namespace HG::Rendering::Base

namespace HG::Core
{
class Scene;
class Input;
class ResourceManager;
class TimeStatistics;
class CountStatistics;
class ThreadPool;
class Benchmark;
class ResourceCache;

/**
 * @brief Class, that describes
 * application instance.
 */
class Application
{
public:
    /**
     * @brief Constructor.
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    explicit Application(std::string name, int argc = 0, char** argv = nullptr);

    /**
     * @brief Destructor.
     * It will delete controllers. (Deletion
     * will be executed with `delete` operator)
     */
    virtual ~Application();

    /**
     * @brief Method for setting current scene.
     * Actual scene change will happen at next
     * frame begin.
     * @param scene Pointer to scene object.
     * `nullptr` will throw `std::invalid_argument`
     * exception.
     */
    void setScene(HG::Core::Scene* scene);

    /**
     * @brief Method for getting current or cached scene.
     * @return Current or cached scene.
     */
    [[nodiscard]] HG::Core::Scene* scene() const;

    /**
     * @brief Method for stopping main
     * application method.
     */
    void stop();

    /**
     * @brief Initialization.
     * @return Initialization success.
     */
    virtual bool init();

    /**
     * @brief Deinitialization.
     */
    virtual void deinit();

    /**
     * @brief Method for performing one game cycle.
     * @return Cycle success.
     */
    virtual bool performCycle();

    /**
     * @brief Method for executing application.
     * (Automatically performs initialization and
     * performing cycles until window closed)
     * @return Result code.
     */
    virtual int exec();

    /**
     * @brief Method for getting pointer to
     * application renderer.
     * @return Pointer to renderer.
     */
    [[nodiscard]] HG::Rendering::Base::Renderer* renderer() const;

    /**
     * @brief Method for getting pointer to application
     * resource manager.
     * @return Pointer to resource manager.
     */
    [[nodiscard]] HG::Core::ResourceManager* resourceManager() const;

    /**
     * @brief Method for getting object for time
     * calculations.
     * @return Pointer to time statistics object.
     */
    [[nodiscard]] HG::Core::TimeStatistics* timeStatistics() const;

    /**
     * @brief Method for getting object for
     * counting statistics.
     * @return Pointer to count statistics object.
     */
    [[nodiscard]] HG::Core::CountStatistics* countStatistics() const;

    /**
     * @brief Method for getting object
     * for benchmarking.
     * @return Pointer to benchmark.
     */
    [[nodiscard]] HG::Core::Benchmark* benchmark() const;

    /**
     * @brief Method for getting application thread pool.
     * @return Pointer to application thread pool.
     */
    [[nodiscard]] HG::Core::ThreadPool* threadPool() const;

    /**
     * @brief Method for getting application resource cache.
     * @return Pointer to application resource cache.
     */
    [[nodiscard]] HG::Core::ResourceCache* resourceCache() const;

    /**
     * @brief Method for receiving pointer to
     * input controller/receiver. If you are
     * coding behaviour, you shall not using
     * `const_cast` with this pointer.
     * @return
     */
    [[nodiscard]] const HG::Core::Input* input() const;

    /**
     * @brief Method for getting initial application
     * title.
     * @return
     */
    [[nodiscard]] std::string title() const;

    /**
     * @brief Method for setting system controller.
     */
    void setSystemController(HG::Rendering::Base::SystemController* systemController);

    /**
     * @brief Method for setting physics controller.
     */
    void setPhysicsController(HG::Physics::Base::PhysicsController* physicsController);

    /**
     * @brief Method for getting physics controller.
     * @tparam T Type of physics controller. It must be
     * derived from `HG::Physics::Base::PhysicsController`.
     * @return Pointer to physics controller.
     */
    [[nodiscard]] HG::Physics::Base::PhysicsController* physicsController() const;

    /**
     * @brief Method for getting system controller.
     * @return Pointer to system controller or
     * `nullptr` if there is no such.
     */
    [[nodiscard]] HG::Rendering::Base::SystemController* systemController() const;

protected:
    /**
     * @brief Method for processing
     * scene swapping.
     */
    virtual void proceedScene();

private:
    // Title for created window
    std::string m_applicationTitle;

    // Renderer object
    HG::Rendering::Base::Renderer* m_renderer;

    // Application system controller
    HG::Rendering::Base::SystemController* m_systemController;

    // Physics controller
    HG::Physics::Base::PhysicsController* m_physicsController;

    // Thread pool
    HG::Core::ThreadPool* m_threadPool;

    // Input receiver/producer
    HG::Core::Input* m_input;

    // Resource manager
    HG::Core::ResourceManager* m_resourceManager;

    // Time statistics
    HG::Core::TimeStatistics* m_timeStatistics;

    // Count statistics
    HG::Core::CountStatistics* m_countStatistics;

    // Benchmarking
    HG::Core::Benchmark* m_benchmark;

    // Cache for objects
    HG::Core::ResourceCache* m_resourceCache;

    // Scene has to be changed only at new frame.
    // Using caching new scene, until new frame will begin.
    HG::Core::Scene* m_currentScene;
    HG::Core::Scene* m_cachedScene;
};
} // namespace HG::Core
