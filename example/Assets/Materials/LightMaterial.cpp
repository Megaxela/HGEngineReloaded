#include "LightMaterial.hpp"

const char* LightMaterial::rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    vs_out.FragPos = vec3(model * vec4(inPosition, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
    vs_out.TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;

// texture samplers
uniform sampler2D diffuseTexture;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numberOfPointLights;

vec4 proceedPointLight(PointLight light, vec3 ambient, vec3 color);

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

    // Ambient
    vec3 ambient = 0.2 * color;

    if (numberOfPointLights == 0)
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    vec4 mixedColor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < numberOfPointLights; ++i)
    {
        mixedColor += proceedPointLight(pointLights[i], ambient, color);
    }

    FragColor = mixedColor;
}

vec4 proceedPointLight(PointLight light, vec3 ambient, vec3 color)
{
    // Diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // Assuming bright white light color
    vec3 specular = vec3(0.3) * spec;

    return vec4(ambient + diffuse + specular, 1.0);
}

#endif
)";

void LightMaterial::setDiffuse(HG::Rendering::Base::Texture* texture)
{
    set("diffuseTexture", texture);
}
