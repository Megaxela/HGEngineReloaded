# Setting required modules/libraries
set(LIBRARIES
    HGCore
    HGUtils
    HGPhysicsBase
    ALogger
)

# Setting additional includes
set(INCLUDE)

# Creating Standard module
describe_module(Standard "${LIBRARIES}" "${INCLUDE}")

# Adding definition if added as dependency
target_compile_definitions(HGStandard PUBLIC
    HG_HAS_STANDARD
)