#pragma once

// C++ STL
#include <chrono>
#include <vector>
#include <map>

namespace HG::Core
{
    /**
     * @brief Class, that describes application
     * timing statistics, that can be appended with
     * additional timings. By default it calculates
     * CPU update time and rendering time.
     */
    class TimeStatistics
    {
    public:

        enum Timers
        {
            FrameTime = 0,
            FirstSystemTimer = FrameTime,
            RenderTime = 1,
            UpdateTime = 2,
            PhysicsTime = 3,
            LastSystemTimer
        };

        /**
         * @brief Constructor.
         */
        TimeStatistics();

        /**
         * @brief Method for getting estimate frame
         * delta time for several frames in microseconds.
         * Number of frames for estimation can be changed by
         * method `changeEstimateBuffer` with `FrameTime` timer.
         * @return Frame delta time in microseconds.
         */
        std::chrono::microseconds frameDeltaTime();

        /**
         * @brief Method for getting last frame render time.
         * @return Last frame time in microseconds.
         */
        std::chrono::microseconds lastFrameDeltaTime();

        /**
         * @brief Method for getting estimage physics time
         * for several last frames. Number of frames for
         * estimation can be changed by method
         * `changeEstimateBuffer` with `PhysicsTime` timer.
         * @return Estimate physics time in microseconds.
         */
        std::chrono::microseconds physicsTime();

        /**
         * @brief Method for getting estimate render time
         * for several last frames. Number of frames for
         * estimation can be changed by method
         * `changeEstimateBuffer` with `RenderTime` timer.
         * @return Estimate render time in microseconds.
         */
        std::chrono::microseconds renderTime();

        /**
         * @brief Method for getting last frame render time.
         * @return Last frame render time.
         */
        std::chrono::microseconds lastFrameRenderTime();

        /**
         * @brief Method for getting estimate update (logic) time
         * for several last frames. Number of frames for
         * estimation can be changed by method
         * `changeEstimateBuffer` with `UpdateTime` timer.
         * @return Estimate update (logic) time in microseconds.
         */
        std::chrono::microseconds updateTime();

        /**
         * @brief Method for getting last frame update (logic) time
         * in microseconds.
         * @return Last frame update (logic) time in microseconds.
         */
        std::chrono::microseconds lastFrameUpdateTime();

        /**
         * @brief Method for changing estimate buffer size.
         * If number of frames will be lower then current,
         * older information will be trimmed. If number of
         * frames will be higher than current most older
         * frame info will be copied and append buffer from the
         * end.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer ID.
         * @param numberOfFrames Number of frames to estimate.
         */
        void changeEstimateBuffer(int timer, std::size_t numberOfFrames);

        /**
         * @brief Method for adding timing information for
         * specified timer.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer id.
         * @param microseconds Timer info in microseconds.
         */
        void tickTimer(int timer, std::chrono::microseconds microseconds);

        /**
         * @brief Supportive method for time calculating.
         * Method begins counting of specific timer.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer id.
         */
        void tickTimerBegin(int timer);

        /**
         * @brief Supportive method for time calculating.
         * It stops timer (if it was run), gets time value,
         * and starts timer again.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer id.
         * @return Counted time in microseconds.
         */
        std::chrono::microseconds tickTimerAtomic(int timer);

        /**
         * @brief Supportive method for time calculating.
         * Method ends counting of specific timer and adds
         * it to timing information for specified timer.
         * Can throw exception `std::runtime_error` if
         * timer stopped without call of `tickTimerBegin`.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer id.
         * @return Counted time in microseconds.
         */
        std::chrono::microseconds tickTimerEnd(int timer);

        /**
         * @brief Method for getting timer estimated
         * value.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @param timer Timer id.
         * @return Time in microseconds.
         */
        std::chrono::microseconds getTimerEstimate(int timer) const;

        /**
         * @brief Method for getting timer last frame value.
         * @param timer Timer id.
         * Can throw `std::invalid_argument` exception if there is no
         * timer with specified id.
         * @return Time in microseconds.
         */
        std::chrono::microseconds getTimerLastFrame(int timer) const;

        /**
         * @brief Method for adding new timer.
         * Can throw exception `std::invalid_argument` if timer
         * with this id is already presented.
         * @param timer Timer id.
         */
        void addTimer(int timer);

        /**
         * @brief Method for removing timer.
         * Can throw exception `std::invalid_argument` if there
         * was no such timer.
         * @param timer Timer id.
         */
        void removeTimer(int timer);

        /**
         * @brief Method for checking is some timer with
         * specified ID presented.
         * @param timer Timer id.
         */
        bool hasTimer(int timer) const;

    private:

        /**
         * @brief Actual timer class.
         * Buffer implemented as simplest
         * ring buffer.
         */
        class Timer
        {
        public:
            /**
             * @brief Constructor.
             */
            Timer();

            /**
             * @brief Method for adding timing
             * information.
             * @param mcs Timer in microseconds.
             */
            void tick(std::chrono::microseconds mcs);

            /**
             * @brief Supportive method for starting
             * timer.
             */
            void tickBegin();

            /**
             * @brief Supportive method for finishing
             * timer and adding it as timing information.
             * Can throw exception `std::runtime_error` if
             * timer stopped without call of `tickBegin`.
             * @return Counted time.
             */
            std::chrono::microseconds tickEnd();

            /**
             * @brief Method for getting estimated time.
             * @return Estimated time in microseconds.
             */
            std::chrono::microseconds estimateTime() const;

            /**
             * @brief Method for getting last frame.
             * @return Last frame time in microseconds.
             */
            std::chrono::microseconds lastFrameTime() const;

            /**
             * @brief Method for changing estimate buffer size.
             * If number of frames will be lower then current,
             * older information will be trimmed. If number of
             * frames will be higher than current most older
             * frame info will be copied and append buffer from the
             * end.
             * @param count Number of frames to estimate.
             * @todo Documentation behaviour is not implemented yet.
             */
            void changeEstimateBuffer(std::size_t count);

            /**
             * @brief Method for getting timer started state.
             * @return Was timer started.
             */
            bool isTimerStarted() const;

        private:

            // Buffer is always 1 more byte than
            std::vector<std::chrono::microseconds> m_buffer;
            std::size_t m_insertPosition;
            std::size_t m_bufferSize;
            bool m_timerStarted;
            std::chrono::steady_clock::time_point m_timerStart;
        };

        // Timers container
        std::map<int, Timer> m_timers;

    };
}

