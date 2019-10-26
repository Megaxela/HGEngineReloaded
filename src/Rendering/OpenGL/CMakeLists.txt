set(OpenGL_GL_PREFERENCE LEGACY)

if (WIN32)
    set(${GDI_LIBRARY}
            gdi32)
endif()

describe_module(
    NAME RenderingOGL
    DEPENDENCIES
        ${GDI_LIBRARY}
        gl
        HGCore
        HGRenderingBase
)
