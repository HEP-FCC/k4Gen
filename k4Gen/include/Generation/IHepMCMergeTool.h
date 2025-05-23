#ifndef GENERATION_IHEPMCMERGETOOL_H
#define GENERATION_IHEPMCMERGETOOL_H

#include "GaudiKernel/IAlgTool.h"

#include "HepMC3/GenEvent.h"

/** @class IHepMCMergeTool IHepMCMergeTool.h "Generation/IHepMCMergeTool.h"
 *
 *  Abstract interface to merge HepMC Events
 *
 */

class IHepMCMergeTool : virtual public IAlgTool {
public:
  DeclareInterfaceID(IHepMCMergeTool, 1, 0);

  /// Turn a signal event and a vector of pileup events into a merged event.
  virtual StatusCode merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) = 0;
};

#endif // GENERATION_IHEPMCMERGETOOL_H
