// HG::Rendering::Base
#include <RenderTarget.hpp>
#include <RenderingPipeline.hpp>
#include <Texture.hpp>

// HG::Rendering::OpenGL
#include <Common/RenderTargetDataProcessor.hpp>
#include <Common/RenderTargetData.hpp>
#include <Common/Texture2DData.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::RenderTargetDataProcessor() :
    m_defaultData(new RenderTargetData(
        std::move(gl::framebuffer(0)),
        std::move(gl::renderbuffer(0)),
        glm::ivec2{0, 0}
    ))
{

}

bool HG::Rendering::OpenGL::Common::RenderTargetDataProcessor::setup(HG::Rendering::Base::RenderData* data)
{
    auto renderTarget = dynamic_cast<HG::Rendering::Base::RenderTarget*>(data);

    if (renderTarget == nullptr)
    {
        Error() << "Got non render target render data in render target data processor. Types are corrupted.";
        exit(-1);
    }

    if (renderTarget->isDefault())
    {
        if (renderTarget->specificData() != m_defaultData)
        {
            renderTarget->setSpecificData(m_defaultData);
        }

        return true;
    }

    RenderTargetData* externalData = nullptr;

    // Creating external data if not presented
    if ((externalData = static_cast<RenderTargetData*>(renderTarget->specificData())) == nullptr)
    {
        externalData = new RenderTargetData();
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
            if (!renderingPipeline()->setup(iter->second))
            {
                // Can't init render target
                return false;
            }
        }

        auto textureExternal = static_cast<Texture2DData*>(iter->second->specificData());

        externalData->Framebuffer.attach_texture(
            GL_COLOR_ATTACHMENT0 + iter->first,
            textureExternal->Texture
        );
    }

    if (externalData->Size == renderTarget->size())
    {
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
    auto renderTargetData = static_cast<HG::Rendering::OpenGL::Common::RenderTargetData*>(renderTarget->specificData());

    return  renderTargetData == nullptr ||
            renderTargetData->Size != renderTarget->size() ||
           !renderTargetData->Framebuffer.is_valid() ||
            renderTargetData->Framebuffer.status() == GL_FRAMEBUFFER_COMPLETE ||
           !renderTargetData->Valid;
}
