cmake_minimum_required(VERSION 3.9)
project(HGCore)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_CORE_TESTS "Build core tests" Off)

set(CMAKE_CXX_FLAGS -fsanitize=address)

if (${HG_BUILD_CORE_TESTS})
    add_subdirectory(tests)
endif()

add_library(HGCore STATIC
    src/Application.cpp
        include/HG/Core/Application.hpp
    src/Scene.cpp
        include/HG/Core/Scene.hpp
    src/GameObject.cpp
        include/HG/Core/GameObject.hpp
    src/Behaviour.cpp
        include/HG/Core/Behaviour.hpp
    src/GameObjectBuilder.cpp
        include/HG/Core/GameObjectBuilder.hpp
    src/GameObjectCache.cpp
        include/HG/Core/GameObjectCache.hpp
    src/Transform.cpp
        include/HG/Core/Transform.hpp
    src/ResourceManager.cpp
        include/HG/Core/ResourceManager.hpp
        include/HG/Core/ResourceAccessor.hpp
    src/TimeStatistics.cpp
        include/HG/Core/TimeStatistics.hpp
    src/Input.cpp
        include/HG/Core/Input.hpp
        include/HG/Core/Data.hpp
    src/ThreadPool.cpp
        include/HG/Core/ThreadPool.hpp
    src/CountStatistics.cpp
        include/HG/Core/CountStatistics.hpp
        include/HG/Core/BuildProperties.hpp
    src/Logging.cpp
        include/HG/Core/Logging.hpp
)

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
