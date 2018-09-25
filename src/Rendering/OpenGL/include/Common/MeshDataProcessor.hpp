#pragma once

// C++ STL
#include <map>
#include <memory>

// HG::Rendering::Base
#include <AbstractRenderDataProcessor.hpp> // Required for inheritance

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

        bool needSetup(HG::Rendering::Base::RenderData* data) override;

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

