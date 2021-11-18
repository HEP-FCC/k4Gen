#include "HepMCFullMerge.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

DECLARE_COMPONENT(HepMCFullMerge)

HepMCFullMerge::HepMCFullMerge(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent) {
  declareInterface<IHepMCMergeTool>(this);
}

HepMCFullMerge::~HepMCFullMerge() {}

StatusCode HepMCFullMerge::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if (!sc.isSuccess()) return sc;
  return sc;
}

StatusCode HepMCFullMerge::merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) {
  for (auto it = eventVector.cbegin(), end = eventVector.cend(); it != end; ++it) {
    // keep track of which vertex in full event corresponds to which vertex in merged event
    std::unordered_map<const HepMC3::GenVertex*, HepMC3::GenVertex*> inputToMergedVertexMap;
    for (auto v: (*it).vertices()) {
      HepMC3::GenVertex* outvertex = new HepMC3::GenVertex(v->position());
      inputToMergedVertexMap[v.get()] = outvertex;
      signalEvent.add_vertex(outvertex);
    }
    for (auto p: (*it).particles()) {
      // ownership of the particle is given to the vertex
      HepMC3::GenParticle* newparticle = new HepMC3::GenParticle(*p);
      // attach particles to correct vertices in merged event
      if (nullptr != p->end_vertex()) {
        inputToMergedVertexMap[p->end_vertex().get()]->add_particle_in(newparticle);
      }
      if (nullptr != p->production_vertex()) {
        inputToMergedVertexMap[p->production_vertex().get()]->add_particle_out(newparticle);
      }
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode HepMCFullMerge::finalize() { return GaudiTool::finalize(); }
