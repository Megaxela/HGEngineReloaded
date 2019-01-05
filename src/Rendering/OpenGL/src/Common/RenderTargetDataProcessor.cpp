// HG::Core
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/RenderTargetDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/RenderTargetData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::RenderTargetDataProcessor()
{

}

bool HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::setup(HG::Rendering::Base::RenderData* data, bool guarantee)
{
    auto renderTarget = dynamic_cast<HG::Rendering::Base::RenderTarget*>(data);

    if (renderTarget == nullptr)
    {
        Error() << "Got non render target render data in render target data processor. Types are corrupted.";
        exit(-1);
    }

    if (renderTarget->isDefault())
    {
        if (renderTarget->specificData() == nullptr)
        {
            renderTarget->setSpecificData(
                new (application()->resourceCache()) RenderTargetData(
                    std::move(gl::framebuffer(0)),
                    std::move(gl::renderbuffer(0)),
                    glm::ivec2{0, 0}
                )
            );
        }

        return true;
    }

    RenderTargetData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = renderTarget->castSpecificDataTo<RenderTargetData>()) == nullptr)
    {
        externalData = new (application()->resourceCache()) RenderTargetData();
        renderTarget->setSpecificData(externalData);
    }

    if (!externalData->Framebuffer.is_valid())
    {
        externalData->Framebuffer = std::move(gl::framebuffer());
    }

    for (auto iter = renderTarget->colorTextureBegin(),
              end  = renderTarget->colorTextureEnd();
         iter != end;
         ++iter)
    {
        if (renderingPipeline()->needSetup(iter->second))
        {
            BENCH("Setting up render target");
            if (!renderingPipeline()->setup(iter->second))
            {
                // Can't init render target
                return false;
            }
        }

        auto textureExternal = iter->second->castSpecificDataTo<Texture2DData>();

        BENCH("Attaching texture to framebuffer");
        externalData->Framebuffer.attach_texture(
            GL_COLOR_ATTACHMENT0 + iter->first,
            textureExternal->Texture
        );
    }

    if (externalData->Size == renderTarget->size())
    {
        BENCH("Setting storage to renderbuffer");
        externalData->Size = renderTarget->size();
        externalData->Renderbuffer.set_storage(
            GL_DEPTH24_STENCIL8,
            externalData->Size.x, externalData->Size.y
        );
    }

    externalData->Valid = true;

    return true;
}

std::size_t HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::getTarget()
{
    return HG::Rendering::Base::RenderTarget::DataId;
}

bool HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::needSetup(HG::Rendering::Base::RenderData* data)
{
    auto renderTarget = static_cast<HG::Rendering::Base::RenderTarget*>(data);
    auto renderTargetData = renderTarget->castSpecificDataTo<HG::Rendering::OpenGL::Common::RenderTargetData>();

    return  renderTargetData == nullptr ||
            renderTargetData->Size != renderTarget->size() ||
           !renderTargetData->Framebuffer.is_valid() ||
            renderTargetData->Framebuffer.status() == GL_FRAMEBUFFER_COMPLETE ||
           !renderTargetData->Valid;
}
