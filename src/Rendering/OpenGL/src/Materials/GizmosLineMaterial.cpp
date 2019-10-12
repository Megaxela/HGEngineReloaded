// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Materials/GizmosLineMaterial.hpp>

namespace HG::Rendering::OpenGL::Materials
{
const char* GizmosLineMaterial::rawShader = R"(
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
}
