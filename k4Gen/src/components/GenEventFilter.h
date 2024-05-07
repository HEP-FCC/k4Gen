#ifndef GENERATION_GENEVENTFILTER_H
#define GENERATION_GENEVENTFILTER_H

// Gaudi
#include "GaudiKernel/Algorithm.h"
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
 *  Filters events based on the user defined filter rule applied on MCParticle
 *  collection.
 *
 *  @author J. Smiesko
*/

class GenEventFilter : public Gaudi::Algorithm {

public:
  /// Constructor
  GenEventFilter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize
  virtual StatusCode initialize();
  /// Execute: Applies the filter
  virtual StatusCode execute(const EventContext& evtCtx) const;
  /// Finalize
  virtual StatusCode finalize();

private:
  /// Handle for the MCParticle collection to be read
  mutable DataHandle<edm4hep::MCParticleCollection> m_inColl{
      "particles", Gaudi::DataHandle::Reader, this};

  /// Rule to filter the events with
  Gaudi::Property<std::string> m_filterRuleStr{
      this, "filterRule", "", "Filter rule to apply on the events"};

  /// Path of the filter rule file
  Gaudi::Property<std::string> m_filterRulePath{
      this, "filterRulePath", "", "Path to the filter rule file"};

  /// Targeted number of events.
  mutable std::atomic<unsigned int> m_nEventsTarget;
  /// Keep track of how many events were already accepted.
  mutable std::atomic<unsigned int> m_nEventsAccepted;
  /// Keep track of how many events we went through.
  mutable std::atomic<unsigned int> m_nEventsSeen;
  /// How many events were skipped in a row.
  mutable std::atomic<unsigned int> m_nEventsSkipped;
  /// Pointer to the property.
  SmartIF<IProperty> m_property;
  /// Pointer to the incident service.
  SmartIF<IIncidentSvc> m_incidentSvc;
  /// Pointer to the event processor.
  SmartIF<IEventProcessor> m_eventProcessor;
  /// Filter rule pointer.
  bool (*m_filterRulePtr)(const edm4hep::MCParticleCollection*);
};

#endif  // GENERATION_GENEVENTFILTER_H
