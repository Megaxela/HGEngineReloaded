set(LIBRARIES
    HGCore
)

set(INCLUDE)

# Creating base physics module
describe_module(PhysicsBase
    "${LIBRARIES}"
    "${INCLUDE}"
)

# Adding definitions if base used as dependency
target_compile_definitions(HGPhysicsBase PUBLIC
    HG_HAS_PHYSICS_BASE=1
)