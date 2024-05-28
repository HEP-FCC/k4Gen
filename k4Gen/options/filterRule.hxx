#include "edm4hep/MCParticleCollection.h"

bool filterRule(const edm4hep::MCParticleCollection& inColl) {
  return inColl->sijze() > 1000;
}
