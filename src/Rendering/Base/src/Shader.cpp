#include "Shader.hpp"

RENDERING_BASE_MODULE_NS::Shader::Shader() :
    m_shaderText(),
    m_externalData(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Shader::~Shader()
{
    clearExternalData();
}

void RENDERING_BASE_MODULE_NS::Shader::clearExternalData()
{
    delete m_externalData;

    m_externalData = nullptr;
}

void RENDERING_BASE_MODULE_NS::Shader::setShaderText(std::string text)
{
    m_shaderText = std::move(text);
}

std::string RENDERING_BASE_MODULE_NS::Shader::shaderText() const
{
    return m_shaderText;
}
