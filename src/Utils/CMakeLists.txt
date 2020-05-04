# Searching for zlib
find_package(ZLIB REQUIRED)

# Windows fix
if (UNIX AND NOT EMSCRIPTEN)
    set(STD_FS_LIBRARY stdc++fs)
endif()

describe_module(
    NAME Utils
    INCLUDE
        ${ZLIB_INCLUDE_DIRS}

    DEPENDENCIES
        spdlog
        glm
        stb
        rapidxml
        bytearray
        ringbuffer
        nlohmann_json
        assimp
        fmt
        ${ZLIB_LIBRARIES}
        ${STD_FS_LIBRARY}

    DEFINITIONS
        -DGLM_ENABLE_EXPERIMENTAL
)
