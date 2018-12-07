
# Setting required modules/libraries
set(LIBRARIES
    HGUtils
)

# Setting additional includes
set(INCLUDE)

# Creating PackageProcessorLibrary module
describe_module(PackageProcessorLibrary "${LIBRARIES}" "${INCLUDE}")