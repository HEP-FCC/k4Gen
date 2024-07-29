
#ifndef GENERATION_GENALG_H
#define GENERATION_GENALG_H

#include "Generation/IHepMCMergeTool.h"
#include "Generation/IHepMCProviderTool.h"
#include "Generation/IPileUpTool.h"
#include "Generation/IVertexSmearingTool.h"


#include "Gaudi/Algorithm.h"
#include "GaudiKernel/ToolHandle.h"


#include "k4FWCore/DataHandle.h"
namespace HepMC3 {
class GenEvent;
}

class GenAlg : public Gaudi::Algorithm {

public:
  /// Constructor.
  GenAlg(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute(const EventContext&) const;
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Tools to handle input from HepMC-file
  ToolHandle<IHepMCProviderTool> m_signalProvider{"MomentumRangeParticleGun/HepMCProviderTool", this};
  ToolHandle<IHepMCProviderTool> m_pileUpProvider{"MomentumRangeParticleGun/HepMCProviderTool", this};
  ToolHandle<IPileUpTool> m_pileUpTool{"ConstPileUp/PileUpTool", this};

  /// Tool to merge HepMC events
  ToolHandle<IHepMCMergeTool> m_HepMCMergeTool{"HepMCSimpleMerge/HepMCMergeTool", this};
  // Tool to smear vertices
  ToolHandle<IVertexSmearingTool> m_vertexSmearingTool{"FlatSmearVertex/VertexSmearingTool", this};
  // output handle for finished event
  mutable DataHandle<HepMC3::GenEvent> m_hepmchandle{"hepmc", Gaudi::DataHandle::Writer, this};
};

#endif  // GENERATION_GENALG_H
