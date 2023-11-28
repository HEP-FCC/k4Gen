#include "GenEventFilter.h"

// Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/Incident.h"

// Datamodel
#include "edm4hep/MCParticleCollection.h"

// ROOT
#include "TInterpreter.h"

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

  {
    bool success = gInterpreter->Declare("#include \"edm4hep/MCParticleCollection.h\"");
    if (!success) {
      error() << "Unable to find edm4hep::MCParticleCollection header file!" << endmsg;
      return StatusCode::FAILURE;
    }
    debug() << "Found edm4hep::MCParticleCollection header file." << endmsg;
  }

  {
    bool success = gInterpreter->Declare("bool filterRule(const edm4hep::MCParticleCollection& inColl){return inColl.size() > 1000;}");
    if (!success) {
      error() << "Unable to compile filter rule!" << endmsg;
      return StatusCode::FAILURE;
    }
    debug() << "Filter rule compiled successfully." << endmsg;
  }

  {
    enum TInterpreter::EErrorCode err = TInterpreter::kProcessing;
    m_filterRulePtr = (bool (*)(const edm4hep::MCParticleCollection*)) gInterpreter->ProcessLineSynch("&filterRule", &err);
    if (err != TInterpreter::kNoError) {
      error() << "Unable to obtain filter rule pointer!" << endmsg;
      return StatusCode::FAILURE;
    }
    debug() << "Filter rule pointer obtained successfully." << endmsg;
  }

  return StatusCode::SUCCESS;
}

StatusCode GenEventFilter::execute() {
  const edm4hep::MCParticleCollection* inParticles = m_inColl.get();
  m_nEventsSeen++;

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


  if (!(*m_filterRulePtr)(inParticles)) {
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
  debug() << "Number of events seen so far: " << m_nEventsSeen << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode GenEventFilter::finalize() {
  debug() << "Number of events seen: " << m_nEventsSeen << endmsg;

  return GaudiAlgorithm::finalize();
}
