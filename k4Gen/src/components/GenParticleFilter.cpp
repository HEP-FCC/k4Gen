#include "GenParticleFilter.h"

#include "edm4hep/MCParticleCollection.h"

DECLARE_COMPONENT(GenParticleFilter)

GenParticleFilter::GenParticleFilter(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {
  declareProperty("GenParticles", m_iGenpHandle, "Generator Particles to filter (input)");
  declareProperty("GenParticlesFiltered", m_oGenpHandle, "Filtered Generator particles (output)");
}

StatusCode GenParticleFilter::initialize() { return Gaudi::Algorithm::initialize(); }

StatusCode GenParticleFilter::execute(const EventContext&) const {
  const auto inparticles = m_iGenpHandle.get();
  auto particles = m_oGenpHandle.createAndPut();
  bool accept = false;
  int cntr = 0;
  for (auto ptc : (*inparticles)) {
    accept = false;
    for (auto status : m_accept) {
      if (ptc.getGeneratorStatus() == status) {
        accept = true;
      }
    }
    if (accept) {
      auto outptc = ptc.clone();
      particles->push_back(outptc);
    }
    cntr++;
  }
  return StatusCode::SUCCESS;
}

StatusCode GenParticleFilter::finalize() { return Gaudi::Algorithm::finalize(); }
