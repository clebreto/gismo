## Version: $Id:  $
##
##

## Commentary:
##
##

## Changelog:
##
##

##
## Code starts here
## #############################################################################

find_package(ZLIB)

if(NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found, fetching ...")

    include(FetchContent)

    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG v1.3.1)

    FetchContent_MakeAvailable(zlib)
else()
    add_library(zlib ALIAS ZLIB::ZLIB)
endif()

message(STATUS "ZLIB include dirs: ${ZLIB_INCLUDE_DIRS}")

## #############################################################################
## Code ends here
