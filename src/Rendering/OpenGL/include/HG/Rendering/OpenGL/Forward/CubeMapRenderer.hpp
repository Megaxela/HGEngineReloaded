#pragma once

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp> // Required for inheritance

namespace HG::Rendering::Base
{
class Material;
}

namespace HG::Rendering::OpenGL::Forward
{
/**
 * @brief Class, that describes forward cubemap
 * renderer.
 */
class CubeMapRenderer
    : public HG::Rendering::OpenGL::Forward::AbstractRenderer
    , private HG::Rendering::OpenGL::Common::MaterialProcessor
{
public:
    /**
     * @brief Constructor.
     */
    CubeMapRenderer();

    /**
     * @brief Method that performs actual cubemap rendering.
     * @param renderBehaviour Pointer to render behaviour.
     */
    void render(HG::Rendering::Base::RenderBehaviour* renderBehaviour) override;

    /**
     * @brief Method for getting render behaviours type,
     * that this renderer can proceeed.
     * (HG::Rendering::Base::Behaviours::CubeMap)
     * @return
     */
    std::size_t getTarget() override;

protected:
    /**
     * @brief Method for initializing
     * cubemap renderer.
     */
    void onInit() override;

    /**
     * @brief Method for deinitialization
     * cubemap renderer.
     */
    void onDeinit() override;

private:
    gl::vertex_array m_vao;
    gl::buffer m_vbo;

    HG::Rendering::Base::Material* m_skyboxMaterial;
};

} // namespace HG::Rendering::OpenGL::Forward
