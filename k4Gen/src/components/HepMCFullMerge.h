
#ifndef GENERATION_HEPMCFULLMERGE_H
#define GENERATION_HEPMCFULLMERGE_H

#include "Generation/IHepMCMergeTool.h"
#include "k4FWCore/DataHandle.h"

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/RndmGenerators.h"

/**
 * Merge several HepMC events into one. Keeps all particles of all events,
 * not just final state ones, in contrast to HepMCSimpleMerge
 * Implementation partly due to pilemc https://pilemc.hepforge.org/
 */

class HepMCFullMerge final : public AlgTool, virtual public IHepMCMergeTool {
public:
  HepMCFullMerge(const std::string& type, const std::string& name, const IInterface* parent);

  virtual ~HepMCFullMerge() final;

  virtual StatusCode initialize() final;

  virtual StatusCode finalize() final;

  /** Merge the events in the container into the signalEvent
   * For events in the vector, new vertices in signalEvent are created to which all pile-up particles are attached.
   *  @param[in/out] signalEvent is the signal event that will be enriched with puEvents from eventVector
   *  @param[in] eventVector is the vector of pile-up GenEvents
   */
  virtual StatusCode merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) final;
};

#endif // GENERATION_HEPMCFULLMERGE_H
