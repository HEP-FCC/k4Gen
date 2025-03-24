#ifndef GENERATION_HEPMCHISTOGRAMS_H
#define GENERATION_HEPMCHISTOGRAMS_H

#include "Gaudi/Algorithm.h"
#include "GaudiKernel/ITHistSvc.h"
#include "HepMC3/GenEvent.h"
#include "k4FWCore/DataHandle.h"

#include "TH1F.h"

class HepMCHistograms : public Gaudi::Algorithm {

public:
  /// Constructor.
  HepMCHistograms(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute(const EventContext&) const;
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Handle for the HepMC to be read
  mutable DataHandle<HepMC3::GenEvent> m_hepmchandle{"HepMC", Gaudi::DataHandle::Reader, this};

  SmartIF<ITHistSvc> m_ths; ///< THistogram service

  TH1F* m_pt{nullptr};  ///< histogram for pT of particles
  TH1F* m_eta{nullptr}; ///< histogram for pseudorapidity of particles

  TH1F* m_d0{nullptr}; ///< histogram for transversal IP
  TH1F* m_z0{nullptr}; ///< histogram for longidudinal IP
};

#endif // GENERATION_HEPMCHISTOGRAMS_H
