#ifndef GENERATION_HEPMCTOEDMCONVERTER_H
#define GENERATION_HEPMCTOEDMCONVERTER_H

#include "k4FWCore/DataHandle.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Units.h"

namespace edm4hep {
class MCParticleCollection;
class MCParticle;
}

class HepMCToEDMConverter : public GaudiAlgorithm {

public:
  /// Constructor.
  HepMCToEDMConverter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute();
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// list of hepmc statuses that will be converted. 
  /// If emtpy, all particles will be converted. 
  Gaudi::Property<std::vector<unsigned int>> m_hepmcStatusList{this, "hepmcStatusList", {1}, "list of hepmc statuses to keep. An empty list means all statuses will be kept"};
  /// Handle for the HepMC to be read
  DataHandle<HepMC::GenEvent> m_hepmchandle{"hepmc", Gaudi::DataHandle::Reader, this};
  /// Handle for the genparticles to be written
  DataHandle<edm4hep::MCParticleCollection> m_genphandle{"GenParticles", Gaudi::DataHandle::Writer, this};

  edm4hep::MCParticle convert(HepMC::GenParticle* hepmcParticle);
};
#endif
