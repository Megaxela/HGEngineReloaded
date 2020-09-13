// HG::Rendering::Vulkan
#include <HG/Rendering/Vulkan/RenderingPipeline.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace {
    struct RenderingPipelineImpl
    {
        VkInstance instance = nullptr;
        VkPhysicalDevice gpu = nullptr;
        VkDevice device = nullptr;
        VkPhysicalDeviceProperties gpu_properties;
    };

    RenderingPipelineImpl& getImpl(void* impl)
    {
        return *static_cast<RenderingPipelineImpl*>(impl);
    }
}

namespace HG::Rendering::Vulkan {
    RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
        HG::Rendering::Base::RenderingPipeline(application),
        m_impl(new RenderingPipelineImpl)
    {

    }

    void RenderingPipeline::clear(HG::Utils::Color color)
    {

    }

    void RenderingPipeline::render(const std::vector<HG::Core::GameObject*>& objects)
    {

    }

    bool RenderingPipeline::render(HG::Rendering::Base::RenderBehaviour* behaviour)
    {
        return true;
    }

    void RenderingPipeline::blit(HG::Rendering::Base::RenderTarget* target, HG::Rendering::Base::BlitData* blitData)
    {

    }

    void RenderingPipeline::getTextureRegion(HG::Rendering::Base::Texture* texture, glm::ivec2 tl, glm::ivec2 br,
                                             uint8_t* data)
    {

    }

    bool RenderingPipeline::init()
    {
        if (!HG::Rendering::Base::RenderingPipeline::init()) {
            return false;
        }

        initVulkanInstance();
        createPhysicalDevice();

        return true;
    }

    void RenderingPipeline::deinit()
    {
        auto& impl = getImpl(m_impl);

        vkDestroyDevice(impl.device, nullptr);
        vkDestroyInstance(impl.instance, nullptr);

        // to be sure
        impl.device = nullptr;
        impl.instance = nullptr;

        return HG::Rendering::Base::RenderingPipeline::deinit();
    }

    bool RenderingPipeline::initVulkanInstance()
    {
        auto& impl = getImpl(m_impl);

        std::uint32_t amount = 0;
        VkResult err = VK_SUCCESS;

        VkApplicationInfo applicationInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "HGEngine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_2,
        };

        VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &applicationInfo
        };

        if ((err = vkCreateInstance(&instanceCreateInfo, nullptr, &impl.instance)) != VK_SUCCESS)
        {
            HGError("Vulkan: Unable to create instance: {}", err);
            return false;
        }

        HGInfo("Vulkan: Initialized");
        return true;
    }

    bool RenderingPipeline::createPhysicalDevice()
    {
        auto& impl = getImpl(m_impl);

        std::uint32_t amount = 0;
        VkResult err = VK_SUCCESS;

        {
            // Creating physical device
            if ((err = vkEnumeratePhysicalDevices(impl.instance, &amount, nullptr)) != VK_SUCCESS)
            {
                HGError("Vulkan: Unable to enumerate physical devices: {}", err);
                return false;
            }

            if (amount == 0)
            {
                HGError("Vulkan: No physical devices available");
                return false;
            }

            HGInfo("Vulkan: Found {} physical devices", amount);

            std::vector<VkPhysicalDevice> devices(amount, nullptr);
            if ((err = vkEnumeratePhysicalDevices(impl.instance, &amount, devices.data())) != VK_SUCCESS)
            {
                HGError("Vulkan: Unable to list physical devices: {}", err);
                return false;
            }

            impl.gpu = devices[0];

            vkGetPhysicalDeviceProperties(impl.gpu, &impl.gpu_properties);

            HGInfo("Vulkan: Selected \"{}\" gpu", impl.gpu_properties.deviceName);
        }

        std::uint32_t graphicsQueueIndex = 0;
        {
            HGInfo("Vulkan: Detecting queue families");
            vkGetPhysicalDeviceQueueFamilyProperties(impl.gpu, &amount, nullptr);

            std::vector<VkQueueFamilyProperties> properties(amount, VkQueueFamilyProperties());
            vkGetPhysicalDeviceQueueFamilyProperties(impl.gpu, &amount, properties.data());

            bool found = false;
            for (std::uint32_t i = 0; i < amount; ++i)
            {
                auto& queueProperties = properties[i];
                HGInfo("Vulkan: Queue [index: {}, flags: {:b}, count: {}]", i, queueProperties.queueFlags, queueProperties.queueCount);
                if (queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    found = true;
                    graphicsQueueIndex = i;
                }
            }

            if (!found)
            {
                HGError("Vulkan: Unable to find queue for graphics");
                return false;
            }
        }

        {
            HGInfo("Vulkan: Enumerating instance layers");
            if ((err = vkEnumerateInstanceLayerProperties(&amount, nullptr)) != VK_SUCCESS)
            {
                HGError("Vulkan: Unable to enumerate instance layers: {}", err);
                return false;
            }

            std::vector<VkLayerProperties> layerProperties(amount, VkLayerProperties());
            if ((err = vkEnumerateInstanceLayerProperties(&amount, layerProperties.data())) != VK_SUCCESS)
            {
                HGError("Vulkan: Unable to list instance layers: {}", err);
                return false;
            }

            for (const auto& property : layerProperties) {
                HGInfo("Vulkan: Layer [name: \"{}\", desk: \"{}\", implVer: {}.{}.{}, specVer: {}.{}.{}]",
                       property.layerName,
                       property.description,
                       VK_VERSION_MAJOR(property.implementationVersion),
                       VK_VERSION_MINOR(property.implementationVersion),
                       VK_VERSION_PATCH(property.implementationVersion),
                       VK_VERSION_MAJOR(property.specVersion),
                       VK_VERSION_MINOR(property.specVersion),
                       VK_VERSION_PATCH(property.specVersion));
            }
        }

        std::array<float, 1> queuePriorities ={1.0f};

        VkDeviceQueueCreateInfo queueDeviceQueueCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = graphicsQueueIndex,
            .queueCount = 1,
            .pQueuePriorities = queuePriorities.data()
        };

        VkDeviceCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueDeviceQueueCreateInfo
        };

        if ((err = vkCreateDevice(impl.gpu, &createInfo, nullptr, &impl.device)) != VK_SUCCESS)
        {
            HGError("Vulkan: Unable to create device: {}", err);
            return false;
        }

        HGInfo("Vulkan: Device created");

        return true;
    }
}