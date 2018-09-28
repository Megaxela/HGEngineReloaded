cmake_minimum_required(VERSION 3.9)
project(HGRenderingOGL)

set(CMAKE_CXX_STANDARD 17)

option(HGRenderingOGL_USE_GLFW "Using GLFW as OpenGL library." Off)
option(HGRenderingOGL_USE_Qt   "Using Qt as OpenGL library." Off)

if (${HGRenderingOGL_USE_GLFW} AND ${HGRenderingOGL_USE_Qt})
    message(FATAL_ERROR "You can use only 1 opengl providing library.")
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

if (${HGRenderingOGL_USE_GLFW})
    message(STATUS "Rendering library: GLFW")
    set(RENDERER_DEFINITION GRAPHICS_USE_GLFW)

    if (WIN32)

        find_package(GLFW REQUIRED)

        set(RENDERER_LIBRARY ${GLFW_LIBRARY})
        set(RENDERER_INCLUDE ${GLFW_INCLUDE_DIR})
        
    else()
        set(RENDERER_LIBRARY glfw)
    endif()
#    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/../Base/libraries/GWork/cmake/Modules/")
#    find_package(GLFW REQUIRED)
#
#    set(RENDERER_LIBRARY ${GLFW_LIBRARIES} ${GLFW_DEPENDENCIES})

elseif(${HGRenderingOGL_USE_Qt})
    message(STATUS "Rendering library: Qt")
    set(RENDERER_DEFINITION GRAPHICS_USE_QT)

else()
    message(FATAL_ERROR "You must select 1 opengl providing library.")
endif()

add_subdirectory(libraries)

add_library(HGRenderingOGL STATIC
    include/GL/gl_ordered.hpp
    src/Forward/RenderingPipeline.cpp
    include/Forward/RenderingPipeline.hpp
    src/GLFWSystemController.cpp
    include/GLFWSystemController.hpp
    src/GizmosRenderer.cpp
    include/GizmosRenderer.hpp
    src/Materials/GizmosLineMaterial.cpp
    include/Materials/GizmosLineMaterial.hpp
    src/Materials/GizmosMeshMaterial.cpp
    include/Materials/GizmosMeshMaterial.hpp
    src/Materials/MeshFallbackMaterial.cpp
    include/Materials/MeshFallbackMaterial.hpp
    src/Materials/SpriteMaterial.cpp
    include/Materials/SpriteMaterial.hpp
    src/Forward/AbstractRenderer.cpp
    include/Forward/AbstractRenderer.hpp
    src/Forward/MeshRenderer.cpp
    include/Forward/MeshRenderer.hpp
    include/Common/ShaderData.hpp
    include/Common/MeshData.hpp
    include/Common/Texture2DData.hpp
    src/Forward/SpriteRenderer.cpp
    include/Forward/SpriteRenderer.hpp
    src/ImGuiRenderer.cpp
    include/ImGuiRenderer.hpp
    src/Materials/ImGuiMaterial.cpp
    include/Materials/ImGuiMaterial.hpp
    include/Common/CubeMapTextureData.hpp
    src/Common/MeshDataProcessor.cpp
    include/Common/MeshDataProcessor.hpp
    src/Common/ShaderDataProcessor.cpp
    include/Common/ShaderDataProcessor.hpp
    src/Common/Texture2DDataProcessor.cpp
    include/Common/Texture2DDataProcessor.hpp
    src/Common/CubeMapTextureDataProcessor.cpp
    include/Common/CubeMapTextureDataProcessor.hpp
    src/Deferred/RenderingPipeline.cpp
    include/Deferred/RenderingPipeline.hpp
    include/Common/RenderTargetData.hpp
    src/Common/RenderTargetDataProcessor.cpp
    include/Common/RenderTargetDataProcessor.hpp
    src/Forward/CubeMapRenderer.cpp
    include/Forward/CubeMapRenderer.hpp
    src/Materials/SkyboxMaterial.cpp 
    include/Materials/SkyboxMaterial.hpp
)

set_target_properties(HGRenderingOGL PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HGRenderingOGL
    gl
    HGRenderingBase
    HGCore
    ${RENDERER_LIBRARY}
)

target_include_directories(HGRenderingOGL PUBLIC
    include
    ${RENDERER_INCLUDE}
)

target_compile_definitions(HGRenderingOGL
    PUBLIC
    -D${RENDERER_DEFINITION}
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGRenderingOGL
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