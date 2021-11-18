
#include "HepMC2FileReader.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

DECLARE_COMPONENT(HepMC2FileReader)

HepMC2FileReader::HepMC2FileReader(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent), m_file(nullptr) {
  declareInterface<IHepMCProviderTool>(this);
}

HepMC2FileReader::~HepMC2FileReader() { ; }

StatusCode HepMC2FileReader::initialize() {
  if (m_filename.empty()) {
    error() << "Input file name is not specified!" << endmsg;
    return StatusCode::FAILURE;
  }
  // open file using HepMC routines
  m_file = std::make_unique<HepMC3::ReaderAsciiHepMC2>(m_filename.value());
  StatusCode sc = GaudiTool::initialize();
  return sc;
}

StatusCode HepMC2FileReader::getNextEvent(HepMC3::GenEvent& event) {
  if (!m_file->read_event(event)) {
    error() << "Premature end of file: Please set the number of events according to hepMC file." << endmsg;
    return Error("Reached end of file before finished processing");
  }
  return StatusCode::SUCCESS;
}

StatusCode HepMC2FileReader::finalize() {
  m_file.reset();
  return GaudiTool::finalize();
}
