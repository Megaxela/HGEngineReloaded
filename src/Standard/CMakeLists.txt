cmake_minimum_required(VERSION 3.9)

# Creating Standard module
describe_module(Standard
    "HGCore;HGUtils;HGPhysicsBase;ALogger"
    ""
)

# Adding definition if added as dependency
target_compile_definitions(HGStandard PUBLIC
    HG_HAS_STANDARD
)