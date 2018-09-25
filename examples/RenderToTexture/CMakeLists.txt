cmake_minimum_required(VERSION 3.9)
project(HGExampleRenderToTexture)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleRenderToTexture
    main.cpp
        Assets/Scenes/RenderToTextureScene.cpp Assets/Scenes/RenderToTextureScene.hpp Assets/Materials/TextureMaterial.cpp Assets/Materials/TextureMaterial.hpp)

target_include_directories(HGExampleRenderToTexture PRIVATE .)

target_link_libraries(HGExampleRenderToTexture
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)