CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

#---------------------------------------------------------------
project(k4Gen LANGUAGES CXX)

#- RPATH settings ----------------------------------------------
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE CACHE BOOL "RPATH USE LINK PATH")
option(USE_RUNPATH_INSTEAD_OF_RPATH "Use runpath instead of rpath (allow specifying liked libraries via LD_LIBRARY_PATH)" OFF)
if(USE_RUNPATH_INSTEAD_OF_RPATH) 
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--enable-new-dtags")
endif()

#- Dependencies ------------------------------------------------
find_package(ROOT COMPONENTS RIO Tree)
find_package(Gaudi)
find_package(k4FWCore)
find_package(EDM4HEP)

# use FindHEPPDT from Gaudi (used when finding heppdt later)
set(CMAKE_MODULE_PATH ${Gaudi_LIBRARY_DIR}/cmake/Gaudi/modules ${CMAKE_MODULE_PATH})

#---------------------------------------------------------------
include(GNUInstallDirs)
include(CTest)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/InstallArea/ CACHE PATH
    "Install path prefix, prepended onto install directories." FORCE )
endif()

# Set up C++ Standard
# ``-DCMAKE_CXX_STANDARD=<standard>`` when invoking CMake
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard")

if(NOT CMAKE_CXX_STANDARD MATCHES "14|17")
  message(FATAL_ERROR "Unsupported C++ standard: ${CMAKE_CXX_STANDARD}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#---------------------------------------------------------------
add_subdirectory(k4Gen)

#---------------------------------------------------------------
install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/"
        )

gaudi_install(CMAKE cmake/${PROJECT_NAME}Config.cmake)
