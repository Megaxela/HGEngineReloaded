#pragma once

#include <memory>
#include <future>

namespace HG::Utils
{
    /**
     * @brief Class, that overlaps stl's future
     * object for more easily access to future data.
     * @tparam ResultType Result type.
     */
    template<typename ResultType>
    class FutureHandler
    {
    public:

        using Ptr = std::shared_ptr< FutureHandler<ResultType> >;

        /**
         * @brief Initializer constructor.
         * @param future Future.
         */
        FutureHandler(std::future<ResultType> future) :
            m_future(std::move(future)),
            m_actualResult(),
            m_received(false)
        {}

        FutureHandler(FutureHandler&& rhs) noexcept :
            m_future(),
            m_actualResult(),
            m_received()
        {
            // todo: lock rhs handler mutex here
            m_future = std::move(rhs.m_future);
            m_actualResult = std::move(rhs.m_actualResult);
            m_received = rhs.m_received;
        }

        /**
         * @brief Fallback constructor. If this
         * constructor was used - handler will only
         * pass value back.
         * @param result Result.
         */
        FutureHandler(ResultType result) :
            m_future(),
            m_actualResult(std::move(result)),
            m_received(true)
        {}

        /**
         * @brief Method for getting result type.
         * @return Result type.
         */
        ResultType get()
        {
            if (m_received)
            {
                return m_actualResult;
            }

            if (m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                m_received = true;

                m_actualResult = std::move(m_future.get());

                return m_actualResult;
            }

            return ResultType();
        }

        ResultType guaranteeGet()
        {
            m_actualResult = std::move(m_future.get());
            m_received = true;

            return m_actualResult;
        }

        /**
         * @brief Method for casting future to result type.
         */
        operator ResultType()
        {
            return get();
        }

    private:

        std::future<ResultType> m_future;

        ResultType m_actualResult;

        bool m_received;

    };
}

