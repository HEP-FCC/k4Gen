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
                 LINK Gaudi::GaudiKernel HepMC Gaudi::GaudiAlgLib k4FWCore::k4FWCore k4FWCore::k4FWCorePlugins HepPDT ${EVTGEN_LIBRARIES} EDM4HEP::edm4hep EDM4HEP::edm4hepDict)

target_include_directories(k4Gen PUBLIC ${PYTHIA8_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


install(TARGETS k4Gen
  EXPORT k4GenTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)


include(CTest)

get_target_property(k4fwcore_lib k4FWCore::k4FWCore LOCATION)
get_filename_component(k4fwcore_loc ${k4fwcore_lib} DIRECTORY)

get_target_property(edm4hep_lib EDM4HEP::edm4hepDict LOCATION)
get_filename_component(edm4hep_loc ${edm4hep_lib} DIRECTORY)

get_target_property(root_lib ROOT::Core LOCATION)
get_filename_component(root_loc ${root_lib} DIRECTORY)

get_target_property(podio_lib podio::podio LOCATION)
get_filename_component(podio_loc ${podio_lib} DIRECTORY)


function(set_test_env _testname)
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT "ROOT_INCLUDE_PATH=${podio_loc}/../include/podio:${edm4hep_loc}/../include:$ENV{ROOT_INCLUDE_PATH}")
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}:${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}:${root_loc}:${k4fwcore_loc}:${edm4hep_loc}:${podio_loc}:$ENV{LD_LIBRARY_PATH}")
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/genConf:${k4fwcore_loc}/../python:$ENV{PYTHONPATH}")
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT "PATH=${k4fwcore_loc}/../bin:$ENV{PATH}")
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT "K4GEN=${CMAKE_CURRENT_LIST_DIR}/")
endfunction()




add_test(NAME ParticleGun
               WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
               COMMAND k4run ${CMAKE_CURRENT_LIST_DIR}/options/particleGun.py
               )
set_test_env(ParticleGun)


add_test(NAME Pythia8Default
               WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
               COMMAND  k4run ${CMAKE_CURRENT_LIST_DIR}/options/pythia.py 
              )
set_test_env(Pythia8Default)

#--- Install the example options to the directory where the spack installation
#--- points the $K4GEN environment variable
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/options
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/examples
  PATTERN "__init__.py" EXCLUDE)
