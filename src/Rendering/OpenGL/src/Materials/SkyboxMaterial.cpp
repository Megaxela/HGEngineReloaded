// HG::Rendering::OpenGL
#include <Materials/SkyboxMaterial.hpp>

const char* HG::Rendering::OpenGL::Materials::SkyboxMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = inPosition;
    vec4 pos = projection * view * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}
#endif

#ifdef FragmentShader
layout (location = 0) out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCubeArray skybox;

void main()
{
    FragColor = texture(skybox, vec4(TexCoords, 0.0));
//    FragColor = vec4(TexCoords, 1.0f);
}
#endif
)";