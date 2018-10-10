// TransparentExample
#include <Assets/Materials/GlassMaterial.hpp>

const char* GlassMaterial::rawShader = R"(
#ifdef VertexShader

// Indices of refraction
const float Air = 1.0;
const float Glass = 1.51714;

// Air to glass ratio of the indices of refraction (Eta)
const float Eta = Air / Glass;

// see http://en.wikipedia.org/wiki/Refractive_index Reflectivity
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform vec3 camera;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 reflection;
    vec3 refraction;
    float fresnel;
}
vs_out;

void main()
{
    vec4 vertex = model * vec4(inPosition, 1.0f);

    vec3 incident = normalize(vec3(vertex) - camera);

    vs_out.refraction = refract(incident, inNormal, Eta);
    vs_out.reflection = reflect(incident, inNormal);

    vs_out.fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-incident, inNormal)), 5.0);

    gl_Position = projection * view * vertex;
}
#endif

#ifdef FragmentShader

layout (location = 0) out vec4 FragColor;

uniform samplerCubeArray cubemap;

in VS_OUT
{
    vec3 reflection;
    vec3 refraction;
    float fresnel;
}
vs_in;

void main()
{
    vec4 refractionColor = texture(cubemap, vec4(normalize(vs_in.refraction), 0));
    vec4 reflectionColor = texture(cubemap, vec4(normalize(vs_in.reflection), 0));

    FragColor = mix(refractionColor, reflectionColor, vs_in.fresnel);
    FragColor.a = 0.7;
}

#endif
)";