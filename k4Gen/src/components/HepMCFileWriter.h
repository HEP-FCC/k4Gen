#ifndef GENERATION_HEPMCFILEWRITER_H
#define GENERATION_HEPMCFILEWRITER_H

#include "GaudiAlg/GaudiAlgorithm.h"

#include "k4FWCore/DataHandle.h"

namespace HepMC3 {
class GenEvent;
class WriterAscii;
}

/**
 *
 * \brief Algorithm to write HepMC input to a text file.
 *
 * \attention
 * The HepMC format is text-based, fairly verbose and more suitable
 * for debugging than actual storage of physics result, which should be
 * done in the fccsw event data format.
 */

class HepMC2FileWriter : public GaudiAlgorithm {

public:
  /// Constructor.
  HepMC2FileWriter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute();
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Handle for the HepMC to be read
  DataHandle<HepMC3::GenEvent> m_hepmchandle{"HepMC", Gaudi::DataHandle::Reader, this};
  Gaudi::Property<std::string> m_filename{this, "Filename", "Output_HepMC.dat", "Name of the HepMC file to write"};
  std::unique_ptr<HepMC3::WriterAscii> m_file;
};

#endif  // GENERATION_HEPMCFILEWRITER_H
