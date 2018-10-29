#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

class TextureMaterial : public HG::Rendering::Base::Material
{
public:
    static const char* rawShader;

    void setTexture(HG::Rendering::Base::Texture* texture);
};

