#pragma once

#include <HG/Rendering/Base/Material.hpp>
#include <HG/Utils/Color.hpp>

namespace HG::Rendering::Base
{
class Texture;
}

class PBRMaterial : public HG::Rendering::Base::Material
{
public:
    static const char* rawShader;

    void setAlbedoMap(HG::Rendering::Base::Texture* texture);

    void setNormalMap(HG::Rendering::Base::Texture* texture);

    void setMetallicMap(HG::Rendering::Base::Texture* texture);

    void setRoughnessMap(HG::Rendering::Base::Texture* texture);

    void setAmbientOcclusionMap(HG::Rendering::Base::Texture* texture);

    void setHeightMap(HG::Rendering::Base::Texture* texture);
};
