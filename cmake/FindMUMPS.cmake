## ############################
## Module to find MUMPS package
## ############################

set(MUMPS_FOUND "NO")

if(MUMPS_DIR)
  # MUMPS_DIR is specified
  set(MUMPS_INCLUDE_DIR ${MUMPS_DIR}/include)
  set(MUMPS_LIBRARY_DIR ${MUMPS_DIR}/lib)
elseif(DEFINED ENV{MUMPS_DIR})
  # MUMPS_DIR is specified
  set(MUMPS_INCLUDE_DIR $ENV{MUMPS_DIR}/include)
  set(MUMPS_LIBRARY_DIR $ENV{MUMPS_DIR}/lib)
elseif(DEFINED ENV{MUMPS_LIBRARY_DIR})
  # MUMPS_INCLUDE_DIR & LIBRARY_DIR is specified
  set(MUMPS_INCLUDE_DIR $ENV{MUMPS_INCLUDE_DIR})
  set(MUMPS_LIBRARY_DIR $ENV{MUMPS_LIBRARY_DIR})
else()
  # Otherwise look for standard or specific places
  find_path (MUMPS_INCLUDE_DIR
    NAMES zmumps_struc.h
    PATHS
    /opt/MUMPS/include
    /usr/MUMPS/include
    /usr/local/opt/mumps/include
    /usr/include
    /usr/include/MUMPS
    ~/include)

  find_path(MUMPS_LIBRARY_DIR
    NAMES libzmumps.so
    PATHS
    /opt/MUMPS/lib
    /usr/MUMPS/lib
    /usr/local/opt/mumps/lib
    /usr/lib
    /usr/lib64/openmpi/lib
    /usr/lib/x86_64-linux-gnu/
    ~/lib)
endif()

if(MUMPS_INCLUDE_DIR AND MUMPS_LIBRARY_DIR)
  set(MUMPS_FOUND YES)

  find_library(MUMPS_COMMON_LIBRARY 
    NAMES mumps_common
    PATHS ${MUMPS_LIBRARY_DIR} 
    NO_DEFAULT_PATH)

  find_library(MUMPS_Z_LIBRARY 
    NAMES zmumps
    PATHS ${MUMPS_LIBRARY_DIR} 
    NO_DEFAULT_PATH)

  find_library(MUMPS_C_LIBRARY
    NAMES cmumps
    PATHS ${MUMPS_LIBRARY_DIR}
    NO_DEFAULT_PATH)

  find_library(MUMPS_PORD_LIBRARY 
    NAMES pord
    PATHS ${MUMPS_LIBRARY_DIR}
    NO_DEFAULT_PATH)

  set(MUMPS_LIBRARIES ${MUMPS_Z_LIBRARY}
    ${MUMPS_C_LIBRARY} ${MUMPS_COMMON_LIBRARY} ${MUMPS_PORD_LIBRARY})
else()
  if(MUMPS_FIND_REQUIRED)
    message(FATAL_ERROR "MUMPS not found, please set MUMPS_DIR to your MUMPS install directory")
  endif()
endif()
