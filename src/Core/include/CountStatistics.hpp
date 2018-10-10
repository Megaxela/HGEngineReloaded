#pragma once

// C++ STL
#include <map>

namespace HG::Core
{
    /**
     * @brief Class, that describes application
     * counting statistics, that can be appended with
     * additional counters. By default it calculates
     * number of gameobjects.
     * There is several types of counters.
     * 1. Cumulative - it's simple accumulator that
     *                 can be zeroed at any time by user.
     *
     * 2. LastFrame - it's counter that returns last frame
     *                value of this counter (because current frame
     *                values just collects)
     *
     */
    class CountStatistics
    {
    public:

        enum class CounterType
        {
            Cumulative,
            LastFrame
        };



    private:

        /**
         * @brief Class, that describes abstract
         * counter.
         */
        class AbstractCounter
        {
        public:

            /**
             * @brief Constructor.
             */
            explicit AbstractCounter(CounterType type);

            /**
             * @brief Method for getting counter type.
             */
            CounterType type() const;

            /**
             * @brief Method, that's called by CountStatistics
             * object, when frame is changed.
             */
            virtual void frameChanged();

            /**
             * @brief Method for adding value to counter.
             * @param value Value to add.
             */
            virtual void add(std::size_t value);

            /**
             * @brief Method for getting counter value.
             * @return Current counter value.
             */
            virtual uint64_t value() const;

        private:

            uint64_t m_value;

            CounterType m_type;
        };

        std::map<int, Counter> m_counters;
    };
}

