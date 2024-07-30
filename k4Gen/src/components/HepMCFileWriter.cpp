#include "HepMCFileWriter.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"

DECLARE_COMPONENT(HepMCFileWriter)

HepMCFileWriter::HepMCFileWriter(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {
  declareProperty("hepmc", m_hepmchandle, "The HepMC event to write to text file (input)");
}

StatusCode HepMCFileWriter::initialize() {
  m_file = std::make_unique<HepMC3::WriterAscii>(m_filename.value());
  return Gaudi::Algorithm::initialize();
}

StatusCode HepMCFileWriter::execute(const EventContext&) const {
  const HepMC3::GenEvent* theEvent = m_hepmchandle.get();
  m_file->write_event(*theEvent);
  return StatusCode::SUCCESS;
}

StatusCode HepMCFileWriter::finalize() {
  m_file.reset();
  return Gaudi::Algorithm::finalize();
}
