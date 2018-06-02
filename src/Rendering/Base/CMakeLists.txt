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
)

target_include_directories(HGRenderingBase PUBLIC
        include
)

target_link_libraries(HGRenderingBase
        HGCore
        HGUtils
        imgui
)

target_compile_definitions(HGRenderingBase
        PUBLIC -DRENDERING_BASE_MODULE_NS=HG::Rendering::Base
)