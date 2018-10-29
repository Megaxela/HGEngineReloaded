cmake_minimum_required(VERSION 3.9)

# Creating base physics module
describe_module(PhysicsBase
    "HGCore"
    ""
)

# Adding definitions if base used as dependency
target_compile_definitions(HGPhysicsBase PUBLIC
    HG_HAS_PHYSICS_BASE=1
)