
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

# Creating RenderingBase module
describe_module(RenderingBase "${LIBRARIES}" "${INCLUDE}")

# Adding definition if added as dependency
target_compile_definitions(HGRenderingBase PUBLIC
    HG_HAS_RENDERING_BASE
)