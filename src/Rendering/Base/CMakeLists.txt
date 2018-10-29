cmake_minimum_required(VERSION 3.9)
project(HGRenderingBase)

set(CMAKE_CXX_STANDARD 17)

set(RENDER_OPENGL On)
add_subdirectory(libraries)

add_library(HGRenderingBase STATIC
        include/HG/Rendering/Base/Camera.hpp
    src/Camera.cpp
        include/HG/Rendering/Base/Lights/AbstractLight.hpp
    src/Lights/AbstractLight.cpp
        include/HG/Rendering/Base/Renderer.hpp
    src/Renderer.cpp
        include/HG/Rendering/Base/RenderingPipeline.hpp
    src/RenderingPipeline.cpp
    src/RenderBehaviour.cpp
        include/HG/Rendering/Base/RenderBehaviour.hpp
    src/Behaviours/Mesh.cpp
        include/HG/Rendering/Base/Behaviours/Mesh.hpp
    src/SystemController.cpp
        include/HG/Rendering/Base/SystemController.hpp
    src/Material.cpp
        include/HG/Rendering/Base/Material.hpp
    src/Texture.cpp
        include/HG/Rendering/Base/Texture.hpp
    src/ShaderData.cpp
        include/HG/Rendering/Base/Shader.hpp
    src/Lights/PointLight.cpp
        include/HG/Rendering/Base/Lights/PointLight.hpp
    src/Lights/DirectionalLight.cpp
        include/HG/Rendering/Base/Lights/DirectionalLight.hpp
    src/Behaviours/CubeMap.cpp
        include/HG/Rendering/Base/Behaviours/CubeMap.hpp
        include/HG/Rendering/Base/CubeMap.hpp
    src/CubeMap.cpp
    src/Behaviours/Sprite.cpp
        include/HG/Rendering/Base/Behaviours/Sprite.hpp
    src/Gizmos.cpp
        include/HG/Rendering/Base/Gizmos.hpp
    src/MaterialCollection.cpp
        include/HG/Rendering/Base/MaterialCollection.hpp
        include/HG/Rendering/Base/RenderSpecificData.hpp
    src/RenderData.cpp
        include/HG/Rendering/Base/RenderData.hpp
    src/AbstractRenderDataProcessor.cpp
        include/HG/Rendering/Base/AbstractRenderDataProcessor.hpp
    src/MaterialValue.cpp
        include/HG/Rendering/Base/MaterialValue.hpp
    src/RenderTarget.cpp
        include/HG/Rendering/Base/RenderTarget.hpp
)

target_include_directories(HGRenderingBase PUBLIC
    include
)

target_link_libraries(HGRenderingBase
    HGCore
    HGUtils
    imgui
)

target_compile_definitions(HGRenderingBase PUBLIC
    HG_HAS_RENDERING_BASE
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGRenderingBase
        PRIVATE
            -ansi
            -pedantic
            -Wall
            -Wextra
            -Woverloaded-virtual
            -Winit-self
            -Wunreachable-code
    )
endif()