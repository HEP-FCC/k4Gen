#include "HepMCDumper.h"

#include "HepMC3/Print.h"

DECLARE_COMPONENT(HepMCDumper)

HepMCDumper::HepMCDumper(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {
  declareProperty("hepmc", m_hepmchandle, "The HepMC event to dump");
}

StatusCode HepMCDumper::initialize() { return Gaudi::Algorithm::initialize(); }

StatusCode HepMCDumper::execute(const EventContext&) const {
  const HepMC3::GenEvent* theEvent = m_hepmchandle.get();
  HepMC3::Print::content(*theEvent);
  return StatusCode::SUCCESS;
}

StatusCode HepMCDumper::finalize() { return Gaudi::Algorithm::finalize(); }
