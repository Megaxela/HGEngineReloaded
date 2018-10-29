cmake_minimum_required(VERSION 3.9)

# Setting up libraries
set(RENDER_OPENGL On)

# Including libraries
add_subdirectory(libraries)

# Creating RenderingBase module
describe_module(RenderingBase
    "HGCore;HGUtils;imgui"
    ""
)

# Adding definition if added as dependency
target_compile_definitions(HGRenderingBase PUBLIC
    HG_HAS_RENDERING_BASE
)