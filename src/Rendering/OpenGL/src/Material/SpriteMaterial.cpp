#include "Materials/SpriteMaterial.hpp"

const char* OGL_RENDERING_MODULE_NS::Materials::SpriteMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

uniform vec2 size;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition * vec3(size, 1.0f), 1.0f);
    TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in vec2 TexCoords;

// texture samplers
uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, TexCoords).rgba;

    if (color.a >= 0.01)
    {
        FragColor = color;
    }
    else
    {
        discard;
    }
}
#endif
)";