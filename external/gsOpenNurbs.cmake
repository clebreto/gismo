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

find_package(opennurbs QUIET)

if(NOT opennurbs_FOUND)

    message(STATUS "Opennurbs not found, fetching ...")

    include(FetchContent)
    include(GNUInstallDirs)

    FetchContent_Declare(
        opennurbs
        GIT_REPOSITORY git@gitlab.inria.fr:dream/opennurbs.git
        GIT_TAG master
    )

    FetchContent_GetProperties(opennurbs)
    if(NOT opennurbs_POPULATED)
        FetchContent_Populate(opennurbs)

        # Configure and build OpenNurbs
        execute_process(
            COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${opennurbs_SOURCE_DIR}
            RESULT_VARIABLE result
        )
        if(result)
            message(FATAL_ERROR "CMake step for opennurbs failed: ${result}")
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${opennurbs_SOURCE_DIR}
            RESULT_VARIABLE result
        )
        if(result)
            message(FATAL_ERROR "Build step for opennurbs failed: ${result}")
        endif()
    endif()

    set(OPENNURBS_LIBRARY_PATH "${opennurbs_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}/libopennurbs.a")
    set(opennurbs_INCLUDE_DIRS ${opennurbs_SOURCE_DIR})
endif()

include_directories(${opennurbs_INCLUDE_DIRS})

message(STATUS "OpenNurbs include dirs: ${opennurbs_INCLUDE_DIRS}")
message(STATUS "OpenNurbs library: ${OPENNURBS_LIBRARY_PATH}")

## #############################################################################
## Code ends here
