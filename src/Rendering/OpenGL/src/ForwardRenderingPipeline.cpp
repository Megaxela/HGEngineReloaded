#include <ForwardRenderingPipeline.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <RenderBehaviour.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>
#include <gl/auxiliary/glm_uniforms.hpp>

// Rendering behaviours
#include <Behaviours/Mesh.hpp>
#include <gl/all.hpp>
#include <imgui.h>
#include <Lights/AbstractLight.hpp>
#include <Lights/PointLight.hpp>
#include <Lights/DirectionalLight.hpp>

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

OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application) :
    RenderingPipeline(application),
    m_textureNumber(0),
    m_behavioursCache(),
    m_sortedBehaviours(),
    m_meshFallback(),
    m_spriteShader(),
    m_spriteData(nullptr),
    m_gizmosRenderer(application)
{

}

OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::~ForwardRenderingPipeline()
{
    delete m_spriteData;
}

bool OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::init()
{
    if (!RenderingPipeline::init())
    {
        return false;
    }

    Info() << "Initializing forward rendering.";

    if (!initFallbackShader())
    {
        return false;
    }

    m_gizmosRenderer.init();

    return initSpriteShader();
}

bool HG::Rendering::OpenGL::ForwardRenderingPipeline::initFallbackShader()
{
    Info() << "Creating fallback shader.";

    m_meshFallback.setShaderText(
        R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in vec2 TexCoords;

// texture samplers
uniform sampler2D fallbackTexture;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#endif
)"
    );

    return true;
}

bool HG::Rendering::OpenGL::ForwardRenderingPipeline::initSpriteShader()
{
    Info() << "Creating sprite shader";

    m_spriteShader.setShaderText(
        R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

uniform vec2 size;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition * vec3(size, 1.0f), 1.0f);
    TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in vec2 TexCoords;

// texture samplers
uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, TexCoords).rgba;

    if (color.a >= 0.01)
    {
        FragColor = color;
    }
    else
    {
        discard;
    }
}
#endif
)");

    setup(&m_spriteShader);

    // Initializing MeshData
    m_spriteData = new MeshData();

    // Binding vertex array
    m_spriteData->VAO.bind();

    // Binding vertex buffer object
    m_spriteData->VBO.bind(GL_ARRAY_BUFFER);

    ::UTILS_MODULE_NS::Mesh mesh;

    float scale = 0.01f;

    // Image is flipped by y
    mesh.Vertices = {
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{ 0.5f * scale, -0.5f * scale,  0}, {0.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{-0.5f * scale,  0.5f * scale,  0}, {1.0f,  0.0f}},
    };

    mesh.Indices = {
        0, 1, 2, 3, 4, 5
    };

    // Loading data into VBO
    m_spriteData->VBO.set_data(
        mesh.Vertices.size() * sizeof(::UTILS_MODULE_NS::Vertex),
        mesh.Vertices.data()
    );

    // Binding element buffer object
    m_spriteData->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    m_spriteData->EBO.set_data(
        mesh.Indices.size() * sizeof(uint32_t),
        mesh.Indices.data()
    );

    // Binding vertex buffer
    m_spriteData->VAO.set_vertex_buffer(0, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    m_spriteData->VAO.set_vertex_buffer(1, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    m_spriteData->VAO.set_vertex_buffer(2, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));

    // Enabling attributes
    m_spriteData->VAO.set_attribute_enabled(0, true);
    m_spriteData->VAO.set_attribute_enabled(1, true);
    m_spriteData->VAO.set_attribute_enabled(2, true);
    m_spriteData->VAO.set_attribute_enabled(3, true);
    m_spriteData->VAO.set_attribute_enabled(4, true);

    // Setting
    m_spriteData->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, position)));
    m_spriteData->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, normal)));
    m_spriteData->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, uv)));
    m_spriteData->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, tangent)));
    m_spriteData->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, bitangent)));

    return true;
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects)
{

    // Clearing main buffer
    gl::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});

    gl::clear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    // Getting camera
    auto camera = ::RENDERING_BASE_MODULE_NS::Camera::active();

    if (camera != nullptr)
    {
        proceedGameObjects(objects);
    }

    // Render Gizmos
    application()->renderer()->gizmos()->visitShapes(m_gizmosRenderer);

    m_gizmosRenderer.render();

    // Render ImGui
    ImGui::Render();

    // Swapping graphics buffers
    application()->systemController()->swapBuffers();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::proceedGameObjects(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects)
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
        switch (behaviour->behaviourType())
        {
        case ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Id:
            renderMesh(
                behaviour->gameObject(),
                static_cast<::RENDERING_BASE_MODULE_NS::Behaviours::Mesh*>(
                    behaviour
                )
            );
            break;

        case ::RENDERING_BASE_MODULE_NS::Behaviours::Sprite::Id:
            renderSprite(
                behaviour->gameObject(),
                static_cast<::RENDERING_BASE_MODULE_NS::Behaviours::Sprite*>(
                    behaviour
                )
            );
            break;

        default:
            Info()
                << "Trying to render unknown render behaviour \""
                << SystemTools::getTypeName(behaviour)
                << "\"";
            break;
        }
    }
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(RENDERING_BASE_MODULE_NS::RenderBehaviour *behaviour)
{
    switch (behaviour->behaviourType())
    {
    case ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Id:
        setupMesh(static_cast<Base::Behaviours::Mesh *>(behaviour));
        break;
    default:
        Error() << "Setting up unknown render behaviour.";
        break;
    }
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setupMesh(::RENDERING_BASE_MODULE_NS::Behaviours::Mesh *behaviour)
{
    if (behaviour->mesh() == nullptr)
    {
        Error() << "Can't setup renderer without set mesh.";
        return;
    }

    if (behaviour->externalData<MeshData>() == nullptr)
    {
        behaviour->setExternalData<MeshData>();
    }

    auto data = behaviour->externalData<MeshData>();

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = behaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(
        mesh->Vertices.size() * sizeof(::UTILS_MODULE_NS::Vertex),
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
    data->VAO.set_vertex_buffer(0, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    data->VAO.set_vertex_buffer(1, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    data->VAO.set_vertex_buffer(2, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));

    // Enabling attributes
    data->VAO.set_attribute_enabled(0, true);
    data->VAO.set_attribute_enabled(1, true);
    data->VAO.set_attribute_enabled(2, true);
    data->VAO.set_attribute_enabled(3, true);
    data->VAO.set_attribute_enabled(4, true);

    // Setting
    data->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, position)));
    data->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, normal)));
    data->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, uv)));
    data->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, tangent)));
    data->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, bitangent)));

}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(::RENDERING_BASE_MODULE_NS::CubeMapTexture* texture)
{
    // If one of surfaces are not available
    // post error
    if (texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Right) == nullptr ||
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Left) == nullptr ||
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Top) == nullptr ||
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Bottom) == nullptr ||
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Front) == nullptr ||
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Back) == nullptr)
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
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Right),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Left),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    );
    setupCubeMapSide(
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Top),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Bottom),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    );
    setupCubeMapSide(
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Front),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    );
    setupCubeMapSide(
        texture->getSideSurface(::RENDERING_BASE_MODULE_NS::CubeMapTexture::Side::Back),
        externalData->Texture,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    );

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    externalData->Texture.set_wrap_s(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_t(GL_CLAMP_TO_EDGE);
    externalData->Texture.set_wrap_r(GL_CLAMP_TO_EDGE);
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setupCubeMapSide(const ::UTILS_MODULE_NS::SurfacePtr& surface,
                                                                         gl::cubemap_texture& texture,
                                                                         GLuint side)
{
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

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(::RENDERING_BASE_MODULE_NS::Texture* texture)
{
    // Checking surface on texture
    if (texture->surface() == nullptr)
    {
        Error() << "Can't setup texture without surface on it.";
        return;
    }

    // Creating external data if not presented
    if (texture->externalData<TextureData>() == nullptr)
    {
        texture->setExternalData<TextureData>();
    }

    auto externalData = texture->externalData<TextureData>();
    
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

    // Loading data into texture
    externalData->Texture.set_storage(
            1,       // Levels
            GL_RGBA8, // Internal format
            texture->surface()->Width,
            texture->surface()->Height
    );
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
    externalData->Texture.bind();
}

GLuint OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::getFilter(::RENDERING_BASE_MODULE_NS::Texture::Filtering filter)
{
    switch (filter)
    {
    case Base::Texture::Nearest: return GL_NEAREST;
    case Base::Texture::Linear:  return GL_LINEAR;
    }

    return 0;
}

GLuint OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::getWrapping(::RENDERING_BASE_MODULE_NS::Texture::Wrapping wrapping)
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

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(::RENDERING_BASE_MODULE_NS::Shader *shader)
{
    // Creating external data if not presented
    if (shader->externalData<ShaderData>() == nullptr)
    {
        shader->setExternalData<ShaderData>();
    }

    auto externalData = shader->externalData<ShaderData>();

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

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::renderSprite(::CORE_MODULE_NS::GameObject* gameObject,
                                                                     ::RENDERING_BASE_MODULE_NS::Behaviours::Sprite* spriteBehaviour)
{
    auto* program = m_spriteShader.externalData<ShaderData>();
    auto* spriteExternal = spriteBehaviour->texture()->externalData<TextureData>();

    if (!spriteExternal)
    {
        // todo: Add fallback sprite image
        Error() << "Sprite texture was not defined.";
        return;
    }

    program->Program.use();

    GLint location;

    if ((location = program->Program.uniform_location("model")) != -1)
    {
        program->Program.set_uniform(
            location,
            gameObject->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->Program.uniform_location("view")) != -1)
    {
        program->Program.set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix()
        );
    }

    if ((location = program->Program.uniform_location("projection")) != -1)
    {
        program->Program.set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()
                ->projectionMatrix()
        );
    }

    if ((location = program->Program.uniform_location("size")) != -1)
    {
        program->Program.set_uniform(
            location,
            glm::vec2(
                spriteBehaviour->texture()->surface()->Width,
                spriteBehaviour->texture()->surface()->Height
            )
        );
    }


    if ((location = program->Program.uniform_location("tex")) != -1)
    {
        program->Program.set_uniform_1i(
            location,
            m_textureNumber
        );

        spriteExternal->Texture.set_active(m_textureNumber);
        spriteExternal->Texture.bind();
    }

    m_spriteData->VAO.bind();

    gl::set_blending_enabled(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(6),
        static_cast<GLsizei>(6),
        GL_UNSIGNED_INT
    );

    m_spriteData->VAO.unbind();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::renderMesh(::CORE_MODULE_NS::GameObject* gameObject,
                                                                   ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* meshBehaviour)
{

    auto data = meshBehaviour->externalData<MeshData>();

    // todo: On errors, render "error" mesh instead.
    // Additional information in mesh is
    // contains VAO, VBO, EBO, so here is just
    // rendering of that info.
    if ( data == nullptr ||
        !data->VAO.is_valid() ||
        !data->VBO.is_valid() ||
        !data->EBO.is_valid())
    {
        Error()
            << "Mesh rendering behaviour of \""
            << meshBehaviour->gameObject()->name()
            << "\" has not external data or has non valid buffers.";
        return;
    }

    gl::program* program = nullptr;

    if (meshBehaviour->material() == nullptr ||
        meshBehaviour->material()->shader() == nullptr ||
        meshBehaviour->material()->shader()->externalData<ShaderData>() == nullptr)
    {
        program = &m_meshFallback.externalData<ShaderData>()->Program;

        program->use();
    }
    else
    {
        program = &meshBehaviour->material()->shader()->externalData<ShaderData>()->Program;

        program->use();

        m_textureNumber = 0;
        for (auto&& [uniformName, uniformValue] : *meshBehaviour->material())
        {
            setShaderUniform(program, uniformName, uniformValue);
        }
    }

    // Checking for VBO, VAO and EBO

    GLint location;

    if ((location = program->uniform_location("model")) != -1)
    {
        program->set_uniform(
            location,
            gameObject->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->uniform_location("view")) != -1)
    {
        program->set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix()
        );
    }

    if ((location = program->uniform_location("projection")) != -1)
    {
        program->set_uniform(location, ::RENDERING_BASE_MODULE_NS::Camera::active()->projectionMatrix());
    }

    // Setting lighting uniforms
    auto& lights = ::RENDERING_BASE_MODULE_NS::AbstractLight::totalLights();

    int pointLightIndex = 0;
    int directionalLightIndex = 0;
    int spotLightIndex = 0;

    for (auto&& light : lights)
    {
        switch (light->type())
        {
        case ::RENDERING_BASE_MODULE_NS::AbstractLight::Type::Point:
        {
            auto castedLight = static_cast<::RENDERING_BASE_MODULE_NS::Lights::PointLight*>(light);

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].position")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->gameObject()->transform()->globalPosition()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].constant")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->constant()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].linear")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->linear()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].quadratic")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->quadratic()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].ambient")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->ambient().toRGBVector()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].diffuse")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->diffuse().toRGBVector()
                );
            }

            if ((location = program->uniform_location("pointLights[" + std::to_string(pointLightIndex) + "].specular")) != -1)
            {
                program->set_uniform(
                    location,
                    castedLight->specular().toRGBVector()
                );
            }

            ++pointLightIndex;
            break;
        }
        case ::RENDERING_BASE_MODULE_NS::AbstractLight::Type::Directional:
        {
            auto castedLight = static_cast<::RENDERING_BASE_MODULE_NS::Lights::DirectionalLight*>(light);

            // todo: Finish directional light uniform info
            ++directionalLightIndex;
            break;
        }
        case ::RENDERING_BASE_MODULE_NS::AbstractLight::Type::Spot:
        {
//            auto castedLight = static_cast<::RENDERING_BASE_MODULE_NS::Lights::*>(light);

            // todo: Finish stop light uniform info
            ++spotLightIndex;
            break;
        }
        }
    }

    // todo: Add lighting caching
    // Setting number of point lights
    if ((location = program->uniform_location("numberOfPointLights")) != -1)
    {
        program->set_uniform(location, pointLightIndex);
    }

    // Setting number of directional lights
    if ((location = program->uniform_location("numberOfDirectionalLights")) != -1)
    {
        program->set_uniform(location, directionalLightIndex);
    }

    // Setting number of spot lights
    if ((location = program->uniform_location("numberOfSpotLights")) != -1)
    {
        program->set_uniform(location, spotLightIndex);
    }

    // Setting camera position to shader
    if ((location = program->uniform_location("viewPos")) != -1)
    {
        program->set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()
                ->gameObject()
                ->transform()
                ->globalPosition()
        );
    }

    data->VAO.bind();

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(meshBehaviour->mesh()->Indices.size()),
        static_cast<GLsizei>(meshBehaviour->mesh()->Indices.size()),
        GL_UNSIGNED_INT
    );

    data->VAO.unbind();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setShaderUniform(gl::program *program,
                                                                         const std::string &name,
                                                                         const ::RENDERING_BASE_MODULE_NS::Material::Value &value)
{

    auto location = program->uniform_location(name);

    switch (value.type)
    {
    case Base::Material::Value::Type::Int:
        program->set_uniform(
            location,
            value.integer
        );
        break;
    case Base::Material::Value::Type::Float:
        program->set_uniform(
            location,
            value.floating
        );
        break;
    case Base::Material::Value::Type::Boolean:
        program->set_uniform(
            location,
            value.boolean
        );
        break;
    case Base::Material::Value::Type::Vector2:
        program->set_uniform(
            location,
            value.vector2
        );
        break;
    case Base::Material::Value::Type::Vector3:
        program->set_uniform(
            location,
            value.vector3
        );
        break;
    case Base::Material::Value::Type::Vector4:
        program->set_uniform(
            location,
            value.vector4
        );
        break;
    case Base::Material::Value::Type::Matrix2x2:
        program->set_uniform(
            location,
            value.mat2x2
        );
        break;
    case Base::Material::Value::Type::Matrix3x3:
        program->set_uniform(
            location,
            value.mat3x3
        );
        break;
    case Base::Material::Value::Type::Matrix4x4:
        program->set_uniform(
            location,
            value.mat4x4
        );
        break;
    case Base::Material::Value::Type::Texture:
        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit
        program->set_uniform_1i(
            location,
            m_textureNumber
        );

        value.texture->externalData<TextureData>()->Texture.set_active(m_textureNumber);
        value.texture->externalData<TextureData>()->Texture.bind();

        ++m_textureNumber;

        break;
    }
}
