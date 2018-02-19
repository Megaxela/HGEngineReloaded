cmake_minimum_required(VERSION 3.9)
project(HGRenderingOGL)

set(CMAKE_CXX_STANDARD 17)

add_library(HGRenderingOGL STATIC
        src/Shader.cpp
        include/Shader.hpp
        include/GL/gl_ordered.hpp
)

target_include_directories(HGRenderingOGL PUBLIC
        include
)

#target_link_libraries(HGRenderingOGL
#         # DoubleBufferContainer
#)

target_compile_definitions(HGRenderingOGL
        PUBLIC -DOGL_RENDERING_MODULE_NS=HG::Rendering::OpenGL
)