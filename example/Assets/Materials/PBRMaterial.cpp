#include "PBRMaterial.hpp"

const char* PBRMaterial::rawShader = R"(
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

//uniform float metallic;
//uniform float roughness;
//uniform float ao;

// texture samplers
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numberOfPointLights;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = max(a * a, 0.001f);
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom; // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometryGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometryGGX(NdotV, roughness);
    float ggx1  = GeometryGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, fs_in.TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.FragPos);
    vec3 Q2  = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 albedo     = pow(texture(albedoMap,    fs_in.TexCoords).rgb, vec3(2.2));
    float metallic  =     texture(metallicMap,  fs_in.TexCoords).r;
    float roughness =     texture(roughnessMap, fs_in.TexCoords).r;
    float ao        =     texture(aoMap,        fs_in.TexCoords).r;


    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < numberOfPointLights; ++i)
    {
        // Calculate per-light radiance
        vec3 L = normalize(pointLights[i].position - fs_in.FragPos);
        vec3 H = normalize(V + L);

        float distance    = length(pointLights[i].position - fs_in.FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = pointLights[i].diffuse * attenuation;

        // Cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        //F = F0;

        vec3 nomenator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = nomenator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color   = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

#endif
)";

void PBRMaterial::setAlbedo(const HG::Utils::Color& color)
{
    set("albedo", color.toRGBVector());
}

void PBRMaterial::setMetallic(float value)
{
    set("metallic", value);
}

void PBRMaterial::setRoughness(float roughness)
{
    set("roughness", roughness);
}

void PBRMaterial::setAmbientOclusion(float ao)
{
    set("ao", ao);
}

void PBRMaterial::setAlbedoMap(HG::Rendering::Base::Texture* texture)
{
    set("albedoMap", texture);
}

void PBRMaterial::setNormalMap(HG::Rendering::Base::Texture* texture)
{
    set("normalMap", texture);
}

void PBRMaterial::setMetallicMap(HG::Rendering::Base::Texture* texture)
{
    set("metallicMap", texture);
}

void PBRMaterial::setRoughnessMap(HG::Rendering::Base::Texture* texture)
{
    set("roughnessMap", texture);
}

void PBRMaterial::setAmbientOcclusionMap(HG::Rendering::Base::Texture* texture)
{
    set("aoMap", texture);
}
