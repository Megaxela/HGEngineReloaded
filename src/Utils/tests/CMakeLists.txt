set(HG_TEST_CASES
        ${HG_TEST_CASES}
        ${CMAKE_CURRENT_LIST_DIR}/color.cpp
        ${CMAKE_CURRENT_LIST_DIR}/base64.cpp
        ${CMAKE_CURRENT_LIST_DIR}/zlib.cpp

        CACHE STRING "" FORCE
)

set(HG_TEST_LIBS
        ${HG_TEST_LIBS}
        HGUtils
        CACHE STRING "" FORCE
)

set(HG_TEST_ASSETS
        ${HG_TEST_ASSETS}

        CACHE_STRING "" FORCE
)