cmake_minimum_required(VERSION 3.9)
project(HGExampleBlitting)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleBlitting
    main.cpp
    Assets/Scenes/BlitScene.cpp
    Assets/Scenes/BlitScene.hpp
)

target_include_directories(HGExampleBlitting PRIVATE .)

target_link_libraries(HGExampleBlitting
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)