#pragma once

// C++ STL
#include <string>
#include <unordered_map>
#include <stack>
#include <chrono>
#include <shared_mutex>
#include <thread>
#include <vector>

#define BENCH(NAME) \
BENCH_D(application(), NAME)

#define BENCH_D(APP, NAME) \
HG::Core::Benchmark::ScopeJob __bench ## __COUNTER__ (APP, NAME)

#define BENCH_I(NAME) \
HG::Core::Benchmark::ScopeJob(application(), NAME)

namespace HG::Core
{
    class Application;

    /**
     * @brief Class, that performs frame timing
     * register.
     */
    class Benchmark
    {
    public:

        using TimeType = std::chrono::steady_clock::time_point;

        /**
         * @brief Class, that describes job info.
         */
        struct JobContainer
        {
            /**
             * @brief Constructor.
             * @param name Job name.
             * @param start Job start time.
             */
            JobContainer(std::string name,
                         TimeType start) :
                    subjobs(0),
                    name(std::move(name)),
                    startTime(start),
                    finishTime()
            {

            }

            std::size_t subjobs;
            std::string name;

            TimeType startTime;
            TimeType finishTime;
        };

        class ScopeJob
        {
        public:

            /**
             * @brief Constructor.
             * @param application Pointer to application.
             * Pointer to becnhmark object will be taken from it.
             * @param name Name.
             */
            ScopeJob(HG::Core::Application* application, std::string name);

            /**
             * @brief Destructor.
             */
            ~ScopeJob();

        private:
            HG::Core::Benchmark* m_benchmark;
        };

        Benchmark();

        /**
         * @brief Method, that performs pusing
         * job in caller thread.
         * @param name Job name.
         */
        void startJob(std::string name);

        /**
         * @brief Method, that counts
         * execution time and pops top job.
         * If there is no job to stop
         * `std::runtime_error` exception
         * will be thrown.
         */
        void stopJob();

        /**
         * @brief Method for getting caller
         * thread stack size.
         * @return Jobs stack size.
         */
        std::size_t numberOfActiveJobs();

        /**
         * @brief Method for getting thread ids
         * that's registered in benchmark.
         * @param ids Result vector.
         */
        void getThreadsId(std::vector<std::thread::id>& ids);

        /**
         * @brief Method for getting closed jobs data for
         * specified thread.
         * std::invalid_argument will be thrown if there is no
         * such thread. You have to check for exception, because
         * if some thread will call for `clear` method - thread will
         * be removed from container and exception will be thrown.
         * @param id Thread identifier.
         * @return Copy of stack.
         */
        // todo: Optimize with atomic pointer switching
        std::stack<JobContainer> getClosedJobs(std::thread::id id);

        /**
         * @brief Method for getting container with frame times.
         * @return Time of frames starts.
         */
        std::vector<TimeType> frameTimes() const;

        /**
         * @brief Method for getting benchmark start
         * time.
         */
        TimeType startTime() const;

        /**
         * @brief Method for getting benchmark finish
         * time.
         */
        TimeType finishTime() const;

        /**
         * @brief Method for clearing current benchmarking
         * state. It will clear saved jobs and threads.
         */
        void clear();

        /**
         * @brief Method, that's called every frame start.
         * It makes mark in benchmark, with time, when
         * frame was started/finished.
         */
        void tick();

        /**
         * @brief Method, that performs begins benchmarking recording.
         */
        void start();

        /**
         * @brief Method, that performs finish benchmarking recording.
         */
        void finish();

        /**
         * @brief Method for getting is benchmark running.
         * @return Is running.
         */
        bool isRunning() const;

    private:

        struct ThreadContainer
        {
            std::stack<JobContainer> countingJobs;
            std::stack<JobContainer> doneJobs;
        };

        std::shared_mutex m_timingsMutex;
        std::unordered_map<
            std::thread::id,
            ThreadContainer
        > m_timings;

        std::vector<TimeType> m_frameTimes;
        TimeType m_start;
        TimeType m_finish;
        bool m_wantRunning;
        bool m_isRunning;
    };
}
