#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
class MeshFallbackMaterial : public HG::Rendering::Base::Material
{
public:
    static const char* rawShader;
};
} // namespace HG::Rendering::OpenGL::Materials
