
# Setting required modules/libraries
set(LIBRARIES
    HGUtils
    HGRenderingBase
    HGPhysicsBase
    pthread
)

# Setting additional includes
set(INCLUDE)

# Creating Core module
describe_module(Core "${LIBRARIES}" "${INCLUDE}")