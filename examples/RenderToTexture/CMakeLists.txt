cmake_minimum_required(VERSION 3.9)
project(HGExampleRenderToTexture)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleRenderToTexture
    main.cpp
    Assets/Scenes/RenderToTextureScene.cpp
    Assets/Scenes/RenderToTextureScene.hpp
    Assets/Materials/TextureMaterial.cpp
    Assets/Materials/TextureMaterial.hpp
    Assets/Behaviours/RenderToTextureBehaviour.cpp
    Assets/Behaviours/RenderToTextureBehaviour.hpp
)

target_include_directories(HGExampleRenderToTexture PRIVATE .)

target_link_libraries(HGExampleRenderToTexture
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/Models/cube.obj Assets/Models/cube.obj COPYONLY)
configure_file(Assets/Models/cube.mtl Assets/Models/cube.mtl COPYONLY)
configure_file(Assets/Textures/container2.png Assets/Textures/container2.png COPYONLY)