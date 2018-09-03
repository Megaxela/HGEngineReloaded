#pragma once

#include "AbstractRenderDataProcessor.hpp"
#include <map>
#include <memory>

namespace HG::Utils
{
    class Mesh;
}

namespace HG::Rendering::OpenGL::Common
{
    class MeshData;

    /**
     * @brief Class, that describes mesh data processor.
     */
    class MeshDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:

        bool setup(HG::Rendering::Base::RenderData* data) override;

        std::size_t getTarget() override;

    private:
        std::map<
            std::weak_ptr<HG::Utils::Mesh>,
            MeshData*,
            std::owner_less<
                std::weak_ptr<HG::Utils::Mesh>
            >
        > m_meshes;
    };
}

