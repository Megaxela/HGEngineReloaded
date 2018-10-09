cmake_minimum_required(VERSION 3.9)
project(HGExampleTransparent)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleTransparent
    main.cpp
    Assets/Scenes/TransparentScene.cpp
    Assets/Scenes/TransparentScene.hpp
    Assets/Materials/GlassMaterial.cpp
    Assets/Materials/GlassMaterial.hpp
)

target_include_directories(HGExampleTransparent PRIVATE .)

target_link_libraries(HGExampleTransparent
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/Models/bottle.obj Assets/Models/bottle.obj COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_ft.JPG Assets/Cubemaps/Lake/lake2_ft.JPG COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_bk.JPG Assets/Cubemaps/Lake/lake2_bk.JPG COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_up.JPG Assets/Cubemaps/Lake/lake2_up.JPG COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_dn.JPG Assets/Cubemaps/Lake/lake2_dn.JPG COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_lf.JPG Assets/Cubemaps/Lake/lake2_lf.JPG COPYONLY)
configure_file(Assets/Cubemaps/Lake/lake2_rt.JPG Assets/Cubemaps/Lake/lake2_rt.JPG COPYONLY)