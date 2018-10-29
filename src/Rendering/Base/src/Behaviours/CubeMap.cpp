// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/CubeMap.hpp>

HG::Rendering::Base::Behaviours::CubeMap::CubeMap(HG::Rendering::Base::CubeMap* cubeMap) :
    RenderBehaviour(RenderBehaviourId),
    m_cubeMap(cubeMap)
{

}

HG::Rendering::Base::Behaviours::CubeMap::~CubeMap()
{

}

void HG::Rendering::Base::Behaviours::CubeMap::setCubeMap(HG::Rendering::Base::CubeMap* cubeMap)
{
    m_cubeMap = cubeMap;
}

HG::Rendering::Base::CubeMap* HG::Rendering::Base::Behaviours::CubeMap::cubeMap() const
{
    return m_cubeMap;
}
