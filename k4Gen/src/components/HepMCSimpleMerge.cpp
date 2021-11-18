#include "HepMCSimpleMerge.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

DECLARE_COMPONENT(HepMCSimpleMerge)

HepMCSimpleMerge::HepMCSimpleMerge(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent) {
  declareInterface<IHepMCMergeTool>(this);
}

HepMCSimpleMerge::~HepMCSimpleMerge() {}

StatusCode HepMCSimpleMerge::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if (!sc.isSuccess()) return sc;
  return sc;
}

StatusCode HepMCSimpleMerge::merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) {
  // iterate over vertices and add them to signalEvent
  for (auto it = eventVector.cbegin(), end = eventVector.cend(); it != end; ++it) {
    std::unordered_map<const HepMC3::GenVertex*, HepMC3::GenVertex*> inputToMergedVertexMap;
    for (auto v: (*it).vertices()) {
      HepMC3::GenVertex* newVertex = new HepMC3::GenVertex(v->position());
      inputToMergedVertexMap[v.get()] = newVertex;
    }
    for (auto p: (*it).particles()) {
      // simple check if final-state particle:
      // has no end vertex and correct status code meaning no further decays
      if (!p->end_vertex() && p->status() == 1) {
        // ownership of the particle  (newParticle) is then given to the vertex (newVertex)
        HepMC3::GenParticle* newParticle = new HepMC3::GenParticle(*p);
        // each pile up particle is associated to a new production vertex
        // the position information is preserved
        // ownership of the vertex (newVertex) is given to the event (newEvent)
        HepMC3::GenVertex* newVertex = inputToMergedVertexMap[p->production_vertex().get()];
        newVertex->add_particle_out(newParticle);
        signalEvent.add_vertex(newVertex);
      }
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode HepMCSimpleMerge::finalize() { return GaudiTool::finalize(); }
