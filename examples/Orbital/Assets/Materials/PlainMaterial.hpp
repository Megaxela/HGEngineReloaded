#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

class PlainMaterial : public HG::Rendering::Base::Material
{
public:

    static const char* rawShader;

    void setColor(const HG::Utils::Color& color);
};


