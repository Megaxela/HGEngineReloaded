#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp> // Required for inheritance

namespace HG::Rendering::OpenGL::Materials
{
class SkyboxMaterial : public HG::Rendering::Base::Material
{
public:
    static const char* rawShader;
};
} // namespace HG::Rendering::OpenGL::Materials
