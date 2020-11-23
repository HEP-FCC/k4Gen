################################################################################
# Package: k4Gen
################################################################################

find_package(HepMC)
find_package(Pythia8 COMPONENTS pythia8 pythia8tohepmc)
find_package(PODIO)
find_package(HepPDT)
find_package(EvtGen)


gaudi_add_module(k4Gen
                 SOURCES src/components/*.cpp
                 LINK HepMC GaudiAlgLib FWCore Pythia8 PODIO HepPDT ${EVTGEN_LIBRARIES} EDM4HEP::edm4hep EDM4HEP::edm4hepDict)
target_include_directories(k4Gen PUBLIC
  
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS k4Gen
  EXPORT k4GenTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev)


include(CTest)

set(K4RUN ${CMAKE_BINARY_DIR}/run ${PROJECT_SOURCE_DIR}/k4FWCore/scripts/k4run)


add_test(particleGun
               COMMAND ${K4RUN} ${CMAKE_CURRENT_LIST_DIR}/options/particleGun.py
               )


add_test(GenPythiaMatching
               COMMAND  ${K4RUN}  ${CMAKE_CURRENT_LIST_DIR}/options/pythia.py --Filename ${CMAKE_CURRENT_LIST_DIR}/data/powheg_ggHH_events.cmd
              )


