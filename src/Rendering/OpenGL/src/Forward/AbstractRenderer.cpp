// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp>

namespace HG::Rendering::OpenGL::Forward
{
AbstractRenderer::AbstractRenderer() : m_application(nullptr)
{
}

HG::Core::Application* AbstractRenderer::application() const
{
    return m_application;
}

void AbstractRenderer::setApplication(HG::Core::Application* application)
{
    m_application = application;
}
} // namespace HG::Rendering::OpenGL::Forward
