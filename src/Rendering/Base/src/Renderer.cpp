#include <CurrentLogger.hpp>
#include "Renderer.hpp"

HG::Rendering::Base::Renderer::Renderer() :
    m_pipeline(nullptr)
{
    Debug() << "Creating renderer.";
}

HG::Rendering::Base::Renderer::~Renderer()
{
    delete m_pipeline;

    Debug() << "Destroying renderer.";
}

void HG::Rendering::Base::Renderer::render()
{

}
