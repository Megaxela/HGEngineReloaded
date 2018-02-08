#pragma once

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

    private:

        /**
         * @brief Method for processing
         * scene swapping.
         */
        void proceedScene();

        // Scene has to be changed only at new frame.
        // Using caching new scene, until new frame will begin.
        Scene* m_currentScene;
        Scene* m_cachedScene;

        bool m_working;
    };
}

