################################################################################
# Package: k4Gen
################################################################################

find_package(HepMC)
find_package(Pythia8 COMPONENTS pythia8 pythia8tohepmc)
find_package(HepPDT)
find_package(EvtGen)
find_package(Gaudi)

file(GLOB k4gen_plugin_sources src/components/*.cpp)
gaudi_add_module(k4Gen
                 SOURCES ${k4gen_plugin_sources}
                 LINK GaudiKernel HepMC GaudiAlgLib k4FWCore::k4FWCore k4FWCore::k4FWCorePlugins HepPDT ${EVTGEN_LIBRARIES} EDM4HEP::edm4hep EDM4HEP::edm4hepDict)

target_include_directories(k4Gen PUBLIC ${PYTHIA8_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

## Get all propreties that cmake supports
#execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
#
## Convert command output into a CMake list
#STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#
#function(print_properties)
#    message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
#endfunction(print_properties)
#
#function(print_target_properties tgt)
#    if(NOT TARGET ${tgt})
#      message("There is no target named '${tgt}'")
#      return()
#    endif()
#
#    foreach (prop ${CMAKE_PROPERTY_LIST})
#        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
#    # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
#    if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
#        continue()
#    endif()
#        # message ("Checking ${prop}")
#        get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
#        if (propval)
#            get_target_property(propval ${tgt} ${prop})
#            message ("${tgt} ${prop} = ${propval}")
#        endif()
#    endforeach(prop)
#endfunction(print_target_properties)
#
##print_target_properties(k4FWCore::k4FWCorePlugins)
##print_target_properties(Gaudi::GaudiKernel)
#
#get_property(bbbb TARGET k4FWCore::k4FWCorePlugins PROPERTY INSTALL_RPATH )
##message(aaa ${bbbb})
#set_property(TARGET k4Gen APPEND
#    PROPERTY runtime_ld_library_path $bbbb)

install(TARGETS k4Gen
  EXPORT k4GenTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)


include(CTest)



add_test(NAME ParticleGun
               COMMAND k4run ${CMAKE_CURRENT_LIST_DIR}/options/particleGun.py
               )


add_test(NAME Pythia8Default
               WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
               COMMAND  k4run ${CMAKE_CURRENT_LIST_DIR}/options/pythia.py 
              )


