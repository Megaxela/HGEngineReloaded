
# Setting up libraries
set(RENDER_OPENGL On)

describe_module(
    NAME RenderingBase
    DEPENDENCIES
        imgui
        HGCore
        HGUtils
)