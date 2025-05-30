#ifndef GENERATION_HEPMC2FILEWRITER_H
#define GENERATION_HEPMC2FILEWRITER_H

#include "Gaudi/Algorithm.h"

#include "k4FWCore/DataHandle.h"

namespace HepMC3 {
class GenEvent;
class WriterAsciiHepMC2;
} // namespace HepMC3

/**
 *
 * \brief Algorithm to write HepMC input to a text file.
 *
 * \attention
 * The HepMC format is text-based, fairly verbose and more suitable
 * for debugging than actual storage of physics result, which should be
 * done in the fccsw event data format.
 */

class HepMC2FileWriter : public Gaudi::Algorithm {

public:
  /// Constructor.
  HepMC2FileWriter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute.
  virtual StatusCode execute(const EventContext&) const;
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Handle for the HepMC to be read
  mutable k4FWCore::DataHandle<HepMC3::GenEvent> m_hepmchandle{"HepMC", Gaudi::DataHandle::Reader, this};
  Gaudi::Property<std::string> m_filename{this, "Filename", "Output_HepMC.dat", "Name of the HepMC file to write"};
  std::unique_ptr<HepMC3::WriterAsciiHepMC2> m_file;
};

#endif // GENERATION_HEPMC2FILEWRITER_H
