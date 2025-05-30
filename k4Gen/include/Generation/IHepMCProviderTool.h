#ifndef GENERATION_IHEPMCPROVIDERTOOL_H
#define GENERATION_IHEPMCPROVIDERTOOL_H

#include "GaudiKernel/IAlgTool.h"

namespace HepMC3 {
class GenEvent;
}

/**
 *
 *  Abstract interface to generators and other components that produce HepMC data.
 *
 */

class IHepMCProviderTool : virtual public IAlgTool {
public:
  DeclareInterfaceID(IHepMCProviderTool, 3, 0);

  virtual StatusCode getNextEvent(HepMC3::GenEvent&) = 0;
};

#endif // GENERATION_IHEPMCPROVIDERTOOL_H
