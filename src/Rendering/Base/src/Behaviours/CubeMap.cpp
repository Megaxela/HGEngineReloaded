// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/CubeMap.hpp>

namespace HG::Rendering::Base::Behaviours
{
CubeMap::CubeMap(HG::Rendering::Base::CubeMap* cubeMap) : RenderBehaviour(RenderBehaviourId), m_cubeMap(cubeMap)
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::setCubeMap(HG::Rendering::Base::CubeMap* cubeMap)
{
    m_cubeMap = cubeMap;
}

HG::Rendering::Base::CubeMap* CubeMap::cubeMap() const
{
    return m_cubeMap;
}
} // namespace HG::Rendering::Base::Behaviours
