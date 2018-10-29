#pragma once

// C++ STL
#include <type_traits>
#include <functional>
#include <queue>
#include <thread>
#include <shared_mutex>

// HG::Utils
#include <HG/Utils/FutureHandler.hpp>

namespace HG::Core
{
    /**
     * @brief Class, that describes engine thread pool.
     * By default it starts 1 file loading thread,
     * and (N - 2) user threads, for user tasks. Where N
     * is number of system cores. (at least 1 user thread)
     */
    class ThreadPool
    {
    public:

        enum Type
        {
            FileLoadingThread,
            UserThread
        };

        /**
         * @brief Constructor.
         */
        ThreadPool();

        /**
         * @brief Destructor.
         */
        ~ThreadPool();

        /**
         * @brief Method for pushing task into
         * thread pool. If this function returns nonvoid
         * value - it will return HG::Utils::FutureHandler
         * object.
         * @tparam FunctionType Function type.
         * @param function Function.
         * @param type Thread type.
         * @return If function return type is non void -
         * HG::FutureHandler< function_result_type > will be
         * returned. Otherwise void will be returned.
         */
        template<typename FunctionType>
        typename std::conditional<
            std::is_void<
                typename std::invoke_result<FunctionType>::type
            >::value,

            void,

            typename HG::Utils::FutureHandler<
                typename std::invoke_result<FunctionType>::type
            >
        >::type push(FunctionType function, Type type = Type::UserThread)
        {
            // todo: Fix promise to lambda capture lately
            using ResultType = typename std::invoke_result<FunctionType>::type;
            using FutureResultType = typename HG::Utils::FutureHandler<ResultType>;
            constexpr bool IsResultVoid = std::is_void<ResultType>::value;

            if constexpr (IsResultVoid)
            {
                pushInternal(function, type);
            }
            else
            {
                // Making promise and pushing job
                auto promise = std::make_shared<
                    std::promise<ResultType>
                >();

                auto future = std::shared_future(promise->get_future());

                // Creating custom void() job
                auto job = [prom = std::move(promise), function]()
                {
                    prom->set_value(function());
                };

                // Pushing to queue
                pushInternal(job, type);

                return FutureResultType(std::move(future));
            }
        }

        /**
         * @brief Method for starting pool with
         * defined amount of threads.
         * @param type Pool type.
         * @param numberOfThreads Number of required threads.
         */
        void startPool(Type type, std::size_t numberOfThreads);

        /**
         * @brief Method for stopping pool.
         * @param type Pool type.
         */
        void stopPool(Type type);

        /**
         * @brief Method for waiting until pool
         * threads will stop.
         * @param type Pool type.
         */
        void joinPool(Type type);

        /**
         * @brief Method for getting number of pool
         * jobs.
         * @param type Pool type.
         * @return Number of jobs. If there is no such
         * pool - 0.
         */
        std::size_t numberOfJobs(Type type) const;

    private:

        struct PoolData
        {
            // Jobs for pool
            std::queue<std::function<void()>> jobs;
            std::mutex jobsMutex;

            // Is pool running
            std::atomic_bool running = false;

            // ID for next created thread
            std::atomic_size_t nextId = 0;

            // Notifier for jobs
            std::condition_variable jobsNotifier;

            // Actual pool threads
            std::mutex threadsMutex;
            std::unordered_map<std::size_t, std::thread> threads;
        };

        /**
         * @brief Internal method for pushing job to
         * some pool.
         * @param job Job.
         * @param type Pool type.
         */
        void pushInternal(std::function<void()> job, Type type);

        /**
         * @brief Actual thread function.
         */
        void threadFunction(std::size_t id, Type type);

        /**
         * @brief Method for getting pool data.
         * @param type Type.
         * @return Smart pointer to pool data or nullptr.
         */
        std::shared_ptr<PoolData> getPoolData(Type type) const;

        std::unordered_map<
            Type,
            std::shared_ptr<PoolData>
        > m_data;

        mutable std::shared_mutex m_dataMutex;
    };
}

