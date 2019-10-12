// HG::Rendering::Base
#include <HG/Rendering/Base/Shader.hpp>

namespace HG::Rendering::Base
{
Shader::Shader() : RenderData(DataId), m_shaderText()
{
}

void Shader::setShaderText(std::string text)
{
    m_shaderText = std::move(text);
}

std::string Shader::shaderText() const
{
    return m_shaderText;
}
} // namespace HG::Rendering::Base
