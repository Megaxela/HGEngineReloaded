set(OpenGL_GL_PREFERENCE LEGACY)

message(STATUS "Rendering library: GLFW")
set(RENDERER_DEFINITION GRAPHICS_USE_GLFW)

#find_package(GLFW REQUIRED)
#
#set(RENDERER_LIBRARY ${GLFW_LIBRARY})
#set(RENDERER_INCLUDE ${GLFW_INCLUDE_DIR})


describe_module(
    NAME RenderingOGL
    INCLUDE

    DEPENDENCIES
        gl
        HGCore
        HGRenderingBase
        glfw

    DEFINITIONS
        ${RENDERER_DEFINITION}=1
)
