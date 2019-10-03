set(OpenGL_GL_PREFERENCE LEGACY)

message(STATUS "Rendering library: GLFW")
set(RENDERER_DEFINITION GRAPHICS_USE_GLFW)

if (WIN32)
    set(${GDI_LIBRARY}
            gdi32)
endif()

describe_module(
    NAME RenderingOGL
    INCLUDE

    DEPENDENCIES
        ${GDI_LIBRARY}
        gl
        HGCore
        HGRenderingBase
        glfw

    DEFINITIONS
        ${RENDERER_DEFINITION}=1
)
