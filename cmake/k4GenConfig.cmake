
include(CMakeFindDependencyMacro)
find_dependency(podio REQUIRED)
find_dependency(Gaudi REQUIRED)

# - Include the targets file to create the imported targets that a client can
# link to (libraries) or execute (programs)
include("${CMAKE_CURRENT_LIST_DIR}/k4GenTargets.cmake")

get_property(TEST_TARGET_LIBRARY TARGET k4Gen::k4Gen PROPERTY LOCATION)
find_package_handle_standard_args(k4Gen  DEFAULT_MSG CMAKE_CURRENT_LIST_FILE TEST_TARGET_LIBRARY)
