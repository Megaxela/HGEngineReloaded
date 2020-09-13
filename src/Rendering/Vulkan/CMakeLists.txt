find_package(Vulkan)

if (NOT VULKAN_FOUND)
    message(FATAL_ERROR "Vulkan not found")
endif()

describe_module(
    NAME RenderingVulkan
    DEPENDENCIES
        HGCore
        HGRenderingBase
        ${VULKAN_LIBRARY}
    INCLUDE
        ${VULKAN_INCLUDE_DIR}
)