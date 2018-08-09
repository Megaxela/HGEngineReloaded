#include "Shader.hpp"

HG::Rendering::Base::Shader::Shader() :
    m_shaderText(),
    m_externalData(nullptr)
{

}

HG::Rendering::Base::Shader::~Shader()
{
    clearExternalData();
}

void HG::Rendering::Base::Shader::clearExternalData()
{
    delete m_externalData;

    m_externalData = nullptr;
}

void HG::Rendering::Base::Shader::setShaderText(std::string text)
{
    m_shaderText = std::move(text);
}

std::string HG::Rendering::Base::Shader::shaderText() const
{
    return m_shaderText;
}
