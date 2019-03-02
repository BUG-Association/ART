# Set a default build type if none was supplied to cmake
SET(_default_build_type "Release")

# In case of git working copy, set build type to Debug
IF (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    SET(_default_build_type "Debug")
ENDIF()

# Now check if build type has been supplied to cmake, either directly or via a generator
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    #
    MESSAGE(STATUS "Missing build type, setting to '${_default_build_type}'.")
    SET(CMAKE_BUILD_TYPE "${_default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Support cmake-gui
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF()
