#include "HepMCToEDMConverter.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "edm4hep/MCParticleCollection.h"
#include "HepPDT/ParticleID.hh"
#include "HepMC3/Attribute.h"

DECLARE_COMPONENT(HepMCToEDMConverter)


edm4hep::MutableMCParticle HepMCToEDMConverter::convert(std::shared_ptr<const HepMC3::GenParticle> hepmcParticle) {
  edm4hep::MutableMCParticle edm_particle;
  edm_particle.setPDG(hepmcParticle->pdg_id());
  edm_particle.setGeneratorStatus(hepmcParticle->status());
  // look up charge from pdg_id
  HepPDT::ParticleID particleID(hepmcParticle->pdg_id());
  edm_particle.setCharge(particleID.charge());
  // convert momentum
  auto p = hepmcParticle->momentum();
  edm_particle.setMomentum( {float(p.px()), float(p.py()), float(p.pz())} );

  // add spin (particle helicity) information if available
  std::shared_ptr<HepMC3::VectorFloatAttribute> spin = hepmcParticle->attribute<HepMC3::VectorFloatAttribute>("spin");
  if (spin) {
     edm4hep::Vector3f hel(spin->value()[0], spin->value()[1], spin->value()[2]);
     edm_particle.setSpin(hel);
  }

  // convert vertex info
  auto prodVtx = hepmcParticle->production_vertex();

  if ( prodVtx!=nullptr ) {
    auto& pos = prodVtx->position();
    edm_particle.setVertex( {float(pos.x()), float(pos.y()), float(pos.z())} );
  }

  return edm_particle;
}

HepMCToEDMConverter::HepMCToEDMConverter(const std::string& name, ISvcLocator* svcLoc) : GaudiAlgorithm(name, svcLoc) {
  declareProperty("hepmc", m_hepmchandle, "HepMC event handle (input)");
  declareProperty("GenParticles", m_genphandle, "Generated particles collection (output)");
}

StatusCode HepMCToEDMConverter::initialize() { 
  return GaudiAlgorithm::initialize();
}

StatusCode HepMCToEDMConverter::execute() {
  const HepMC3::GenEvent* evt = m_hepmchandle.get();
  edm4hep::MCParticleCollection* particles = new edm4hep::MCParticleCollection();

  
  std::unordered_map<unsigned int, edm4hep::MutableMCParticle> _map;
  for (auto _p:evt->particles()) {
    debug() << "Converting hepmc particle with Pdg_ID \t" << _p->pdg_id() << "and id \t" <<  _p->id() << endmsg;
    if (_map.find(_p->id()) == _map.end()) {
      edm4hep::MutableMCParticle edm_particle = convert(_p);
      _map.insert({_p->id(), edm_particle});
    }
    // mother/daughter links
    auto prodvertex = _p->production_vertex();
    if (nullptr != prodvertex) {
      for (auto particle_mother: prodvertex->particles_in()) {
        if (_map.find(particle_mother->id()) == _map.end()) {
          edm4hep::MutableMCParticle edm_particle = convert(particle_mother);
          _map.insert({particle_mother->id(), edm_particle});
        }
        _map[_p->id()].addToParents(_map[particle_mother->id()]);
      }
    }
    auto endvertex = _p->end_vertex();
    if (nullptr != endvertex) {
      for (auto particle_daughter: endvertex->particles_out()) {
        if (_map.find(particle_daughter->id()) == _map.end()) {
          auto edm_particle = convert(particle_daughter);
          _map.insert({particle_daughter->id(), edm_particle});
        }
        _map[_p->id()].addToDaughters(_map[particle_daughter->id()]);
      }
    }
  }
  for (auto particle_pair: _map) {
    particles->push_back(particle_pair.second);
  }
  m_genphandle.put(particles);
  return StatusCode::SUCCESS;
}

StatusCode HepMCToEDMConverter::finalize() {
   return GaudiAlgorithm::finalize();
}
