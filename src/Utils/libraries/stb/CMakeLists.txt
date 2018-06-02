add_library(stb STATIC
    stb_image.h
)

target_include_directories(stb PUBLIC
    .
)

set_target_properties(stb PROPERTIES LINKER_LANGUAGE CXX)
