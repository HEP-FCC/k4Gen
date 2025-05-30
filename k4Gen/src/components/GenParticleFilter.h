#ifndef GENERATION_GENPARTICLEFILTER_H
#define GENERATION_GENPARTICLEFILTER_H

#include "Gaudi/Algorithm.h"
#include "k4FWCore/DataHandle.h"

// forward declarations:
namespace edm4hep {
class MCParticleCollection;
}

/** @class GenParticleFilter Generation/src/components/GenParticleFilter.h GenParticleFilter.h
 *
 *  Creates a new MCParticle collection containing all particles that have one of the accepted statuses.
 *  The accepted statuses are set with property accept, e.g. myfilter = GenParticleFilter(accept=[1,2,3])
 *
 *  @author C. Bernet
 *  @author J. Lingemann
 */

class GenParticleFilter : public Gaudi::Algorithm {

public:
  /// Constructor.
  GenParticleFilter(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Execute: Applies the filter
  virtual StatusCode execute(const EventContext&) const;
  /// Finalize.
  virtual StatusCode finalize();

private:
  /// Particle statuses to accept
  Gaudi::Property<std::vector<int>> m_accept{this, "accept", {1}, "Particle statuses to accept"};
  /// Handle for the ParticleCollection to be read
  mutable k4FWCore::DataHandle<edm4hep::MCParticleCollection> m_iGenpHandle{"GenParticles", Gaudi::DataHandle::Reader, this};
  /// Handle for the genparticles to be written
  mutable k4FWCore::DataHandle<edm4hep::MCParticleCollection> m_oGenpHandle{"GenParticlesFiltered", Gaudi::DataHandle::Writer,
                                                                  this};
};

#endif // GENERATION_GENPARTICLEFILTER_H
