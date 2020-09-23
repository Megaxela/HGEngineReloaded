// HG::System::PC
#include <HG/System/PC/InitializerFabric.hpp>
#include <HG/System/PC/OpenGL/ContextInitializer.hpp>
#include <HG/System/PC/Vulkan/ContextInitializer.hpp>

namespace HG::System::PC
{
    InitializerFabric::InitializerFabric() :
        m_builders()
    {
        addInitializer<HG::System::PC::OpenGL::ContextInitializer>();
        addInitializer<HG::System::PC::Vulkan::ContextInitializer>();
    }

    std::shared_ptr<HG::System::PC::Base::ContextInitializer>
    InitializerFabric::build(HG::Rendering::Base::RenderingPipeline::Backend backend) const
    {
        auto iter = m_builders.find(backend);

        if (iter == m_builders.end())
        {
            return nullptr;
        }

        return iter->second();
    }
}
