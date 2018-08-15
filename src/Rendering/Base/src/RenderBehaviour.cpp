#include <GameObject.hpp>
#include "RenderBehaviour.hpp"

HG::Rendering::Base::RenderBehaviour::RenderBehaviour(std::size_t type) :
    Behaviour(HG::Core::Behaviour::Type::Render),
    RenderData(DataId),
    m_type(type)
{

}

std::size_t HG::Rendering::Base::RenderBehaviour::renderBehaviourType()
{
    return m_type;
}

void HG::Rendering::Base::RenderBehaviour::onStart()
{
    Behaviour::onStart();
}

void HG::Rendering::Base::RenderBehaviour::onUpdate()
{
    Behaviour::onUpdate();
}

void HG::Rendering::Base::RenderBehaviour::onFixedUpdate()
{
    Behaviour::onFixedUpdate();
}
