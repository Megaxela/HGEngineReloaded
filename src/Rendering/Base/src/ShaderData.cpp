#include "Shader.hpp"

HG::Rendering::Base::Shader::Shader() :
    RenderData(DataId),
    m_shaderText()
{

}

void HG::Rendering::Base::Shader::setShaderText(std::string text)
{
    m_shaderText = std::move(text);
}

std::string HG::Rendering::Base::Shader::shaderText() const
{
    return m_shaderText;
}
