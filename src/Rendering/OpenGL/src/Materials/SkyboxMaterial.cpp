// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Materials/SkyboxMaterial.hpp>

namespace HG::Rendering::OpenGL::Materials
{
const char* SkyboxMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 position;

out vec3 texCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoordinates = position;
    vec4 pos = projection * view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
#endif

#ifdef FragmentShader
layout (location = 0) out vec4 FragColor;

in vec3 texCoordinates;

uniform samplerCubeArray skybox;

void main()
{
    FragColor = texture(skybox, vec4(texCoordinates, 0.0));
}
#endif
)";
}
