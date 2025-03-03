# - Find RapidXML
# Find the RapidXML includes
# This module defines:
#   RAPIDXML_FOUND         - True if RapidXML was found.
#   RAPIDXML_INCLUDE_DIRS  - Include directories for RapidXML.

find_path(RAPIDXML_INCLUDE_DIRS
    NAMES rapidxml/rapidxml.hpp
    HINTS "${CMAKE_INSTALL_PREFIX}/include"
)

# Handle the REQUIRED and QUIETLY arguments using CMake's standard function.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidXML
    REQUIRED_VARS RAPIDXML_INCLUDE_DIRS
    FAIL_MESSAGE "Could NOT find RapidXML (missing: ${RAPIDXML_INCLUDE_DIRS})"
)

# If the package is found, set up include directories and libraries.
if(RapidXML_FOUND)
    message(STATUS "Found RapidXML: Include dirs: ${RAPIDXML_INCLUDE_DIRS}")
endif()

