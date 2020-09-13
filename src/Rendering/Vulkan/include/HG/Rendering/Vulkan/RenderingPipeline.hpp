#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderingPipeline.hpp>

namespace HG::Core
{
class Application;
}

namespace HG::Rendering::Vulkan
{
    /**
     * @brief Class, that describes Vulkan default rendering pipeline.
     */
    class RenderingPipeline : public HG::Rendering::Base::RenderingPipeline
    {
    public:
        /**
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit RenderingPipeline(HG::Core::Application* application);

        bool init() override;

        void clear(HG::Utils::Color color) override;

        void render(const std::vector<HG::Core::GameObject*>& objects) override;

        bool render(HG::Rendering::Base::RenderBehaviour* behaviour) override;

        void blit(HG::Rendering::Base::RenderTarget* target, HG::Rendering::Base::BlitData* blitData) override;

        void getTextureRegion(HG::Rendering::Base::Texture* texture, glm::ivec2 tl, glm::ivec2 br, uint8_t* data) override;

        void deinit() override;

    private:
        bool initVulkanInstance();
        bool createPhysicalDevice();

        void* m_impl;
    };
}


