#include <Forward/RenderingPipeline.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <RenderBehaviour.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>

// Rendering behaviours
#include <Behaviours/Mesh.hpp>
#include <gl/all.hpp>
#include <imgui.h>
#include <Lights/AbstractLight.hpp>
#include <Lights/PointLight.hpp>
#include <Lights/DirectionalLight.hpp>
#include <Materials/MeshFallbackMaterial.hpp>
#include <Materials/SpriteMaterial.hpp>
#include <Common/ShaderData.hpp>
#include <Common/Texture2DData.hpp>

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

HG::Rendering::OpenGL::Forward::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    HG::Rendering::Base::RenderingPipeline(application),
    m_behavioursCache(),
    m_sortedBehaviours(),
    m_gizmosRenderer(application),
    m_imguiRenderer(application),
    m_renderers()
{

}

HG::Rendering::OpenGL::Forward::RenderingPipeline::~RenderingPipeline()
{

}

HG::Rendering::OpenGL::Forward::RenderingPipeline *HG::Rendering::OpenGL::Forward::RenderingPipeline::addRenderer(HG::Rendering::OpenGL::Forward::AbstractRenderer *renderer)
{
    renderer->setApplication(application());
    m_renderers[renderer->getTarget()] = renderer;

    return this;
}

bool HG::Rendering::OpenGL::Forward::RenderingPipeline::init()
{
    if (!HG::Rendering::Base::RenderingPipeline::init())
    {
        return false;
    }

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->init();
    }

    m_gizmosRenderer.init();

    m_imguiRenderer.init();

    return true;
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::render(const HG::Core::Scene::GameObjectsContainer& objects)
{

    // Clearing main buffer
    gl::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});

    gl::clear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    // Getting camera
    auto camera = HG::Rendering::Base::Camera::active();

    if (camera != nullptr)
    {
        proceedGameObjects(objects);
    }

    // Render Gizmos
    application()->renderer()->gizmos()->visitShapes(m_gizmosRenderer);

    m_gizmosRenderer.render();

    // Render ImGui
    ImGui::Render();

    m_imguiRenderer.render();

    // Swapping graphics buffers
    application()->systemController()->swapBuffers();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::proceedGameObjects(const HG::Core::Scene::GameObjectsContainer& objects)
{
    // todo: Add cubemap rendering

    m_sortedBehaviours.clear();

    // Getting camera positions
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::quat cameraRot;

    auto* camera = HG::Rendering::Base::Camera::active();

    cameraPos = camera->gameObject()->transform()->globalPosition();
    cameraRot = camera->gameObject()->transform()->globalRotation();

    // Using multimap for sorting objects by distance from camera
    for (auto&& gameObject : objects)
    {
        if (!gameObject->isEnabled())
        {
            continue;
        }

        m_behavioursCache.clear();

        // Getting rendering behaviours of GO
        gameObject->getRenderingBehaviours(m_behavioursCache);

        for (auto&& behaviour : m_behavioursCache)
        {
            if (!behaviour->isEnabled())
            {
                continue;
            }

            auto cameraSpace = gameObject->transform()->globalPosition() - cameraPos;

            // Not inverting, because Z is positive towards camera)
            m_sortedBehaviours.insert({
                (cameraSpace * glm::inverse(cameraRot)).z,
                behaviour
            });
        }
    }

    for (auto& [distance, behaviour] : m_sortedBehaviours)
    {
        auto renderer = m_renderers.find(behaviour->renderBehaviourType());

        if (renderer == m_renderers.end())
        {
            Info()
                << "Trying to render unknown render behaviour \""
                << SystemTools::getTypeName(behaviour)
                << "\"";
            continue;
        }

        renderer->second->render(behaviour->gameObject(), behaviour);
    }
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setup(HG::Rendering::Base::RenderBehaviour *behaviour)
{
    switch (behaviour->renderBehaviourType())
    {
    case HG::Rendering::Base::Behaviours::Mesh::Id:
        setupMesh(static_cast<Base::Behaviours::Mesh *>(behaviour));
        break;
    default:
        Error() << "Setting up unknown render behaviour.";
        break;
    }
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setupMesh(HG::Rendering::Base::Behaviours::Mesh *behaviour)
{
    if (behaviour->mesh() == nullptr)
    {
        Error() << "Can't setup renderer without set mesh.";
        return;
    }

    if (behaviour->externalData<Common::MeshData>() == nullptr)
    {
        behaviour->setExternalData<Common::MeshData>();
    }

    auto data = behaviour->externalData<Common::MeshData>();

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = behaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(
        mesh->Vertices.size() * sizeof(HG::Utils::Vertex),
        mesh->Vertices.data()
    );

    // Binding element buffer object
    data->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    data->EBO.set_data(
        mesh->Indices.size() * sizeof(uint32_t),
        mesh->Indices.data()
    );

    // Binding vertex buffer
    data->VAO.set_vertex_buffer(0, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(1, data->VBO, 0, sizeof(HG::Utils::Vertex));
    data->VAO.set_vertex_buffer(2, data->VBO, 0, sizeof(HG::Utils::Vertex));

    // Enabling attributes
    data->VAO.set_attribute_enabled(0, true);
    data->VAO.set_attribute_enabled(1, true);
    data->VAO.set_attribute_enabled(2, true);
    data->VAO.set_attribute_enabled(3, true);
    data->VAO.set_attribute_enabled(4, true);

    // Setting
    data->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, position)));
    data->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, normal)));
    data->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, uv)));
    data->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, tangent)));
    data->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(HG::Utils::Vertex, bitangent)));

}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setup(HG::Rendering::Base::CubeMapTexture* texture)
{
    // If one of surfaces are not available
    // post error
    if (texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Right) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Left) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Top) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Bottom) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Front) == nullptr ||
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Back) == nullptr)
    {
        Error() << "Can't setup not fully set up cube map texture.";
        return;
    }

    // Creating external data if not presented
    if (texture->externalData<CubeMapTextureData>() == nullptr)
    {
        texture->setExternalData<CubeMapTextureData>();
    }

    auto externalData = texture->externalData<CubeMapTextureData>();

    // Setting up textures
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
    //#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
    //#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Right),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Left),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Top),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Bottom),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Front),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    );
    setupCubeMapSide(
        texture->getSideSurface(HG::Rendering::Base::CubeMapTexture::Side::Back),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    );

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    externalData->Texture.set_wrap_s(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_t(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_r(GL_CLAMP_TO_EDGE);
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setupCubeMapSide(const HG::Utils::SurfacePtr& surface,
                                                                         gl::cubemap_texture& texture,
                                                                         GLuint side)
{
    // todo: Fix this `unused` variable.
    (void) texture;
    GLuint fileFormat = GL_RGB;

    // Getting type
    switch (surface->Bpp)
    {
    case 1:
        fileFormat = GL_RED;
        break;

    case 2:
        fileFormat = GL_RG;
        break;

    case 3:
        fileFormat = GL_RGB;
        break;

    case 4:
        fileFormat = GL_RGBA;
        break;

    default:
        Error() << "Can't setup texture because of unknown texture format.";
        break;
    }

    // Loading data into texture
    glTexImage2D(
        side,
        0,
        GL_RGBA8,
        surface->Width,
        surface->Height,
        0,
        fileFormat,
        GL_UNSIGNED_BYTE,
        surface->Data
    );

//    texture.set_storage(
//        1,       // Levels
//        GL_RGBA8, // Internal format
//        surface->Width,
//        surface->Height
//    );
//    texture.set_sub_image(
//        0, // Level
//        0, // X offset
//        0, // Y Offset
//        surface->Width,  // Width
//        surface->Height, // Height
//        fileFormat,       // Format
//        GL_UNSIGNED_BYTE, // Type
//        surface->Data
//    );
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setup(HG::Rendering::Base::Texture* texture)
{
    // Checking surface on texture
    if (texture->surface() == nullptr)
    {
        Error() << "Can't setup texture without surface on it.";
        return;
    }

    // Creating external data if not presented
    if (texture->externalData<Common::Texture2DData>() == nullptr)
    {
        texture->setExternalData<Common::Texture2DData>();
    }

    auto externalData = texture->externalData<Common::Texture2DData>();
    
    externalData->Texture.set_min_filter(
        getFilter(
            texture->minificationMethod()
        )
    );
    externalData->Texture.set_mag_filter(
        getFilter(
            texture->magnificationMethod()
        )
    );

    GLuint fileFormat;

    // Getting type
    switch (texture->surface()->Bpp)
    {
    case 1:
        fileFormat = GL_RED;
        break;

    case 2:
        fileFormat = GL_RG;
        break;

    case 3:
        fileFormat = GL_RGB;
        break;

    case 4:
        fileFormat = GL_RGBA;
        break;

    default:
        Error() << "Can't setup texture because of unknown texture format.";
        break;
    }

    // Setting up storage
    externalData->Texture.set_storage(
            1,       // Levels
            GL_RGBA8, // Internal format
            texture->surface()->Width,
            texture->surface()->Height
    );

    // Loading data into texture
    externalData->Texture.set_sub_image(
        0, // Level
        0, // X offset
        0, // Y Offset
        texture->surface()->Width,  // Width
        texture->surface()->Height, // Height
        fileFormat,       // Format
        GL_UNSIGNED_BYTE, // Type
        texture->surface()->Data
    );

    externalData->Texture.unbind();
}

GLuint HG::Rendering::OpenGL::Forward::RenderingPipeline::getFilter(HG::Rendering::Base::Texture::Filtering filter)
{
    switch (filter)
    {
    case Base::Texture::Nearest: return GL_NEAREST;
    case Base::Texture::Linear:  return GL_LINEAR;
    }

    return 0;
}

GLuint HG::Rendering::OpenGL::Forward::RenderingPipeline::getWrapping(HG::Rendering::Base::Texture::Wrapping wrapping)
{
    switch (wrapping)
    {
    case Base::Texture::Repeat:         return GL_REPEAT;
    case Base::Texture::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case Base::Texture::ClampToEdge:    return GL_CLAMP_TO_EDGE;
    case Base::Texture::ClampToBorder:  return GL_CLAMP_TO_BORDER;
    }

    return 0;
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setup(HG::Rendering::Base::Shader *shader)
{
    // Creating external data if not presented
    if (shader->externalData<Common::ShaderData>() == nullptr)
    {
        shader->setExternalData<Common::ShaderData>();
    }

    auto externalData = shader->externalData<Common::ShaderData>();

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
        return /* false */;
    }

    std::string fragmentShaderSource =
        "#version 420 core\n"
        SHADER_DEFAULT_STRUCTS
        "#define FragmentShader\n" +
        shader->shaderText();

    fragmentShader.set_source(fragmentShaderSource);

    if (!fragmentShader.compile())
    {
        Error() << "Can't compile fragment shader. " << fragmentShader.info_log();
        return /* false */;
    }

    if (!externalData->Program.is_valid())
    {
        Error() << "Fallback shader is not valid.";
    }

    externalData->Program.attach_shader(vertexShader);
    externalData->Program.attach_shader(fragmentShader);
    if (!externalData->Program.link())
    {
        Error() << "Can't link shader. " << externalData->Program.info_log();
        return /* false */;
    }
}