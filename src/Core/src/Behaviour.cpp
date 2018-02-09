#include "Behaviour.hpp"

CORE_MODULE_NS::Behaviour::Behaviour()
{

}

void CORE_MODULE_NS::Behaviour::update()
{
    onUpdate();
}

void CORE_MODULE_NS::Behaviour::start()
{
    onStart();
}

void CORE_MODULE_NS::Behaviour::onStart()
{

}

void CORE_MODULE_NS::Behaviour::onUpdate()
{

}

void CORE_MODULE_NS::Behaviour::onFixedUpdate()
{

}
