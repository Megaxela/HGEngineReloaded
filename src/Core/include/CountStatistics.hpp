#pragma once

// C++ STL
#include <map>
#include <memory>

namespace HG::Core
{
    /**
     * @brief Class, that describes application
     * counting statistics, that can be appended with
     * additional counters. By default it calculates
     * number of gameobjects.
     *
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

        using ValueType = uint64_t;

        /**
         * @brief Constructor.
         */
        CountStatistics();

        /**
         * @brief Method for adding new counter.
         * Can throw exception `std::invalid_argument` if counter
         * with this id is already presented in debug mode.
         * @param counter Counter id.
         * @param type New counter type.
         */
        void addCounter(int counter, CounterType type);

        /**
         * @brief Method for removing counter.
         * Can throw exception `std::invalid_argument` if there
         * was no such counter in debug mode.
         * @param counter Counter id.
         */
        void removeCounter(int counter);

        /**
         * @brief Method for checking is some counter with
         * specified ID presented.
         * @param counter Counter id.
         */
        bool hasCounter(int counter) const;

        /**
         * @brief Method for getting counter value.
         * Can throw `std::invalid_argument` exception if there is no
         * counter with specified id in debug mode.
         * @param counter Counter id.
         */
        ValueType value(int counter) const;

        /**
         * @brief Method for resetting counter.
         * Can throw `std::invalid_argument` exception if there is no
         * counter with specified id. in debug mode
         * @param counter Counter id.
         */
        void reset(int counter);

        /**
         * @brief Method that notifies all counters
         * that notifies all counters, that frame changed.
         */
        void frameChanged();

    private:

        class AbstractCounter;

        using CounterPtr = std::unique_ptr<AbstractCounter>;

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
             * @brief Virtual destructor.
             */
            virtual ~AbstractCounter() = default;

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
             * @brief Method, that performs counter
             * resetting.
             */
            virtual void reset() = 0;

            /**
             * @brief Method for adding value to counter.
             * @param value Value to add.
             */
            virtual void add(ValueType value) = 0;

            /**
             * @brief Method for getting counter value.
             * @return Current counter value.
             */
            virtual ValueType value() const = 0;

        private:

            CounterType m_type;
        };

        /**
         * @brief Class, that implements cummulative counter
         * object.
         */
        class CummulativeCounter : public AbstractCounter
        {
        public:

            /**
             * @brief Constructor.
             */
            CummulativeCounter();

            /**
             * @brief Method, that clears counter.
             */
            void reset() override;

            /**
             * @brief Method, that just adds value to
             * counter.
             * @param value Value.
             */
            void add(ValueType value) override;

            /**
             * @brief Method, that returns counter value.
             */
            ValueType value() const override;

        private:

            ValueType m_value;
        };

        /**
         * @brief Class, that implements last frame counter
         * object.
         */
        class LastFrameCounter : public AbstractCounter
        {
        public:

            /**
             * @brief Constructor.
             */
            LastFrameCounter();

            /**
             * @brief Method, that performs rotating
             * counter value on frame changed.
             */
            void frameChanged() override;

            /**
             * @brief Method, that performs counter
             * value reset.
             */
            void reset() override;

            /**
             * @brief Method for adding value to current frame
             * counter.
             */
            void add(ValueType value) override;

            /**
             * @brief Method for getting last frame
             * value.
             */
            ValueType value() const override;

        private:

            ValueType m_previousFrameValue;
            ValueType m_currentFrameValue;
        };

        std::map<int, AbstractCounter*> m_counters;
    };
}

