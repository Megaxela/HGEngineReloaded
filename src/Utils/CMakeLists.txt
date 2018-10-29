cmake_minimum_required(VERSION 3.9)

# Adding libraries
add_subdirectory(libraries)

find_package(ZLIB REQUIRED)

# Creating Utils module
describe_module(Utils
    "ALogger;glm;stb;rapidxml;bytearray;stdc++fs;STLRoundBuffer;nlohmann_json;assimp;${ZLIB_LIBRARIES}"
    ${ZLIB_INCLUDE_DIRS}
)

# Addeing definitions
target_compile_definitions(HGUtils PUBLIC
    -DGLM_ENABLE_EXPERIMENTAL
    HG_HAS_UTILS=1
)