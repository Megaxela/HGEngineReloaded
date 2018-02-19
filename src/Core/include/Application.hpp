#pragma once

#include <Renderer.hpp>
#include "ResourceManager.hpp"

namespace CORE_MODULE_NS
{
    class Scene;

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
        explicit Application(int argc=0, char** argv=nullptr);

        /**
         * @brief Destructor.
         */
        ~Application() = default;

        /**
         * @brief Method for setting current scene.
         * Actual scene change will happen at next
         * frame begin.
         * @param scene Pointer to scene object.
         * `nullptr` will throw `std::invalid_argument`
         * exception.
         */
        void setScene(Scene* scene);

        /**
         * @brief Method for getting current or cached scene.
         * @return Current or cached scene.
         */
        Scene* scene() const;

        /**
         * @brief Method for stopping main
         * application method.
         */
        void stop();

        /**
         * @brief Method for executing application.
         * @return Result code.
         */
        int exec();

        /**
         * @brief Method for getting pointer to
         * application renderer.
         * @return Pointer to renderer.
         */
        Rendering::Base::Renderer* renderer() const;

        /**
         * @brief Method for getting pointer to application
         * resource manager.
         * @return Pointer to resource manager.
         */
        ResourceManager* resourceManager() const;

    private:

        /**
         * @brief Method for processing
         * scene swapping.
         */
        void proceedScene();

        // Renderer object
        Rendering::Base::Renderer m_renderer;

        // Resource manager
        ResourceManager m_resourceManager;

        // Scene has to be changed only at new frame.
        // Using caching new scene, until new frame will begin.
        Scene* m_currentScene;
        Scene* m_cachedScene;

        // While this variable is true, game cycle is active
        bool m_working;
    };
}