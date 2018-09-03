#pragma once

#include <RenderSpecificData.hpp>
#include <type_traits>

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes some data,
     * that can be initialized in rendering pipeline.
     */
    class RenderData
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit RenderData(std::size_t type);

        /**
         * @brief Virtual destructor.
         */
        virtual ~RenderData();

        /**
         * @brief Method for getting specific data
         * of some type.
         * @tparam T Type of expected specific data.
         * @return Pointer to specific data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<RenderSpecificData, T>::value,
            T*
        >::type specificData()
        {
            return dynamic_cast<T*>(m_data);
        }

        /**
         * @brief Method for setting specific data.
         * @tparam T Specific data type.
         * @return Created specific data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<RenderSpecificData, T>::value,
            T*
        >::type setSpecificData()
        {
            delete m_data;

            m_data = new T();

            return static_cast<T*>(m_data);
        }

        /**
         * @brief Method for assigning specific data.
         * @tparam T Specific data type.
         * @return Created specific data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<RenderSpecificData, T>::value,
            void
        >::type assignSpecificData(T* data)
        {
            m_data = data;
        }

        /**
         * @brief Method for clearing external data.
         */
        void clearSpecificData();

        /**
         * @brief Method for getting render data
         * type.
         */
        std::size_t dataType() const;

    private:

        RenderSpecificData* m_data;

        std::size_t m_type;

    };
}

