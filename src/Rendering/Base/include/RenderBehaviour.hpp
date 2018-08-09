#pragma once

#include <cstdio>
#include <GameObject.hpp>
#include <CurrentLogger.hpp>

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes cross pipeline
     * behaviour for rendering.
     */
    class RenderBehaviour
    {
    public:


        /**
         * @brief Class, that describes abstract external data
         * for mesh rendering, that can be used by pipeline to
         * attach some specific information to mesh.
         */
        class ExternalData
        {
        public:
            /**
             * @brief Default virtual destructor.
             */
            virtual ~ExternalData() = default;
        };

        /**
         * @brief Constructor.
         * @param type Derived class `typeid(*this).hash_code()` value.
         */
        explicit RenderBehaviour(std::size_t type);

        /**
         * @brief Default virtual destructor.
         */
        virtual ~RenderBehaviour();

        /**
         * @brief Method for getting behaviour type.
         * @return Real class `typeid(T).hash_code()` value.
         */
        std::size_t behaviourType();

        /**
         * @brief Method for getting parent gameobject.
         * @return Parent gameobject or nullptr if
         * there is no parent.
         */
        ::HG::Core::GameObject* gameObject();

        /**
         * @brief Method for clearing external data.
         */
        void clearExternalData();

        /**
         * @brief Method for getting external data, casted
         * to some type.
         * @tparam T Type to cast.
         * @return Pointer to external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<ExternalData, T>::value,
            T*
        >::type externalData() const
        {
            return static_cast<T*>(m_externalData);
        }

        /**
         * @brief Method for setting external data.
         * @tparam T Type of actual external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<ExternalData, T>::value
        >::type setExternalData()
        {

#ifndef NDEBUG
            if (m_externalData)
            {
                Info() << "Recreating existing external data.";
            }
#endif

            delete m_externalData;

            m_externalData = new T();
        };

    protected:
        friend class ::HG::Core::GameObject;

        /**
         * @brief Method for gameobject to set parent gameobject.
         * @param gameObject
         */
        void setParentGameObject(::HG::Core::GameObject* gameObject);

    private:
        ExternalData* m_externalData;

        std::size_t m_type;

        ::HG::Core::GameObject* m_parent;
    };
}

