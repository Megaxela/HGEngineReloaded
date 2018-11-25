cmake_minimum_required(VERSION 3.9)
project(HGExampleBlitting)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleBlitting
    main.cpp
    Assets/Scenes/BlitScene.cpp
    Assets/Scenes/BlitScene.hpp
    Assets/Materials/TextureMaterial.cpp
    Assets/Materials/TextureMaterial.hpp
)

target_include_directories(HGExampleBlitting PRIVATE .)

target_link_libraries(HGExampleBlitting
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/Textures/atlas.png Assets/Textures/atlas.png COPYONLY)
configure_file(Assets/Models/cube.mtl Assets/Models/cube.mtl COPYONLY)
configure_file(Assets/Models/cube.obj Assets/Models/cube.obj COPYONLY)