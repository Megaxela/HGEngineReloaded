find_package(Vulkan)

if (VULKAN_FOUND)
    message(STATUS "Vulkan was found for PC system")
    set(SPECIFIC_LIBS ${VULKAN_LIBRARY})
    set(SPECIFIC_INCLUDES ${VULKAN_INCLUDE_DIR})
else()
    message(STATUS "Vulkan was not found for PC system")
endif()

describe_module(
    NAME SystemPC
    DEPENDENCIES
        HGCore
        HGUtils
        glfw
        ${SPECIFIC_LIBS}
    INCLUDE
        ${SPECIFIC_INCLUDES}
    DEFINITIONS
        SYSTEM_USE_PC=1
)
