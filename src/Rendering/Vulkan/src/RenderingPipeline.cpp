// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/SystemController.hpp>

// HG::Rendering::Vulkan
#include <HG/Rendering/Vulkan/RenderingPipeline.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// Vulkan
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

namespace {
    struct RenderingPipelineImpl
    {
        ~RenderingPipelineImpl() {
            // todo: Delete surface here
        }

        vk::UniqueInstance instance;
        vk::PhysicalDevice gpu;
        vk::UniqueDevice device;
        vk::PhysicalDeviceProperties gpuProperties;
        vk::Queue queue;
        vk::SurfaceKHR surface;

        std::vector<const char*> instanceLayers;
        std::vector<const char*> instanceExtensions;
        std::vector<const char*> deviceLayers;
        std::vector<const char*> deviceExtensions;

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;
        vk::UniqueDebugUtilsMessengerEXT debugMessenger;
    };

    RenderingPipelineImpl& getImpl(void* impl)
    {
        return *static_cast<RenderingPipelineImpl*>(impl);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,
                                                 VkDebugReportObjectTypeEXT objectType,
                                                 std::uint64_t sourceObject,
                                                 std::size_t location,
                                                 std::int32_t messageCode,
                                                 const char* layerPrefix,
                                                 const char* message,
                                                 void* userData)
    {
        auto formattedMessage = fmt::format("Vulkan Debug: [{}] {}", layerPrefix, message);

        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            HGError(formattedMessage);
        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
            HGWarning(formattedMessage);
        } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
            HGInfo(formattedMessage);
        } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            HGInfo(formattedMessage);
        }

        return false;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL messengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                     void* userData)
    {
        auto messageBase = fmt::format(
            "[t: {}, id: ({}, {})]",
            vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType)),
            callbackData->messageIdNumber,
            callbackData->pMessageIdName
        );

        auto mainMessage = fmt::format("{}: {}", messageBase, callbackData->pMessage);

        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            HGError(mainMessage);
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            HGWarning(mainMessage);
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            HGInfo(mainMessage);
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            HGDebug(mainMessage);
        }

        // queues
        for (std::uint32_t i = 0; i < callbackData->queueLabelCount; ++i) {
            auto queueMessage = fmt::format(
                "{} Queue N{} '{}'",
                messageBase,
                i + 1,
                callbackData->pQueueLabels[i].pLabelName
            );

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                HGError(queueMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                HGWarning(queueMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                HGInfo(queueMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
                HGDebug(queueMessage);
            }
        }

        // cmd buffers
        for (std::uint32_t i = 0; i < callbackData->cmdBufLabelCount; ++i)
        {
            auto cmdBufferMessage = fmt::format(
                "{} Command Buffer N{} '{}'",
                messageBase,
                i + 1,
                callbackData->pCmdBufLabels[i].pLabelName
            );

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                HGError(cmdBufferMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                HGWarning(cmdBufferMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                HGInfo(cmdBufferMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
                HGDebug(cmdBufferMessage);
            }
        }

        // objects
        for (std::uint32_t i = 0; i < callbackData->objectCount; ++i)
        {
            const auto& obj = callbackData->pObjects[i];

            auto objectMessage = fmt::format(
                "{} Object N{} type: '{}', handle: {}, name: '{}'",
                messageBase,
                i + 1,
                vk::to_string(static_cast<vk::ObjectType>(obj.objectType)),
                obj.objectHandle,
                obj.pObjectName ? obj.pObjectName : "<unknown>"
            );

            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                HGError(objectMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                HGWarning(objectMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                HGInfo(objectMessage);
            } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
                HGDebug(objectMessage);
            }
        }

        return true;
    }
}

namespace HG::Rendering::Vulkan {
    RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
        HG::Rendering::Base::RenderingPipeline(application),
        m_impl(nullptr)
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

        m_impl = new RenderingPipelineImpl;

        try
        {
            setupDebug();
            initVulkanDispatcher();
            initVulkanInstance();
            initDebug();
            createPhysicalDevice();
            HGInfo("Initialization finished successfully");
        }
        catch (const vk::SystemError& exc)
        {
            HGError("Unable to init: {} ({})", exc.code().value(), exc.what());
            deinit();
        }
        catch (const std::exception& exc)
        {
            HGError("Unable to init: {}", exc.what());
            deinit();
        }

        return m_impl != nullptr;
    }

    void RenderingPipeline::deinit()
    {
        auto& impl = getImpl(m_impl);

        HGInfo("Deinitializing");
        delete &impl; // hacks
        m_impl = nullptr;

        return HG::Rendering::Base::RenderingPipeline::deinit();
    }

    void RenderingPipeline::initVulkanDispatcher()
    {
        static vk::DynamicLoader dynamicLoader;
        auto vkGetInstanceProcAddr =
            dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
        HGInfo("Vulkan dispatcher initialized");
    }

    void RenderingPipeline::initVulkanInstance()
    {
        auto& impl = getImpl(m_impl);

        vk::ApplicationInfo applicationInfo {
            .pApplicationName = "HGEngine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "HGEngine",
            .apiVersion = VK_API_VERSION_1_2
        };

        vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
            vk::InstanceCreateInfo{
                .pApplicationInfo = &applicationInfo,
                .enabledLayerCount = static_cast<std::uint32_t>(impl.instanceLayers.size()),
                .ppEnabledLayerNames = impl.instanceLayers.data(),
                .enabledExtensionCount = static_cast<std::uint32_t>(impl.instanceExtensions.size()),
                .ppEnabledExtensionNames = impl.instanceExtensions.data()
            },
            impl.debugUtilsMessengerCreateInfo
        );

        impl.instance = vk::createInstanceUnique(instanceCreateInfo.get<vk::InstanceCreateInfo>());

        VULKAN_HPP_DEFAULT_DISPATCHER.init(*impl.instance);

        if (const auto& pipelineSetup = application()->systemController()->systemControllerPipelineSetupCallback())
        {
            pipelineSetup(&impl.instance, &impl.surface);
        }

        HGInfo("Instance initialized");
    }

    void RenderingPipeline::createPhysicalDevice()
    {
        auto& impl = getImpl(m_impl);

        {
            auto devices = impl.instance->enumeratePhysicalDevices();

            if (devices.empty())
            {
                throw std::runtime_error("No physical devices available");
            }

            HGInfo("Found {} physical devices", devices.size());

            impl.gpu = devices[0];
            impl.gpuProperties = impl.gpu.getProperties();

            HGInfo("Selected \"{}\" gpu", impl.gpuProperties.deviceName);
        }

        std::uint32_t graphicsQueueIndex = 0;
        {
            HGInfo("Detecting queue families");
            auto properties = impl.gpu.getQueueFamilyProperties();

            bool found = false;
            for (std::uint32_t i = 0; i < properties.size(); ++i)
            {
                auto& queueProperties = properties[i];
                HGInfo("Queue [index: {}, flags: {:b}, count: {}]",
                       i + 1,
                       std::uint32_t(queueProperties.queueFlags),
                       queueProperties.queueCount);
                if (queueProperties.queueFlags & vk::QueueFlagBits::eGraphics)
                {
                    found = true;
                    graphicsQueueIndex = i;
                }
            }

            if (!found)
            {
                throw std::runtime_error("Unable to find queue for graphics");
            }
        }

        {
            HGInfo("Enumerating instance layers");
            auto layerProperties = vk::enumerateInstanceLayerProperties();

            for (const auto& property : layerProperties) {
                HGInfo("Instance Layer [name: \"{}\", desk: \"{}\", implVer: {}.{}.{}, specVer: {}.{}.{}]",
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


        {
            HGInfo("Enumerating device layers");
            auto layerProperties = impl.gpu.enumerateDeviceLayerProperties();
            for (const auto& property : layerProperties) {
                HGInfo("Device Layer [name: \"{}\", desk: \"{}\", implVer: {}.{}.{}, specVer: {}.{}.{}]",
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

        vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
            .queueFamilyIndex = graphicsQueueIndex,
            .queueCount = 1,
            .pQueuePriorities = queuePriorities.data()
        };

        vk::DeviceCreateInfo deviceCreateInfo {
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &deviceQueueCreateInfo,
            .enabledLayerCount = static_cast<std::uint32_t>(impl.deviceLayers.size()),
            .ppEnabledLayerNames = impl.deviceLayers.data(),
            .enabledExtensionCount = static_cast<std::uint32_t>(impl.deviceExtensions.size()),
            .ppEnabledExtensionNames = impl.deviceExtensions.data()
        };

        impl.device = impl.gpu.createDeviceUnique(deviceCreateInfo);

        HGInfo("Device created");

        impl.queue = impl.device->getQueue(graphicsQueueIndex, 0);
    }

    void RenderingPipeline::setupDebug()
    {
        auto& impl = getImpl(m_impl);
        impl.instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
        impl.instanceLayers.push_back("VK_LAYER_KHRONOS_validation");

        impl.deviceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
        impl.deviceLayers.push_back("VK_LAYER_KHRONOS_validation");

        impl.instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        if (const auto& pipeline_setup = application()->systemController()->pipelineSetupCallback())
        {
            pipeline_setup(&impl.instanceExtensions);
        }

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        );

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        );

        impl.debugUtilsMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT{
            .messageSeverity = severityFlags,
            .messageType = messageTypeFlags,
            .pfnUserCallback = messengerCallback,
        };
    }

    void RenderingPipeline::initDebug()
    {
        auto& impl = getImpl(m_impl);

        impl.debugMessenger = impl.instance->createDebugUtilsMessengerEXTUnique(impl.debugUtilsMessengerCreateInfo);
    }

    const std::string& RenderingPipeline::pipelineName() const
    {
        static std::string name = "Vulkan";
        return name;
    }

    RenderingPipeline::Backend RenderingPipeline::pipelineBackend() const
    {
        return HG::Rendering::Base::RenderingPipeline::Backend::Vulkan;
    }
}