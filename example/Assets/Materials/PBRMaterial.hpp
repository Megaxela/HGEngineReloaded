#pragma once

#include <Material.hpp>
#include <Color.hpp>

namespace HG::Rendering::Base
{
    class Texture;
}

class PBRMaterial : public HG::Rendering::Base::Material
{
public:

    static const char* rawShader;

//    void setAlbedo(const HG::Utils::Color& color);

    void setAlbedoMap(HG::Rendering::Base::Texture* texture);

    void setNormalMap(HG::Rendering::Base::Texture* texture);

    void setMetallicMap(HG::Rendering::Base::Texture* texture);

    void setRoughnessMap(HG::Rendering::Base::Texture* texture);

    void setAmbientOcclusionMap(HG::Rendering::Base::Texture* texture);

    void setHeightMap(HG::Rendering::Base::Texture* texture);

//    void setMetallic(float value);
//
//    void setRoughness(float roughness);
//
//    void setAmbientOclusion(float ao);
};

