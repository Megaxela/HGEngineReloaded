cmake_minimum_required(VERSION 3.9)

add_subdirectory(ALogger)

# Setting up glm build
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# Setting up json
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

# Setting up assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS         OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS        OFF CACHE BOOL "" FORCE)

add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(rapidxml)
add_subdirectory(ByteArray)
add_subdirectory(RingBuffer)
add_subdirectory(json)
add_subdirectory(assimp)