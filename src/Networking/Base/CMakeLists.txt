if (WIN32)
    set(SYSTEM_DEPENDENCIES Ws2_32)
endif()

describe_module(
    NAME NetworkingBase
    DEPENDENCIES
        HGUtils
        HGCore
        ${SYSTEM_DEPENDENCIES}

)
