cmake_minimum_required(VERSION 3.9)
project(HGRenderingOGL)

set(CMAKE_CXX_STANDARD 17)

add_library(HGRenderingOGL STATIC

)

target_include_directories(HGRenderingOGL PUBLIC
        include
)

#target_link_libraries(HGRenderingOGL
#         # UniqueMergeContainer
#)

target_compile_definitions(HGRenderingOGL
        PUBLIC -DBASE_RENDERING_MODULE_NS=HG::Rendering::OpenGL
)