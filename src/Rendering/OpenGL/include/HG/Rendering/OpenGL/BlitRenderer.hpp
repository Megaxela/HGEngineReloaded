#pragma once

// gl
#include <gl/all.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/BlitData.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>

// HG::Utils
#include <HG/Utils/Interfaces/Initializable.hpp>

// Forward declaration
namespace HG::Rendering::Base
{
class Material;
class Texture;
class RenderTarget;
} // namespace HG::Rendering::Base

namespace HG::Rendering::OpenGL
{
/**
 * @brief Class, that describes object
 * for performing blitting.
 */
class BlitRenderer
    : public HG::Utils::Interfaces::Initializable
    , private HG::Rendering::OpenGL::Common::MaterialProcessor
{
public:
    /**
     * @brief Constructor.
     * @param application Pointer to parent application.
     */
    explicit BlitRenderer(HG::Core::Application* application);

    /**
     * @brief Method for getting parent application.
     * @return Pointer to parent application.
     */
    HG::Core::Application* application() const;

    /**
     * @brief Method, that performs rendering data
     * to target rendertarget.
     * @param target Pointer to render target.
     * @param container Pointer to container.
     */
    void render(HG::Rendering::Base::RenderTarget* target,
                HG::Rendering::Base::Texture* texture,
                const HG::Rendering::Base::BlitData::DataContainer& container);

protected:
    /**
     * @brief Method for initialization blitter.
     */
    void onInit() override;

    /**
     * @brief Method for deinitialization blitter.
     */
    void onDeinit() override;

private:
    HG::Core::Application* m_application;
    HG::Rendering::Base::Material* m_material;

    gl::buffer m_vbo;
    gl::buffer m_ebo;

    GLint m_uniformLocationTexture;
    GLint m_uniformLocationTextureSize;
    GLint m_uniformLocationProjection;

    GLuint m_attributeLocationVertices;
    GLuint m_attributeLocationUV;
};
} // namespace HG::Rendering::OpenGL
