#pragma once

#include <RenderBehaviour.hpp>
#include <type_traits>
#include <memory>
#include <CurrentLogger.hpp>

namespace UTILS_MODULE_NS
{
    class Mesh;
    using MeshPtr = std::shared_ptr<Mesh>;
}

namespace RENDERING_BASE_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes mesh rendering.
     */
    class Mesh : public RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t Id = __COUNTER__;

        /**
         * @brief Class, that describes abstract external data
         * for mesh rendering, that can be used by pipeline to
         * attach some specific information to mesh.
         */
        class MeshExternalData
        {
        public:
            /**
             * @brief Default virtual destructor.
             */
            virtual ~MeshExternalData() = default;
        };

        /**
         * @brief Constructor.
         */
        Mesh();

        /**
         * @brief Destructor.
         */
        ~Mesh() override;

        /**
         * @brief Method for getting external data, casted
         * to some type.
         * @tparam T Type to cast.
         * @return Pointer to external data.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<MeshExternalData, T>::value,
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
            std::is_base_of<MeshExternalData, T>::value
        >::type setExternalData()
        {

#ifndef NDEBUG
            if (m_externalData)
            {
                Info() << "Recreating existing external data for mesh.";
            }
#endif

            delete m_externalData;

            m_externalData = new T();
        };

        /**
         * @brief Method for clearing external data.
         */
        void clearExternalData();

        /**
         * @brief Method for getting smart pointer
         * to mesh object.
         * @return Smart pointer to mesh or nullptr if
         * no mesh set.
         */
        UTILS_MODULE_NS::MeshPtr mesh() const;

        /**
         * @brief Method for setting mesh object.
         * @param mesh Smart pointer with mesh object.
         */
        void setMesh(UTILS_MODULE_NS::MeshPtr mesh);

    private:
        MeshExternalData* m_externalData;

        UTILS_MODULE_NS::MeshPtr m_mesh;
    };
}

