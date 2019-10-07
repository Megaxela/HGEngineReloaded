// RenderToTexture
#include <Assets/Materials/TextureMaterial.hpp>

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
    vs_out.TexCoords = vec2(inTexCoords.x, 1-inTexCoords.y);
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
    vec4 tempColor = texture(textureMap, fs_in.TexCoords) + vec4(0.1, 0.2, 0.3, 0.0);

    tempColor.r = mod(tempColor.r, 1.0f);
    tempColor.g = mod(tempColor.g, 1.0f);
    tempColor.b = mod(tempColor.b, 1.0f);

    resultColor = tempColor;
}
#endif
)";

void TextureMaterial::setTexture(HG::Rendering::Base::Texture* texture)
{
    set("textureMap", texture);
}
