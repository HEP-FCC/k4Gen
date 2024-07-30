#include "HepMCHistograms.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

DECLARE_COMPONENT(HepMCHistograms)

HepMCHistograms::HepMCHistograms(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {
  declareProperty("hepmc", m_hepmchandle);
}

StatusCode HepMCHistograms::initialize() {
  if (Gaudi::Algorithm::initialize().isFailure()) return StatusCode::FAILURE;

  if (service("THistSvc", m_ths).isFailure()) {
    error() << "Couldn't get THistSvc" << endmsg;
    return StatusCode::FAILURE;
  }

  m_pt = new TH1F("GenPt", "Generated particles pT", 100, .1, 10);
  if (m_ths->regHist("/rec/GenPt", m_pt).isFailure()) {
    error() << "Couldn't register GenPt" << endmsg;
  }

  m_eta = new TH1F("GenEta", "Generated particles Pseudorapidity", 100, -10, 10);
  if (m_ths->regHist("/rec/GenEta", m_eta).isFailure()) {
    error() << "Couldn't register GenEta" << endmsg;
  }

  m_d0 = new TH1F("GenD0", "Transversal Impact Parameter", 100, 0, 10);
  if (m_ths->regHist("/rec/GenD0", m_d0).isFailure()) {
    error() << "Couldn't register GenD0" << endmsg;
  }

  m_z0 = new TH1F("GenZ0", "Longitudinal Impact Parameter", 100, -30, 30);
  if (m_ths->regHist("/rec/GenZ0", m_z0).isFailure()) {
    error() << "Couldn't register GenZ0" << endmsg;
  }

  return StatusCode::SUCCESS;
}

StatusCode HepMCHistograms::execute(const EventContext&) const {
  auto evt = m_hepmchandle.get();

  info() << "Processing event with " << evt->particles().size() << " particles" << endmsg;

  for (auto p: evt->particles()) {
    m_eta->Fill(p->momentum().eta());
    m_pt->Fill(p->momentum().perp());
  }

  for (auto v: evt->vertices()) {
    m_d0->Fill(v->position().perp());
    m_z0->Fill(v->position().z());
  }

  return StatusCode::SUCCESS;
}

StatusCode HepMCHistograms::finalize() {
  if (Gaudi::Algorithm::finalize().isFailure()) return StatusCode::FAILURE;

  return StatusCode::SUCCESS;
}
