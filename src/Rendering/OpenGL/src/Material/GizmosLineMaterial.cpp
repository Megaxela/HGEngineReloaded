#include "Materials/GizmosLineMaterial.hpp"

const char* OGL_RENDERING_MODULE_NS::Materials::GizmosLineMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
out vec4 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    Color = color;
    gl_Position = projection * view * vec4(vertex, 1.0);
}
#endif

#ifdef FragmentShader
in vec4 Color;
out vec4 color;

void main()
{
    color = Color;
}
#endif
)";