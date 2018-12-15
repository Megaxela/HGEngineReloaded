# Searching for zlib
find_package(ZLIB REQUIRED)

describe_module(
    NAME Utils
    INCLUDE
        ${ZLIB_INCLUDE_DIRS}

    DEPENDENCIES
        ALogger
        glm
        stb
        rapidxml
        bytearray
        stdc++fs
        STLRoundBuffer
        nlohmann_json
        assimp
        ${ZLIB_LIBRARIES}

    DEFINITIONS
        -DGLM_ENABLE_EXPERIMENTAL
)
