#ifndef GENERATION_IHEPMCFILEREADERTOOL_H
#define GENERATION_IHEPMCFILEREADERTOOL_H

#include "GaudiKernel/IAlgTool.h"

namespace HepMC3 {
class GenEvent;
}

/** @class IHepMCFileReaderTool IHepMCFileReaderTool.h "Generation/IHepMCFileReaderTool.h"
 *
 *  Abstract interface to read hepmc files.
 *
 */

class IHepMCFileReaderTool : virtual public IAlgTool {
public:
  DeclareInterfaceID(IHepMCFileReaderTool, 3, 0);

  virtual StatusCode readNextEvent(HepMC3::GenEvent&) = 0;

  virtual StatusCode open(const std::string& filename) = 0;
};

#endif // GENERATION_IHEPMCFILEREADERTOOL_H
