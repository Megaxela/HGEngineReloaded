// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/CubeMapTextureData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/CubeMap.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Shader.hpp>
#include <HG/Rendering/Base/Texture.hpp>

HG::Rendering::OpenGL::Forward::AbstractRenderer::AbstractRenderer() : m_application(nullptr)
{
}

HG::Core::Application* HG::Rendering::OpenGL::Forward::AbstractRenderer::application() const
{
    return m_application;
}

void HG::Rendering::OpenGL::Forward::AbstractRenderer::setApplication(HG::Core::Application* application)
{
    m_application = application;
}