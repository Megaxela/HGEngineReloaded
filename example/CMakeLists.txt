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
configure_file(Assets/Skybox/back.jpg Assets/Skybox/back.jpg COPYONLY)
configure_file(Assets/Skybox/bottom.jpg Assets/Skybox/bottom.jpg COPYONLY)
configure_file(Assets/Skybox/front.jpg Assets/Skybox/front.jpg COPYONLY)
configure_file(Assets/Skybox/left.jpg Assets/Skybox/left.jpg COPYONLY)
configure_file(Assets/Skybox/right.jpg Assets/Skybox/right.jpg COPYONLY)
configure_file(Assets/Skybox/top.jpg Assets/Skybox/top.jpg COPYONLY)