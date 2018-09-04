cmake_minimum_required(VERSION 3.9)
project(Example1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Example1
    main.cpp
    Assets/Scenes/TestScene.cpp
    Assets/Scenes/TestScene.hpp
    Assets/Materials/LightMaterial.cpp
    Assets/Materials/LightMaterial.hpp
    Assets/Behaviours/RotationBehaviour.hpp
    Assets/Behaviours/LocalRotation.hpp
    Assets/Materials/PBRMaterial.cpp
    Assets/Materials/PBRMaterial.hpp
)

target_include_directories(Example1 PRIVATE .)

target_link_libraries(Example1
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/PBR/CopperRock/albedo.png Assets/PBR/CopperRock/albedo.png COPYONLY)
configure_file(Assets/PBR/CopperRock/ao.png Assets/PBR/CopperRock/ao.png COPYONLY)
configure_file(Assets/PBR/CopperRock/height.png Assets/PBR/CopperRock/height.png COPYONLY)
configure_file(Assets/PBR/CopperRock/metallic.png Assets/PBR/CopperRock/metallic.png COPYONLY)
configure_file(Assets/PBR/CopperRock/normal.png Assets/PBR/CopperRock/normal.png COPYONLY)
configure_file(Assets/PBR/CopperRock/roughness.png Assets/PBR/CopperRock/roughness.png COPYONLY)

configure_file(Assets/PBR/GreasyPan/albedo.png    Assets/PBR/GreasyPan/albedo.png    COPYONLY)
configure_file(Assets/PBR/GreasyPan/metallic.png  Assets/PBR/GreasyPan/metallic.png  COPYONLY)
configure_file(Assets/PBR/GreasyPan/normal.png    Assets/PBR/GreasyPan/normal.png    COPYONLY)
configure_file(Assets/PBR/GreasyPan/roughness.png Assets/PBR/GreasyPan/roughness.png COPYONLY)

configure_file(Assets/HDR/Tokyo_BigSight_3k.hdr Assets/HDR/Tokyo_BigSight_3k.hdr COPYONLY)
configure_file(Assets/Models/cube.obj Assets/Models/cube.obj COPYONLY)
configure_file(Assets/Models/cube.mtl Assets/Models/cube.mtl COPYONLY)
configure_file(Assets/Models/icosphere.obj Assets/Models/icosphere.obj COPYONLY)
configure_file(Assets/Models/icosphere.mtl Assets/Models/icosphere.mtl COPYONLY)
configure_file(Assets/Models/icosphere_hq.obj Assets/Models/icosphere_hq.obj COPYONLY)
configure_file(Assets/Models/icosphere_hq.mtl Assets/Models/icosphere_hq.mtl COPYONLY)
configure_file(Assets/Models/example.obj Assets/Models/example.obj COPYONLY)
configure_file(Assets/container2.png Assets/container2.png COPYONLY)
configure_file(Assets/Skybox/back.png   Assets/Skybox/back.png COPYONLY)
configure_file(Assets/Skybox/bottom.png Assets/Skybox/bottom.png COPYONLY)
configure_file(Assets/Skybox/front.png  Assets/Skybox/front.png COPYONLY)
configure_file(Assets/Skybox/left.png   Assets/Skybox/left.png COPYONLY)
configure_file(Assets/Skybox/right.png  Assets/Skybox/right.png COPYONLY)
configure_file(Assets/Skybox/top.png    Assets/Skybox/top.png COPYONLY)