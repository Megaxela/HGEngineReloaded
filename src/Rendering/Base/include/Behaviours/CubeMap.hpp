#pragma once

// HG::Rendering::Base
#include <RenderBehaviour.hpp> // Required for inheritance

// HG::Utils
#include <StringTools.hpp>

namespace HG::Rendering::Base
{
    class CubeMap;
}

namespace HG::Rendering::Base::Behaviours
{
    /**
     * @brief Class, that describes cubemap rendering
     * behaviour.
     */
    class CubeMap : public HG::Rendering::Base::RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t RenderBehaviourId = HG::Utils::StringTools::hash("RenderBehaviour::CubeMap");

        /**
         * @brief Constructor.
         */
        explicit CubeMap(HG::Rendering::Base::CubeMap* cubeMap=nullptr);

        /**
         * @brief Destructor.
         */
        ~CubeMap() override;

        /**
         * @brief Method for setting cube map.
         * @param cubeMap Pointer to cube map.
         */
        void setCubeMap(HG::Rendering::Base::CubeMap* cubeMap);

        /**
         * @brief Method for getting pointer to cubemap object.
         * @return Pointer to cubemap.
         */
        HG::Rendering::Base::CubeMap* cubeMap() const;

    private:

        HG::Rendering::Base::CubeMap* m_cubeMap;
    };
}


