#include <GameObject.hpp>
#include "RenderBehaviour.hpp"

HG::Rendering::Base::RenderBehaviour::RenderBehaviour(std::size_t type) :
    m_externalData(nullptr),
    m_type(type),
    m_parent(nullptr)
{

}

HG::Rendering::Base::RenderBehaviour::~RenderBehaviour()
{
    clearExternalData();
}

void HG::Rendering::Base::RenderBehaviour::clearExternalData()
{
    delete m_externalData;
}

std::size_t HG::Rendering::Base::RenderBehaviour::behaviourType()
{
    return m_type;
}

::HG::Core::GameObject *HG::Rendering::Base::RenderBehaviour::gameObject()
{
    return m_parent;
}

void HG::Rendering::Base::RenderBehaviour::setParentGameObject(::HG::Core::GameObject *gameObject)
{
    m_parent = gameObject;
}
