
# Setting required modules/libraries
set(LIBRARIES
    PlayRho
    HGCore
    HGRenderingBase
)

# Settind additional includes
set(INCLUDE)

# Creating PhysicsPlayRho module
describe_module(PhysicsPlayRho "${LIBRARIES}" "${INCLUDE}")

# Adding definition if added as dependency
target_compile_definitions(HGPhysicsPlayRho PUBLIC
    HG_HAS_PHYSICS_PLAYRHO=1
)