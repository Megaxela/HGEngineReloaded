cmake_minimum_required(VERSION 3.9)

# Rendering system selection mode
option(HGRenderingOGL_USE_GLFW "Using GLFW as OpenGL library." Off)
option(HGRenderingOGL_USE_Qt   "Using Qt as OpenGL library." Off)

if (${HGRenderingOGL_USE_GLFW} AND ${HGRenderingOGL_USE_Qt})
    message(FATAL_ERROR "You can use only 1 opengl providing library.")
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

if (${HGRenderingOGL_USE_GLFW})
    message(STATUS "Rendering library: GLFW")
    set(RENDERER_DEFINITION GRAPHICS_USE_GLFW)

    find_package(GLFW REQUIRED)

    set(RENDERER_LIBRARY ${GLFW_LIBRARY})
    set(RENDERER_INCLUDE ${GLFW_INCLUDE_DIR})

elseif(${HGRenderingOGL_USE_Qt})

    message(STATUS "Rendering library: Qt")
    set(RENDERER_DEFINITION GRAPHICS_USE_QT)

else()
    message(STATUS "Available renderers:")
    message(STATUS "    HGRenderingOGL_USE_GLFW")
    message(STATUS "    HGRenderingOGL_USE_Qt")
    message(FATAL_ERROR "You must select 1 opengl providing library.")
endif()

# Including libraries
add_subdirectory(libraries)

# Creating RenderingOGL module.
describe_module(RenderingOGL
    "gl;HGRenderingBase;HGCore;${RENDERER_LIBRARY}"
    "${RENDERER_INCLUDE}"
)

# Setting, that it's C++ code
set_target_properties(HGRenderingOGL PROPERTIES LINKER_LANGUAGE CXX)

# Adding definition FFS
target_compile_definitions(HGRenderingOGL PUBLIC
    ${RENDERER_DEFINITION}=1
    HG_HAS_RENDERING_OGL=1
)