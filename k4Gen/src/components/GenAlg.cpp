
#include "GenAlg.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

#include "HepMC3/GenEvent.h"

DECLARE_COMPONENT(GenAlg)

GenAlg::GenAlg(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {

  declareProperty("PileUpTool", m_pileUpTool);

  declareProperty("VertexSmearingTool", m_vertexSmearingTool);

  declareProperty("HepMCMergeTool", m_HepMCMergeTool);

  declareProperty("SignalProvider", m_signalProvider);
  declareProperty("PileUpProvider", m_pileUpProvider);

  declareProperty("hepmc", m_hepmchandle, "HepMC event handle (output)");
}

StatusCode GenAlg::initialize() {
  StatusCode sc = Gaudi::Algorithm::initialize();
  if (!sc.isSuccess()) return sc;
  return sc;
}

StatusCode GenAlg::execute(const EventContext&) const {
  HepMC3::GenEvent* theEvent = m_hepmchandle.createAndPut();
  theEvent->set_units(HepMC3::Units::GEV, HepMC3::Units::MM);
  const unsigned int numPileUp = m_pileUpTool->numberOfPileUp();
  std::vector<HepMC3::GenEvent> eventVector;
  eventVector.reserve(numPileUp + 1);
  StatusCode sc;
  if (!m_signalProvider.empty()) {
    sc = m_signalProvider->getNextEvent(*theEvent);
  }
  if (StatusCode::SUCCESS != sc) {
    return sc;
  }
  m_vertexSmearingTool->smearVertex(*theEvent).ignore();
  if (!m_pileUpProvider.empty()) {
    for (unsigned int i_pileUp = 0; i_pileUp < numPileUp; ++i_pileUp) {
      auto puEvt = HepMC3::GenEvent();
      sc = m_pileUpProvider->getNextEvent(puEvt);
      if (StatusCode::SUCCESS != sc) {
        return sc;
      }
      m_vertexSmearingTool->smearVertex(puEvt).ignore();
      eventVector.push_back(std::move(puEvt));
    }
  }
  return m_HepMCMergeTool->merge(*theEvent, eventVector);
}

StatusCode GenAlg::finalize() { return Gaudi::Algorithm::finalize(); }
