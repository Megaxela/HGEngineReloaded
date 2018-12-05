# Searching for zlib
find_package(ZLIB REQUIRED)

# Setting required modules/libraries
set(LIBRARIES
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
)

# Setting additional includes
set(INCLUDE
    ${ZLIB_INCLUDE_DIRS}
)

# Creating Utils module
describe_module(Utils "${LIBRARIES}" "${INCLUDE}")

# Addeing definitions
target_compile_definitions(HGUtils PUBLIC
    -DGLM_ENABLE_EXPERIMENTAL
    HG_HAS_UTILS=1
)