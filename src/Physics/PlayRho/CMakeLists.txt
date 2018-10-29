cmake_minimum_required(VERSION 3.9)

# Including libraries
add_subdirectory(libraries)

# Creating PhysicsPlayRho module
describe_module(PhysicsPlayRho "PlayRho;HGCore;HGRenderingBase")

# Adding definition if added as dependency
target_compile_definitions(HGPhysicsPlayRho PUBLIC
    HG_HAS_PHYSICS_PLAYRHO=1
)