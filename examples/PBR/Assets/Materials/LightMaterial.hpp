#pragma once

#include <HG/Rendering/Base/Material.hpp>

namespace HG::Rendering::Base
{
    class Texture;
}

class LightMaterial : public HG::Rendering::Base::Material
{
public:

    static const char* rawShader;

    void setDiffuse(HG::Rendering::Base::Texture* texture);
};

