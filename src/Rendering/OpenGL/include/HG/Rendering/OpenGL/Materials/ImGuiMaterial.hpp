#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
/**
 * @brief Material for rendering ImGui.
 */
class ImGuiMaterial : public HG::Rendering::Base::Material
{
public:
    static const char* rawShader;
};
} // namespace HG::Rendering::OpenGL::Materials
