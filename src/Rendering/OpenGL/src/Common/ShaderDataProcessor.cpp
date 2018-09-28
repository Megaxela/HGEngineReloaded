// HG::Rendering::Base
#include <Shader.hpp>

// HG::Rendering::OpenGL
#include <Common/ShaderDataProcessor.hpp>
#include <Common/ShaderData.hpp>

// ALogger
#include <CurrentLogger.hpp>

#define SHADER_DEFAULT_STRUCTS \
"#define MAX_POINT_LIGHTS 128\n" \
"#define MAX_DIRECTIONAL_LIGHTS 2\n" \
"#define MAX_SPOT_LIGHTS 32\n" \
"struct DirectionalLight\n" \
"{\n" \
"    vec3 direction;\n" \
"    vec3 ambient;\n" \
"    vec3 diffuse;\n" \
"    vec3 specular;\n" \
"};\n" \
"struct PointLight\n" \
"{\n" \
"    vec3 position;\n" \
"    float linear;\n" \
"    float constant;\n" \
"    float quadratic;\n" \
"    vec3 ambient;\n" \
"    vec3 diffuse;\n" \
"    vec3 specular;\n" \
"};\n" \
"struct SpotLight\n" \
"{\n" \
"    vec3 position;\n" \
"    vec3 direction;\n" \
"    float cutOff;\n" \
"    float outerCutOff;\n" \
"    float constant;\n" \
"    float linear;\n" \
"    float quadratic;\n" \
"    vec3 ambient;\n" \
"    vec3 diffuse;\n" \
"    vec3 specular;\n" \
"};\n"

bool HG::Rendering::OpenGL::Common::ShaderDataProcessor::setup(HG::Rendering::Base::RenderData* data)
{
    auto shader = dynamic_cast<HG::Rendering::Base::Shader*>(data);

    if (shader == nullptr)
    {
        Error() << "Got non shader render data in shader data processor. Types are corrupted.";
        exit(-1);
    }

    ShaderData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = static_cast<ShaderData*>(shader->specificData())) == nullptr)
    {
        externalData = new ShaderData();
        shader->setSpecificData(externalData);
    }

    if (!externalData->Program.is_valid())
    {
        externalData->Program = std::move(gl::program());
    }

    gl::shader vertexShader(GL_VERTEX_SHADER);
    gl::shader fragmentShader(GL_FRAGMENT_SHADER);

    vertexShader.set_source(
        "#version 420 core\n"
        SHADER_DEFAULT_STRUCTS
        "#define VertexShader\n" +
        shader->shaderText()
    );

    if (!vertexShader.compile())
    {
        Error() << "Can't compile vertex shader. Error: " << vertexShader.info_log();
        exit(-1);
        return false;
    }

    fragmentShader.set_source(
        "#version 420 core\n"
        SHADER_DEFAULT_STRUCTS
        "#define FragmentShader\n" +
        shader->shaderText()
    );

    if (!fragmentShader.compile())
    {
        Error() << "Can't compile fragment shader. " << fragmentShader.info_log();
        exit(-1);
        return false;
    }

    if (!externalData->Program.is_valid())
    {
        Error() << "Shader is not valid.";
        return false;
    }

    externalData->Program.attach_shader(vertexShader);
    externalData->Program.attach_shader(fragmentShader);

    if (!externalData->Program.link())
    {
        Error() << "Can't link shader. " << externalData->Program.info_log();
        return false;
    }

    return true;
}

bool HG::Rendering::OpenGL::Common::ShaderDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto shaderData = static_cast<ShaderData*>(data->specificData());

    return  shaderData == nullptr ||
           !shaderData->Program.is_valid();
}

std::size_t HG::Rendering::OpenGL::Common::ShaderDataProcessor::getTarget()
{
    return HG::Rendering::Base::Shader::DataId;
}
