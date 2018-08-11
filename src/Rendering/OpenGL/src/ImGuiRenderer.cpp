#include "ImGuiRenderer.hpp"
#include <Application.hpp>
#include <Materials/ImGuiMaterial.hpp>
#include <imgui.h>
#include <Common/ShaderData.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/auxiliary/glm_uniforms.hpp>

HG::Rendering::OpenGL::ImGuiRenderer::ImGuiRenderer(HG::Core::Application* application) :
    m_application(application),
    m_material(nullptr),
    m_vbo(gl::invalid_id),
    m_ebo(gl::invalid_id),
    m_fontTexture(gl::invalid_id)
{

}

HG::Rendering::OpenGL::ImGuiRenderer::~ImGuiRenderer()
{
    deinit();
}

void HG::Rendering::OpenGL::ImGuiRenderer::deinit()
{
    Info() << "Deinitializing ImGui renderer";

    delete m_material;
    m_material = nullptr;

    // todo: Fix this shitcode, when gl developer will fix move operator.
    {
        gl::buffer del(gl::invalid_id);

        std::swap(m_vbo, del);
    }

    {
        gl::buffer del(gl::invalid_id);

        std::swap(m_ebo, del);
    }

    {
        gl::texture_2d del(gl::invalid_id);

        std::swap(m_fontTexture, del);
    }
}

void HG::Rendering::OpenGL::ImGuiRenderer::init()
{
    Info() << "Initializing ImGui renderer";

    m_material = m_application
        ->renderer()
        ->materialCollection()
        ->getMaterial<Materials::ImGuiMaterial>();

    m_vbo            = std::move(gl::buffer());
    m_ebo = std::move(gl::buffer());

    auto* program = &m_material->shader()->externalData<Common::ShaderData>()->Program;

    m_uniformLocationTex      = program->uniform_location("Texture");
    m_uniformLocationProjMtx  = program->uniform_location("ProjMtx");
    m_attribLocationPosition  = program->attribute_location("Position");
    m_attribLocationUV        = program->attribute_location("UV");
    m_attribLocationColor     = program->attribute_location("Color");

    createFontsTexture();
}

void HG::Rendering::OpenGL::ImGuiRenderer::render()
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    auto& io = ImGui::GetIO();

    auto fb_width  = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    auto fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);

    if (fb_width == 0 || fb_height == 0)
    {
        return;
    }

    auto* draw_data = ImGui::GetDrawData();
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLint last_scissor_box[4];   glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;   glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;   glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);

    bool last_enable_blend = gl::blending_enabled();
    bool last_enable_cull_face = gl::polygon_face_culling_enabled();
    bool last_enable_depth_test = gl::depth_test_enabled();
    bool last_enable_scissor_test = gl::scissor_test_enabled();

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    gl::set_blending_enabled(true);
    gl::set_polygon_face_culling_enabled(false);
    gl::set_depth_test_enabled(false);
    gl::set_scissor_test_enabled(true);
    gl::set_polygon_mode(GL_FILL);
    gl::set_blend_equation(GL_FUNC_ADD, GL_FUNC_ADD);
    gl::set_blend_function(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup viewport, orthographic projection matrix
    gl::set_viewport({0, 0}, {fb_width, fb_height});

    // Generating ortho projection
    const auto ortho_projection = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f);

    // Getting shader program
    auto* program = &m_material->shader()->externalData<Common::ShaderData>()->Program;

    program->use();
    program->set_uniform(m_uniformLocationTex, 0);
    program->set_uniform(m_uniformLocationProjMtx, ortho_projection);

    // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
    gl::sampler::unbind(0);

    // Recreate the VAO every time
    // (This is to easily allow multiple GL contexts.
    // VAO are not shared among GL contexts, and we don't
    // track creation/deletion of windows so we don't have
    // an obvious key to use to cache them.)
    gl::vertex_array vao;

    vao.bind();
    m_vbo.bind(GL_ARRAY_BUFFER);

    // Setting up VAO attributes
    vao.set_attribute_enabled(m_attribLocationPosition, true);
    vao.set_attribute_enabled(m_attribLocationUV,       true);
    vao.set_attribute_enabled(m_attribLocationColor,    true);

    vao.set_vertex_buffer(m_attribLocationPosition, m_vbo, 0, sizeof(ImDrawVert));
    vao.set_vertex_buffer(m_attribLocationUV,       m_vbo, 0, sizeof(ImDrawVert));
    vao.set_vertex_buffer(m_attribLocationColor,    m_vbo, 0, sizeof(ImDrawVert));

    vao.set_attribute_format(m_attribLocationPosition, 2, GL_FLOAT,         false, static_cast<GLuint>(IM_OFFSETOF(ImDrawVert, pos)));
    vao.set_attribute_format(m_attribLocationUV,       2, GL_FLOAT,         false, static_cast<GLuint>(IM_OFFSETOF(ImDrawVert, uv)));
    vao.set_attribute_format(m_attribLocationColor,    4, GL_UNSIGNED_BYTE, true,  static_cast<GLuint>(IM_OFFSETOF(ImDrawVert, col)));

    // Draw
    for (auto commandListIndex = 0;
         commandListIndex < draw_data->CmdListsCount;
         commandListIndex++)
    {
        const auto* cmd_list = draw_data->CmdLists[commandListIndex];
        const ImDrawIdx* idx_buffer_offset = nullptr;

        m_vbo.bind(GL_ARRAY_BUFFER);

        // Loading data to VBO
        m_vbo.set_data(
            cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
            cmd_list->VtxBuffer.Data,
            GL_STREAM_DRAW
        );

        // Binding element buffer object
        m_ebo.bind(GL_ELEMENT_ARRAY_BUFFER);

        // Loading data to EBO
        m_ebo.set_data(
            cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
            cmd_list->IdxBuffer.Data,
            GL_STREAM_DRAW
        );

        // Drawing commands
        for (auto commandIndex = 0;
             commandIndex < cmd_list->CmdBuffer.Size;
             ++commandIndex)
        {
            const auto* pcmd = &cmd_list->CmdBuffer[commandIndex];

            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(
                    GL_TEXTURE_2D,
                    (GLuint)(intptr_t)pcmd->TextureId
                );

                gl::set_scissor(
                    {
                        static_cast<GLint>(pcmd->ClipRect.x),
                        static_cast<GLint>(fb_height - pcmd->ClipRect.w)
                    },
                    {
                        static_cast<GLsizei>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                        static_cast<GLsizei>(pcmd->ClipRect.w - pcmd->ClipRect.y)
                    }
                );

                gl::draw_elements(
                    GL_TRIANGLES,
                    pcmd->ElemCount,
                    GL_UNSIGNED_SHORT, // May cause errors after imgui update, because of propaply uint
                    idx_buffer_offset
                );
            }

            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    gl::set_blend_function(last_blend_src_rgb, last_blend_src_alpha, last_blend_dst_rgb, last_blend_dst_alpha);
    gl::set_blending_enabled(last_enable_blend);
    gl::set_polygon_face_culling_enabled(last_enable_cull_face);
    gl::set_depth_test_enabled(last_enable_depth_test);
    gl::set_scissor_test_enabled(last_enable_scissor_test);
    gl::set_scissor({last_scissor_box[0], last_scissor_box[1]}, {last_scissor_box[2], last_scissor_box[3]});
}

void HG::Rendering::OpenGL::ImGuiRenderer::createFontsTexture()
{
    m_fontTexture = std::move(gl::texture_2d());

    auto& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    m_fontTexture.bind();
    m_fontTexture.set_min_filter(GL_LINEAR);
    m_fontTexture.set_mag_filter(GL_LINEAR);

    m_fontTexture.set_storage(
        1,        // Levels
        GL_RGBA8, // Internal format,
        width,    // Width
        height    // Height
    );

    m_fontTexture.set_sub_image(
        0,        // Level
        0,        // X offset
        0,        // Y offset
        width,    // Width
        height,   // Height
        GL_RGBA, // External format
        GL_UNSIGNED_BYTE, // Type
        pixels
    );

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t) m_fontTexture.id();
}
