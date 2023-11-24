#ifndef GENERATION_GENEVENTFILTER_H
#define GENERATION_GENEVENTFILTER_H

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
class IProperty;
class IIncidentSvc;
class IEventProcessor;

// k4FWCore
#include "k4FWCore/DataHandle.h"

// Datamodel
namespace edm4hep {
  class MCParticleCollection;
}

/** @class GenEventFilter Generation/src/components/GenEventFilter.h GenEventFilter.h
 *
 *  Skips events if MCParticle collection does not contain desired particles.
 *
 *  @author J. Smiesko
*/

class GenEventFilter : public GaudiAlgorithm {

public:
  /// Constructor
  GenEventFilter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize
  virtual StatusCode initialize();
  /// Execute: Applies the filter
  virtual StatusCode execute();
  /// Finalize
  virtual StatusCode finalize();

private:
  /// Handle for the MCParticle collection to be read
  DataHandle<edm4hep::MCParticleCollection> m_inColl{
      "particles", Gaudi::DataHandle::Reader, this};

  /// Rules to accept an event.
  Gaudi::Property<std::vector<std::string>> m_rules{
      this, "rules", {"e-"}, "Rules to accept an event"};

  /// Targeted number of events.
  size_t m_nEventsTarget;
  /// Keep track of how many events were already accepted.
  size_t m_nEventsAccepted;
  /// Keep track of how many events we went through.
  size_t m_nEventsSeen;
  /// Pointer to the property.
  SmartIF<IProperty> m_property;
  /// Pointer to the incident service.
  SmartIF<IIncidentSvc> m_incidentSvc;
  /// Pointer to the event processor.
  SmartIF<IEventProcessor> m_eventProcessor;
};

#endif  // GENERATION_GENEVENTFILTER_H
