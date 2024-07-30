#include "HepMC2FileWriter.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAsciiHepMC2.h"

DECLARE_COMPONENT(HepMC2FileWriter)

HepMC2FileWriter::HepMC2FileWriter(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {
  declareProperty("hepmc", m_hepmchandle, "The HepMC event to write to text file (input)");
}

StatusCode HepMC2FileWriter::initialize() {

  m_file = std::make_unique<HepMC3::WriterAsciiHepMC2>(m_filename.value());
  return Gaudi::Algorithm::initialize();
}

StatusCode HepMC2FileWriter::execute(const EventContext&) const {
  const HepMC3::GenEvent* theEvent = m_hepmchandle.get();
  m_file->write_event(*theEvent);
  return StatusCode::SUCCESS;
}

StatusCode HepMC2FileWriter::finalize() {
  m_file.reset();
  return Gaudi::Algorithm::finalize();
}
