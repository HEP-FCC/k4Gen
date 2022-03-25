################################################################################
# Package: k4Gen
################################################################################

find_package(HepMC3)
find_package(Pythia8 COMPONENTS pythia8 pythia8tohepmc)
find_package(HepPDT)
find_package(EvtGen)

file(GLOB k4gen_plugin_sources src/components/*.cpp)
gaudi_add_module(k4Gen
                 SOURCES ${k4gen_plugin_sources}
                 LINK Gaudi::GaudiKernel ${HEPMC3_LIBRARIES} Gaudi::GaudiAlgLib k4FWCore::k4FWCore ${HEPPDT_LIBRARIES} ${EVTGEN_LIBRARIES} EDM4HEP::edm4hep EDM4HEP::edm4hepDict)

target_include_directories(k4Gen PUBLIC ${PYTHIA8_INCLUDE_DIRS} 
                                        ${HEPMC3_INCLUDE_DIR}
                                        ${HEPPDT_ICNCLUDE_DIR}
                                        ${EVTGEN_INCLUDE_DIR}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


install(TARGETS k4Gen
  EXPORT k4GenTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)


include(CTest)

#--- The genConf directory has been renamed to genConfDir in Gaudi 35r1
#--- See https://gitlab.cern.ch/gaudi/Gaudi/-/merge_requests/1158
set(GAUDI_GENCONF_DIR "genConfDir")
if (${Gaudi_VERSION} VERSION_LESS 35.1)
  set(GAUDI_GENCONF_DIR "genConf")
endif()

function(set_test_env _testname)
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT
    LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}:$<TARGET_FILE_DIR:k4Gen>:$<TARGET_FILE_DIR:ROOT::Core>:$<TARGET_FILE_DIR:k4FWCore::k4FWCore>:$<TARGET_FILE_DIR:EDM4HEP::edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    PYTHONPATH=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/${GAUDI_GENCONF_DIR}:$<TARGET_FILE_DIR:k4FWCore::k4FWCore>/../python:$ENV{PYTHONPATH}
    PATH=$<TARGET_FILE_DIR:k4FWCore::k4FWCore>/../bin:$ENV{PATH}
    K4GEN=${CMAKE_CURRENT_LIST_DIR}/data
    )
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

add_test(NAME Pythia8ExtraSettings
               WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
               COMMAND  k4run ${CMAKE_CURRENT_LIST_DIR}/options/pythia.py  --pythiaExtraSettings "Beams:idA = 13" "Beams:idB = -13"
              )
set_tests_properties(Pythia8ExtraSettings PROPERTIES
  PASS_REGULAR_EXPRESSION "We collide mu- with mu"
  )
set_test_env(Pythia8ExtraSettings)

#--- Install the example options to the directory where the spack installation
#--- points the $K4GEN environment variable
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/options
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/examples
  PATTERN "__init__.py" EXCLUDE)

file(GLOB k4gen_config_files data/*)
install(FILES ${k4gen_config_files}
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/
  )
