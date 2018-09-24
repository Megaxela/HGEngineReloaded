#pragma once

// HG::Rendering::Base
#include <Material.hpp>

// HG::Utils
#include <Color.hpp>

class PlainMaterial : public HG::Rendering::Base::Material
{
public:

    static const char* rawShader;

    void setColor(const HG::Utils::Color& color);
};


