
# Setting up libraries
set(RENDER_OPENGL On)

# Adding required libraries
set(LIBRARIES
    HGCore
    HGUtils
    imgui
)

# Adding additional include dirs
set(INCLUDE)

describe_module(
    NAME RenderingBase
    DEPENDENCIES
        imgui
        HGCore
        HGUtils

    DEFINITIONS
        HG_HAS_RENDERING_BASE
)