#include "Materials/MeshFallbackMaterial.hpp"

const char* OGL_RENDERING_MODULE_NS::Materials::MeshFallbackMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in vec2 TexCoords;

// texture samplers
uniform sampler2D fallbackTexture;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#endif
)";