cmake_minimum_required(VERSION 3.9)
project(HGCore)

set(CMAKE_CXX_STANDARD 17)

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
)

target_include_directories(HGCore PUBLIC
        include
)

target_link_libraries(HGCore
        HGUtils # DoubleBufferContainer
        HGRenderingBase #
)

target_compile_definitions(HGCore
        PUBLIC -DCORE_MODULE_NS=HG::Core
)