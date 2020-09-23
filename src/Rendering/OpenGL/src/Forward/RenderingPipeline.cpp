// HG::Core
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/GameObject.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/BlitRenderer.hpp>
#include <HG/Rendering/OpenGL/Common/RenderTargetData.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp>
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>
#include <HG/Rendering/OpenGL/GizmosRenderer.hpp>
#include <HG/Rendering/OpenGL/ImGuiRenderer.hpp>
#include <HG/Rendering/OpenGL/Materials/BlitMaterial.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/CubeMap.hpp>
#include <HG/Rendering/Base/BlitData.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/RenderBehaviour.hpp>
#include <HG/Rendering/Base/RenderOverride.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Shader.hpp>
#include <HG/Rendering/Base/SystemController.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/SystemTools.hpp>

// GLM
#include <glm/glm.hpp>

// ImGui
#include <imgui.h>

namespace
{
void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 3203 || id == 131185 || id == 131218 || id == 131204 || id == 8 ||
        id == 131076 || /* Usage warning */
        id == 22 ||     /* CPU mapping a busy "streamed data" BO stalled */
        id == 20 ||     /* GTT mapping a busy "miptree" BO stalled */
        id == 14 ||     /* CPU mapping a busy "miptree" BO stalled */
        id == 18 /* CPU mapping a busy "streamed data" BO stalled */)
    {
        return;
    }

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    std::stringstream ss;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        ss << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        ss << "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        ss << "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        ss << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        ss << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        ss << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        ss << "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        ss << "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        ss << "Other";
        break;
    default:
        ss << "Unexpected";
        break;
    }

    ss << " (severity: ";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        ss << "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        ss << "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        ss << "low";
        break;
    default:
        ss << "unexpected";
        break;
    }

    ss << ", id: " << id;

    ss << ") message received from ";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        ss << "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        ss << "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        ss << "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        ss << "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        ss << "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        ss << "Other";
        break;
    default:
        ss << "Unexpected";
        break;
        ;
    }

    std::string messageCopy(message);

    std::replace(messageCopy.begin(), messageCopy.end(), '\n', ' ');

    ss << ": " << messageCopy;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        HGError(ss.str());
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
    case GL_DEBUG_TYPE_PERFORMANCE:
    case GL_DEBUG_TYPE_MARKER:
        HGWarning(ss.str());
        break;

    case GL_DEBUG_TYPE_PUSH_GROUP:
    case GL_DEBUG_TYPE_POP_GROUP:
    case GL_DEBUG_TYPE_OTHER:
    default:
        HGInfo(ss.str());
        break;
    }
}

} // namespace

namespace HG::Rendering::OpenGL::Forward
{
RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    HG::Rendering::Base::RenderingPipeline(application),
    m_behavioursCache(),
    m_sortedBehaviours(),
    m_cachedViewport({-1, -1}),
    m_gizmosRenderer(new HG::Rendering::OpenGL::GizmosRenderer(application)),
    m_imguiRenderer(new HG::Rendering::OpenGL::ImGuiRenderer(application)),
    m_blitRenderer(new HG::Rendering::OpenGL::BlitRenderer(application)),
    m_renderers(),
    m_savedRenderTarget(nullptr)
{
}

RenderingPipeline::~RenderingPipeline()
{
    for (auto&& renderer : m_renderers)
    {
        delete renderer.second;
    }

    delete m_gizmosRenderer;
    delete m_imguiRenderer;
    delete m_blitRenderer;
}

RenderingPipeline* RenderingPipeline::addRenderer(AbstractRenderer* renderer)
{
    renderer->setApplication(application());
    m_renderers[renderer->getTarget()] = renderer;

    return this;
}

bool RenderingPipeline::init()
{
    if (!HG::Rendering::Base::RenderingPipeline::init())
    {
        return false;
    }

    if (!initDependencies())
    {
        return false;
    }

    // todo: Somehow move this to HG::Rendering::Base::RenderingPipeline
    setRenderTarget(application()->renderer()->defaultRenderTarget());

    initRenderingSetup();

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->init();
    }

    m_gizmosRenderer->init();

    m_blitRenderer->init();

    m_imguiRenderer->init();

    return true;
}

void RenderingPipeline::deinit()
{
    HG::Rendering::Base::RenderingPipeline::deinit();

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->deinit();
    }

    m_gizmosRenderer->deinit();

    m_blitRenderer->deinit();

    m_imguiRenderer->deinit();

    application()->renderer()->materialCollection()->clearCache();
}

bool RenderingPipeline::initDependencies()
{
    glewExperimental = GL_TRUE;
    GLenum error;
    if ((error = glewInit()) != GLEW_OK)
    {
        HGError("Can't init GLEW, error: {}", glewGetErrorString(error));
        return false;
    }

    return true;
}

void RenderingPipeline::initRenderingSetup()
{
#ifndef DNDEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        HGInfo("Turning on OpenGL debug output");
        gl::set_debug_output_enabled(true);
        gl::set_syncronous_debug_output_enabled(true);

        glDebugMessageCallback(&glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    // Enabling cull face to clockwise
    gl::set_polygon_face_culling_enabled(true);
    gl::set_cull_face(GL_BACK);
    gl::set_front_face(GL_CW);

    // Enabling depth test
    gl::set_depth_test_enabled(true);
    gl::set_depth_function(GL_LESS);

    // Blending
    gl::set_blending_enabled(true);
    gl::set_blend_function(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderingPipeline::clear(HG::Utils::Color color)
{
    proceedRenderTargetOverride();
    updateViewport();

    // Clearing main buffer
    auto colorArray = color.toRGBAVector();

    gl::set_clear_color({colorArray[0], colorArray[1], colorArray[2], colorArray[3]});

    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingPipeline::render(const std::vector<HG::Core::GameObject*>& objects)
{
    clear(HG::Utils::Color::fromRGB(25, 25, 25));

    // Getting camera
    auto camera = application()->renderer()->activeCamera();

    if (camera != nullptr)
    {
        proceedGameObjects(objects);
    }

    // Restore render target (after override)
    if (m_savedRenderTarget != nullptr)
    {
        setRenderTarget(m_savedRenderTarget);
        m_savedRenderTarget = nullptr;
    }

    // Render Gizmos
    {
        BENCH("Gizmos rendering");
        application()->renderer()->gizmos()->visitShapes(*m_gizmosRenderer);
        m_gizmosRenderer->render();
    }

    // Render ImGui
    {
        BENCH("ImGUI rendering");
        ImGui::Render();
        m_imguiRenderer->render();
    }

    // Swapping graphics buffers
    BENCH("Swapping buffers");
    application()->systemController()->swapBuffers();
}

void RenderingPipeline::proceedGameObjects(const std::vector<HG::Core::GameObject*>& objects)
{
    BENCH("Proceeding gameobjects");
    m_sortedBehaviours.clear();
    HG::Rendering::Base::RenderBehaviour* cubemapBehaviour = nullptr;

    // Getting camera positions
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::quat cameraRot;

    auto* camera = application()->renderer()->activeCamera();

    cameraPos = camera->gameObject()->transform()->globalPosition();
    cameraRot = camera->gameObject()->transform()->globalRotation();

    {
        BENCH("Gameobjects sorting");
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

                if (behaviour->renderBehaviourType() == HG::Rendering::Base::Behaviours::CubeMap::RenderBehaviourId)
                {
                    if (cubemapBehaviour)
                    {
                        HGWarning("Several cubemap behaviours are available at the same time");
                    }

                    cubemapBehaviour = behaviour;
                }
                else
                {
                    // Not inverting, because Z is positive towards camera
                    m_sortedBehaviours.insert({(cameraSpace * glm::inverse(cameraRot)).z, behaviour});
                }
            }
        }
    }

    if (cubemapBehaviour != nullptr)
    {
        BENCH("Cubemap rendering");
        // Disabling depth test
        gl::set_depth_test_enabled(false);

        // Render cubemap
        render(cubemapBehaviour);

        // Enabling depth test back
        gl::set_depth_test_enabled(true);
    }

    // Rendering other scene
    for (auto& [distance, behaviour] : m_sortedBehaviours)
    {
        BENCH("Rendering behaviour");
        render(behaviour);
    }
}

bool RenderingPipeline::render(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    proceedRenderTargetOverride();

    auto rendererIterator = m_renderers.find(behaviour->renderBehaviourType());

    if (rendererIterator == m_renderers.end())
    {
        HGInfo("Trying to render unknown render behaviour \"{}\"", HG::Utils::SystemTools::getTypeName(*behaviour));
        return false;
    }

    // If rendertarget size changed - change viewport
    updateViewport();

    rendererIterator->second->render(behaviour);

    return true;
}

void RenderingPipeline::setRenderTarget(HG::Rendering::Base::RenderTarget* target)
{
    HG::Rendering::Base::RenderingPipeline::setRenderTarget(target);

    if (needSetup(target))
    {
        if (!setup(target))
        {
            HGError("Can't setup target");
            return;
        }
    }

    auto externalData = target->castSpecificDataTo<Common::RenderTargetData>();

    externalData->Framebuffer.bind(GL_FRAMEBUFFER);

    updateViewport();
}

void RenderingPipeline::updateViewport()
{
    if (m_cachedViewport != renderTarget()->size())
    {
        m_cachedViewport = renderTarget()->size();
        gl::set_viewport({0, 0}, {m_cachedViewport.x, m_cachedViewport.y});
    }
}

void RenderingPipeline::proceedRenderTargetOverride()
{
    if ((renderOverride() == nullptr || renderOverride()->mainRenderTarget == nullptr) &&
        m_savedRenderTarget != nullptr)
    {
        setRenderTarget(m_savedRenderTarget);
        m_savedRenderTarget = nullptr;
    }
    else if (renderOverride() && renderOverride()->mainRenderTarget)
    {
        if (m_savedRenderTarget == nullptr)
        {
            m_savedRenderTarget = renderTarget();
        }

        setRenderTarget(renderOverride()->mainRenderTarget);
    }
}

void RenderingPipeline::getTextureRegion(HG::Rendering::Base::Texture* texture,
                                         glm::ivec2 tl,
                                         glm::ivec2 br,
                                         std::uint8_t* data)
{
    BENCH("Getting texture pixel region");
    auto textureData = texture->castSpecificDataTo<Common::Texture2DData>();

    if (textureData == nullptr)
    {
        HGError("No texture data");
        return;
    }

    auto vecData =
        textureData->Texture.sub_image(0, tl.x, tl.y, 0, br.x - tl.x, br.y - tl.y, 1, GL_RGBA, GL_UNSIGNED_BYTE);

    for (auto i : vecData)
    {
        *(data++) = i;
    }
}

void RenderingPipeline::blit(HG::Rendering::Base::RenderTarget* target, HG::Rendering::Base::BlitData* blitData)
{
    auto savedRenderTarget = renderTarget();
    setRenderTarget(target);

    for (const auto& [texture, data] : blitData->blitContainer())
    {
        m_blitRenderer->render(target, texture, data);
    }

    setRenderTarget(savedRenderTarget);
}

const std::string& RenderingPipeline::pipelineName() const
{
    static std::string name = "OpenGL Forward";
    return name;
}

RenderingPipeline::Backend RenderingPipeline::pipelineBackend() const
{
    return HG::Rendering::Base::RenderingPipeline::Backend::OpenGL;
}
} // namespace HG::Rendering::OpenGL::Forward
