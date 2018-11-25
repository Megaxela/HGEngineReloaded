// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Materials/BlitMaterial.hpp>

const char* HG::Rendering::OpenGL::Materials::BlitMaterial::rawShader = R"(
#ifdef VertexShader

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 uvPixels;

out vec2 texCoordinates;

uniform ivec2 textureSize;
uniform mat4 projection;

void main()
{
    texCoordinates = uvPixels / textureSize;
    gl_Position = projection * vec4(vertex.xy, 0, 1) * vec4(1.0, -1.0, 1.0, 1.0);
}

#endif

#ifdef FragmentShader

uniform sampler2D sourceTexture;

layout (location = 0) out vec4 fragColor;

in vec2 texCoordinates;

void main()
{
    fragColor = texture(sourceTexture, texCoordinates);
}

#endif
)";
