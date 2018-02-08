#include "Scene.hpp"

HG::Core::Scene::Scene() :
    m_mainApplication(nullptr)
{

}

void HG::Core::Scene::setApplication(HG::Core::Application *application)
{
    m_mainApplication = application;
}

HG::Core::Application *HG::Core::Scene::application() const
{
    return m_mainApplication;
}

void HG::Core::Scene::update()
{

}

void HG::Core::Scene::start()
{

}
