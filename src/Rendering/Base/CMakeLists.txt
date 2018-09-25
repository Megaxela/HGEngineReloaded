cmake_minimum_required(VERSION 3.9)
project(HGRenderingBase)

set(CMAKE_CXX_STANDARD 17)

set(RENDER_OPENGL On)
add_subdirectory(libraries)

add_library(HGRenderingBase STATIC
    include/Camera.hpp
    src/Camera.cpp
    include/Lights/AbstractLight.hpp
    src/Lights/AbstractLight.cpp
    include/Renderer.hpp
    src/Renderer.cpp
    include/RenderingPipeline.hpp
    src/RenderingPipeline.cpp
    src/RenderBehaviour.cpp
    include/RenderBehaviour.hpp
    src/Behaviours/Mesh.cpp
    include/Behaviours/Mesh.hpp
    src/SystemController.cpp
    include/SystemController.hpp
    src/Material.cpp
    include/Material.hpp
    src/Texture.cpp
    include/Texture.hpp
    src/ShaderData.cpp
    include/Shader.hpp
    src/Lights/PointLight.cpp
    include/Lights/PointLight.hpp
    src/Lights/DirectionalLight.cpp
    include/Lights/DirectionalLight.hpp
    src/Behaviours/CubeMap.cpp
    include/Behaviours/CubeMap.hpp
    include/CubeMapTexture.hpp
    src/CubeMapTexture.cpp
    src/Behaviours/Sprite.cpp
    include/Behaviours/Sprite.hpp
    src/Gizmos.cpp
    include/Gizmos.hpp
    src/MaterialCollection.cpp
    include/MaterialCollection.hpp
    include/RenderSpecificData.hpp
    src/RenderData.cpp
    include/RenderData.hpp
    src/AbstractRenderDataProcessor.cpp
    include/AbstractRenderDataProcessor.hpp
    src/MaterialValue.cpp
    include/MaterialValue.hpp
    src/RenderTarget.cpp
    include/RenderTarget.hpp
)

target_include_directories(HGRenderingBase PUBLIC
    include
)

target_link_libraries(HGRenderingBase
    HGCore
    HGUtils
    imgui
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