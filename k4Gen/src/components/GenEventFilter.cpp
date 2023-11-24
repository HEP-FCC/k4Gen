#include "GenEventFilter.h"

// Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/Incident.h"

// Datamodel
#include "edm4hep/MCParticleCollection.h"

DECLARE_COMPONENT(GenEventFilter)

GenEventFilter::GenEventFilter(
    const std::string& name,
    ISvcLocator* svcLoc) : GaudiAlgorithm(name, svcLoc) {
  declareProperty("particles", m_inColl,
                  "Generated particles to decide on (input)");
}

StatusCode GenEventFilter::initialize() {
  {
    StatusCode sc = GaudiAlgorithm::initialize();
    if (sc.isFailure()) {
      return sc;
    }
  }

  m_property = service("ApplicationMgr", m_property);
  Gaudi::Property<int> evtMax;
  evtMax.assign(m_property->getProperty("EvtMax"));
  m_nEventsTarget = evtMax;
  debug() << "Targeted number of events: " << m_nEventsTarget << endmsg;
  m_nEventsAccepted = 0;
  m_nEventsSeen = 0;

  m_incidentSvc = service("IncidentSvc");

  m_eventProcessor = service("ApplicationMgr");

  return StatusCode::SUCCESS;
}

StatusCode GenEventFilter::execute() {
  const edm4hep::MCParticleCollection* inParticles = m_inColl.get();
  m_nEventsSeen++;
  bool accept = true;
  /*
  int cntr = 0;
  for (auto ptc : (*inParticles)) {
    accept = false;
    for (auto status : m_accept) {
      if (ptc.getGeneratorStatus() == status) {
        accept = true;
      }
    }
    if (accept) {
    }
    cntr++;
  }
  */


  Gaudi::Property<int> go;
  go.assign(m_property->getProperty("Go"));
  debug() << "Go: " << go << endmsg;

  if (inParticles->size() < 1000) {
    accept = false;
  }

  if (!accept) {
    debug() << "Skipping event..." << endmsg;

    {
      StatusCode sc = m_eventProcessor->nextEvent(1);
      if (sc.isFailure()) {
        error() << "Error when attempting to skip the event! Aborting..."
                << endmsg;
        return sc;
      }
    }

    m_incidentSvc->fireIncident(Incident(name(), IncidentType::AbortEvent));

    return StatusCode::SUCCESS;
  }

  m_nEventsAccepted++;

  debug() << "Event contains " << inParticles->size() << " particles."
          << endmsg;
  debug() << "Targeted number of events to generate: " << m_nEventsTarget
          << endmsg;
  debug() << "Number of events already generated: " << m_nEventsAccepted
          << endmsg;
  debug() << "Remaining number of event to generate: "
          << m_nEventsTarget - m_nEventsAccepted << endmsg;
  debug() << "Number of events seen: " << m_nEventsSeen << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode GenEventFilter::finalize() { return GaudiAlgorithm::finalize(); }
