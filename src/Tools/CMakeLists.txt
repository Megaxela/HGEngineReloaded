add_subdirectory(ToolsCore)

if (${HG_BUILD_TOOLS})
    add_subdirectory(PackageProcessorLibrary)
    add_subdirectory(PackageProcessor)
endif()
