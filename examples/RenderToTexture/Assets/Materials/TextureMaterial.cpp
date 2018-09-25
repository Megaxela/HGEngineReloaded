// RenderToTexture
#include <Assets/Materials/TextureMaterial.hpp>
#include "TextureMaterial.hpp"


const char* TextureMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT
{
    vec2 TexCoords;
}
vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    vs_out.TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
layout (location = 0) out vec4 resultColor;

in VS_OUT
{
    vec2 TexCoords;
}
fs_in;

uniform sampler2D textureMap;

void main()
{
    resultColor = texture(textureMap, fs_in.TexCoords);
}
#endif
)";

void TextureMaterial::setTexture(HG::Rendering::Base::Texture* texture)
{
    set("textureMap", texture);
}
