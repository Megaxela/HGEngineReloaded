// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Shader.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderDataProcessor.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

#define SHADER_DEFAULT_STRUCTS           \
    "#define MAX_POINT_LIGHTS 128\n"     \
    "#define MAX_DIRECTIONAL_LIGHTS 2\n" \
    "#define MAX_SPOT_LIGHTS 32\n"       \
    "struct DirectionalLight\n"          \
    "{\n"                                \
    "    vec3 direction;\n"              \
    "    vec3 ambient;\n"                \
    "    vec3 diffuse;\n"                \
    "    vec3 specular;\n"               \
    "};\n"                               \
    "struct PointLight\n"                \
    "{\n"                                \
    "    vec3 position;\n"               \
    "    float linear;\n"                \
    "    float constant;\n"              \
    "    float quadratic;\n"             \
    "    vec3 ambient;\n"                \
    "    vec3 diffuse;\n"                \
    "    vec3 specular;\n"               \
    "};\n"                               \
    "struct SpotLight\n"                 \
    "{\n"                                \
    "    vec3 position;\n"               \
    "    vec3 direction;\n"              \
    "    float cutOff;\n"                \
    "    float outerCutOff;\n"           \
    "    float constant;\n"              \
    "    float linear;\n"                \
    "    float quadratic;\n"             \
    "    vec3 ambient;\n"                \
    "    vec3 diffuse;\n"                \
    "    vec3 specular;\n"               \
    "};\n"

namespace HG::Rendering::OpenGL::Common
{
bool ShaderDataProcessor::setup(HG::Rendering::Base::RenderData* data, bool guarantee)
{
    auto shader = dynamic_cast<HG::Rendering::Base::Shader*>(data);

    if (shader == nullptr)
    {
        HGError("Got non shader render data in shader data processor, types are corrupted");
        exit(-1);
    }

    ShaderData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = shader->castSpecificDataTo<ShaderData>()) == nullptr)
    {
        externalData = new (application()->resourceCache()) ShaderData();
        shader->setSpecificData(externalData);
    }

    if (externalData->Program.id() == gl::invalid_id)
    {
        externalData->Program = std::move(gl::program());
    }

    gl::shader vertexShader(GL_VERTEX_SHADER);
    gl::shader fragmentShader(GL_FRAGMENT_SHADER);

    {
        BENCH("Building vertex shader");
        vertexShader.set_source("#version 420 core\n" SHADER_DEFAULT_STRUCTS "#define VertexShader\n" +
                                shader->shaderText());

        if (!vertexShader.compile())
        {
            HGError("Can't compile vertex shader, error: {}", vertexShader.info_log());
            exit(-1);
            return false;
        }
    }

    {
        BENCH("Building fragment shader");
        fragmentShader.set_source("#version 420 core\n" SHADER_DEFAULT_STRUCTS "#define FragmentShader\n" +
                                  shader->shaderText());

        if (!fragmentShader.compile())
        {
            HGError("Can't compile fragment shader, error: {}", fragmentShader.info_log());
            exit(-1);
            return false;
        }
    }

    if (BENCH_I("Validating program"), !externalData->Program.is_valid())
    {
        HGError("Shader is not valid");
        return false;
    }

    BENCH("Linking shaders to program");
    externalData->Program.attach_shader(vertexShader);
    externalData->Program.attach_shader(fragmentShader);

    if (!externalData->Program.link())
    {
        HGError("Can't link shader, error: {}", externalData->Program.info_log());
        return false;
    }

    externalData->Valid = true;

    return true;
}

bool ShaderDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto shaderData = data->castSpecificDataTo<ShaderData>();

    return shaderData == nullptr || shaderData->Program.id() == gl::invalid_id || !shaderData->Valid;
}

std::size_t ShaderDataProcessor::getTarget()
{
    return HG::Rendering::Base::Shader::DataId;
}
} // namespace HG::Rendering::OpenGL::Common
