
#include "HepMCFileReaderTool.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

DECLARE_COMPONENT(HepMCFileReader)

HepMCFileReader::HepMCFileReader(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent), m_file(nullptr) {
  declareInterface<IHepMCProviderTool>(this);
}

HepMCFileReader::~HepMCFileReader() { ; }

StatusCode HepMCFileReader::initialize() {
  if (m_filename.empty()) {
    error() << "Input file name is not specified!" << endmsg;
    return StatusCode::FAILURE;
  }
  // open file using HepMC routines
  m_file = std::make_unique<HepMC3::ReaderAscii>(m_filename.value());
  StatusCode sc = GaudiTool::initialize();
  return sc;
}

StatusCode HepMCFileReader::getNextEvent(HepMC3::GenEvent& event) {
  if (!m_file->read_event(event)) {
   // if (m_file->rdstate() == std::ios::eofbit) {
   //   error() << "Error reading HepMC file" << endmsg;
   //   return StatusCode::FAILURE;
   // }
    error() << "Premature end of file: Please set the number of events according to hepMC file." << endmsg;
    return Error("Reached end of file before finished processing");
  }
  return StatusCode::SUCCESS;
}

StatusCode HepMCFileReader::finalize() {
  m_file.reset();
  return GaudiTool::finalize();
}
