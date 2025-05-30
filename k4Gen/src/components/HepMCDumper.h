#ifndef GENERATION_HEPMCDUMPER_H
#define GENERATION_HEPMCDUMPER_H

#include "Gaudi/Algorithm.h"

#include "k4FWCore/DataHandle.h"

#include "HepMC3/GenEvent.h"

class HepMCDumper : public Gaudi::Algorithm {

public:
  /// Constructor.
  HepMCDumper(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute(const EventContext&) const;
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Handle for the HepMC to be read
  mutable k4FWCore::DataHandle<HepMC3::GenEvent> m_hepmchandle{"hepmc", Gaudi::DataHandle::Reader, this};
};

#endif // GENERATION_HEPMCDUMPER_H
