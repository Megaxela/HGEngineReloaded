cmake_minimum_required(VERSION 3.9)
project(HGCore)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_CORE_TESTS "Build core tests" Off)

if (${HG_BUILD_CORE_TESTS})
    add_subdirectory(tests)
endif()

add_library(HGCore STATIC
    src/Application.cpp
    include/Application.hpp
    src/Scene.cpp
    include/Scene.hpp
    src/GameObject.cpp
    include/GameObject.hpp
    src/Behaviour.cpp
    include/Behaviour.hpp
    src/GameObjectBuilder.cpp
    include/GameObjectBuilder.hpp
    src/GameObjectCache.cpp
    include/GameObjectCache.hpp
    src/Transform.cpp
    include/Transform.hpp
    src/ResourceManager.cpp
    include/ResourceManager.hpp
    include/ResourceAccessor.hpp
    src/TimeStatistics.cpp
    include/TimeStatistics.hpp
    src/Input.cpp
    include/Input.hpp src/Data.cpp include/Data.hpp)

target_include_directories(HGCore PUBLIC
    include
)

target_link_libraries(HGCore
        HGUtils # DoubleBufferContainer
        HGRenderingBase #
        HGPhysicsBase
        pthread
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGCore
        PRIVATE
            -ansi
            -pedantic
            -Wall
            -Wextra
            -Woverloaded-virtual
            -Winit-self
            -Wunreachable-code
    )
endif()
