#ifndef GENERATION_HEPEVTREADER_H
#define GENERATION_HEPEVTREADER_H

#include <fstream>


#include "Generation/IHepMCFileReaderTool.h"
#include "Generation/IVertexSmearingTool.h"
#include "Generation/IHepMCMergeTool.h"

#include "k4FWCore/DataHandle.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"

#include "GaudiKernel/SystemOfUnits.h"


namespace edm4hep {
class MCParticleCollection;
}

/**@class HepEVTReader HepEVTReader.h HepEVTReader.h
 *
 *  This algorithm reads in events from a HepEVT file and puts them into the
 *  transient event store.
 *  event.
 *
 */

class HepEVTReader: public GaudiAlgorithm {

public:
  /// Constructor.
  HepEVTReader(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute();
  /// Finalize.
  virtual StatusCode finalize();

private:

  std::string m_filename;
  std::ifstream m_input;
  int NHEP;
  int m_format;

  /// Handle for the genparticles to be written
  DataHandle<edm4hep::MCParticleCollection> m_genphandle {"GenParticles", Gaudi::DataHandle::Writer, this};

};

#endif //GENERATION_HEPEVTREADER_H
