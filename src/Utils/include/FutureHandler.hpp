#pragma once

// C++ STL
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

        /**
         * @brief Initializer constructor.
         * @param future Future.
         */
        FutureHandler(std::shared_future<ResultType> future) :
            m_predefinedValue(),
            m_future(std::move(future))
        {}

        FutureHandler(FutureHandler<ResultType>&& rhs) noexcept :
            m_predefinedValue(std::move(rhs.m_predefinedValue)),
            m_future(std::move(rhs.m_future))
        {}

        FutureHandler<ResultType>& operator=(FutureHandler<ResultType>&& rhs) noexcept
        {
            m_future = std::move(rhs.m_future);
            m_predefinedValue = std::move(rhs.m_predefinedValue);

            return (*this);
        }

        FutureHandler(const FutureHandler<ResultType>& rhs) :
            m_predefinedValue(rhs.m_predefinedValue),
            m_future(rhs.m_future)
        {}

        FutureHandler<ResultType>& operator=(const FutureHandler<ResultType>& rhs)
        {
            m_future = rhs.m_future;
            m_predefinedValue = rhs.m_predefinedValue;

            return (*this);
        }

        /**
         * @brief Fallback constructor. If this
         * constructor was used - handler will only
         * pass value back.
         * @param result Result.
         */
        FutureHandler(ResultType result) :
            m_predefinedValue(std::move(result)),
            m_future()
        {}

        /**
         * @brief Method for getting result type.
         * @return Result type.
         */
        ResultType get()
        {
            if (m_future.valid())
            {
                if (m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                    return m_future.get();
                }

                return ResultType();
            }
            else
            {
                return m_predefinedValue;
            }
        }

        ResultType guaranteeGet()
        {
            return m_future.get();
        }

        /**
         * @brief Method for casting future to result type.
         */
        operator ResultType()
        {
            return get();
        }

        ResultType operator->()
        {
            return get();
        }

    private:

        ResultType m_predefinedValue;
        std::shared_future<ResultType> m_future;

    };
}

