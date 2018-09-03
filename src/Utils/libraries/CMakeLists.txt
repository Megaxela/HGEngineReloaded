cmake_minimum_required(VERSION 3.9)

add_subdirectory(ALogger)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(rapidxml)
add_subdirectory(ByteArray)
add_subdirectory(RingBuffer)

# Setting up assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS Off)
set(BUILD_SHARED_LIBS Off)
set(ASSIMP_BUILD_TESTS Off)
add_subdirectory(assimp)