cmake_minimum_required(VERSION 3.9)
project(HGRenderingOGL)

set(CMAKE_CXX_STANDARD 17)

option(HGRenderingOGL_USE_GLFW "Using GLFW as OpenGL library." On)
option(HGRenderingOGL_USE_Qt   "Using Qt as OpenGL library." Off)

if (${HGRenderingOGL_USE_GLFW} AND ${HGRenderingOGL_USE_Qt})
    message(FATAL_ERROR "You can use only 1 opengl providing library.")
endif()

if (${HGRenderingOGL_USE_GLFW})
    message("Rendering library: GLFW")
    set(RENDERER_DEFINITION GRAPHICS_USE_GLFW)
    set(RENDERER_LIBRARY glfw3)

elseif(${HGRenderingOGL_USE_Qt})
    message("Rendering library: Qt")
    set(RENDERER_DEFINITION GRAPHICS_USE_QT)

else()
    message(FATAL_ERROR "You must select 1 opengl providing library.")
endif()

add_subdirectory(libraries)

add_library(HGRenderingOGL STATIC
        include/GL/gl_ordered.hpp
        src/ForwardRenderingPipeline.cpp
        include/ForwardRenderingPipeline.hpp
        src/GLFWSystemController.cpp
        include/GLFWSystemController.hpp
)

target_include_directories(HGRenderingOGL PUBLIC
        include
)

set_target_properties(HGRenderingOGL PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(HGRenderingOGL
         # DoubleBufferContainer
        gl
        HGRenderingBase
        ${RENDERER_LIBRARY}
)

target_compile_definitions(HGRenderingOGL
        PUBLIC
        -DOGL_RENDERING_MODULE_NS=HG::Rendering::OpenGL
        -D${RENDERER_DEFINITION}
)