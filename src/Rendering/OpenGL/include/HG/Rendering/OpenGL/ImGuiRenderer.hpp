#pragma once

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp>

// gl
#include <gl/all.hpp>

// Forward declaration
namespace HG::Rendering::Base
{
class Material;
class Texture;
} // namespace HG::Rendering::Base

namespace HG::Rendering::OpenGL
{
/**
 * @brief Class, that describes object for
 * ImGui rendering.
 */
class ImGuiRenderer
    : public HG::Utils::Interfaces::Initializable
    , public HG::Rendering::OpenGL::Common::MaterialProcessor

{
public:
    /**
     * @brief Constructor.
     */
    explicit ImGuiRenderer(HG::Core::Application* application);

    /**
     * @brief Method for rendering prepared data.
     */
    void render();

    /**
     * @brief Method for getting parent application.
     * @return Pointer to parent application.
     */
    HG::Core::Application* application() const;

protected:
    /**
     * @brief Method for initialization ImGui renderer.
     */
    void onInit() override;

    /**
     * @brief Method for deinitiazliation ImGui renderer.
     */
    void onDeinit() override;

private:
    void createFontsTexture();

    HG::Core::Application* m_application;
    HG::Rendering::Base::Material* m_material;

    gl::buffer m_vbo;
    gl::buffer m_ebo;

    HG::Rendering::Base::Texture* m_fontTexture;

    GLuint m_attribLocationPosition = 0;
    GLuint m_attribLocationUV       = 0;
    GLuint m_attribLocationColor    = 0;
};
} // namespace HG::Rendering::OpenGL
