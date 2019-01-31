cmake_minimum_required(VERSION 3.9)

# Setting up glm build
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# Setting up json
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

# Setting up assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS         OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS        OFF CACHE BOOL "" FORCE)

add_dependency_subdirectory(PATH ALogger    )
add_dependency_subdirectory(PATH glm        )
add_dependency_subdirectory(PATH stb        )
add_dependency_subdirectory(PATH rapidxml   )
add_dependency_subdirectory(PATH ByteArray  TARGET bytearray)
add_dependency_subdirectory(PATH RingBuffer TARGET STLRoundBuffer)
add_dependency_subdirectory(PATH json       TARGET nlohmann_json)
add_dependency_subdirectory(PATH assimp     )