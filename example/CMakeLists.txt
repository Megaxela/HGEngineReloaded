cmake_minimum_required(VERSION 3.9)
project(Example1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Example1
        main.cpp
)

target_include_directories(Example1 PRIVATE
        include
)

target_link_libraries(Example1
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/container2.png Assets/container2.png COPYONLY)
configure_file(Assets/Skybox/back.png   Assets/Skybox/back.png COPYONLY)
configure_file(Assets/Skybox/bottom.png Assets/Skybox/bottom.png COPYONLY)
configure_file(Assets/Skybox/front.png  Assets/Skybox/front.png COPYONLY)
configure_file(Assets/Skybox/left.png   Assets/Skybox/left.png COPYONLY)
configure_file(Assets/Skybox/right.png  Assets/Skybox/right.png COPYONLY)
configure_file(Assets/Skybox/top.png    Assets/Skybox/top.png COPYONLY)