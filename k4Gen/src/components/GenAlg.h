#ifndef GENERATION_GENALG_H
#define GENERATION_GENALG_H

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"

// k4FWCore
#include "k4FWCore/DataHandle.h"

// k4Gen
#include "Generation/IHepMCMergeTool.h"
#include "Generation/IHepMCProviderTool.h"
#include "Generation/IPileUpTool.h"
#include "Generation/IVertexSmearingTool.h"

namespace HepMC3 {
  class GenEvent;
}

class GenAlg : public GaudiAlgorithm {

public:
  /// Constructor.
  GenAlg(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute();
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Tool to provide signal event
  ToolHandle<IHepMCProviderTool> m_signalProvider{"MomentumRangeParticleGun/HepMCProviderTool", this};
  /// Tool to determine number of pileup events
  ToolHandle<IPileUpTool> m_pileUpTool{"ConstPileUp/PileUpTool", this};
  /// Tool to provide pile up event(s)
  ToolHandle<IHepMCProviderTool> m_pileUpProvider{"MomentumRangeParticleGun/HepMCProviderTool", this};
  // Tool to smear vertex
  ToolHandle<IVertexSmearingTool> m_vertexSmearingTool{"FlatSmearVertex/VertexSmearingTool", this};
  /// Tool to merge HepMC events
  ToolHandle<IHepMCMergeTool> m_HepMCMergeTool{"HepMCSimpleMerge/HepMCMergeTool", this};
  // Output handle for finished event
  DataHandle<HepMC3::GenEvent> m_hepmcHandle{"hepmc", Gaudi::DataHandle::Writer, this};
};

#endif  // GENERATION_GENALG_H
