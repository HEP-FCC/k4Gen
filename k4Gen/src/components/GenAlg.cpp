/*
 * Copyright (c) 2020-2024 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "GenAlg.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

#include "HepMC3/GenEvent.h"

DECLARE_COMPONENT(GenAlg)

GenAlg::GenAlg(const std::string& name, ISvcLocator* svcLoc) : Gaudi::Algorithm(name, svcLoc) {

  declareProperty("PileUpTool", m_pileUpTool);

  declareProperty("VertexSmearingTool", m_vertexSmearingTool);

  declareProperty("HepMCMergeTool", m_HepMCMergeTool);

  declareProperty("SignalProvider", m_signalProvider);
  declareProperty("PileUpProvider", m_pileUpProvider);

  declareProperty("hepmc", m_hepmchandle, "HepMC event handle (output)");
}

StatusCode GenAlg::initialize() {
  StatusCode sc = Gaudi::Algorithm::initialize();
  if (!sc.isSuccess())
    return sc;
  return sc;
}

StatusCode GenAlg::execute(const EventContext&) const {
  HepMC3::GenEvent* theEvent = m_hepmchandle.createAndPut();
  theEvent->set_units(HepMC3::Units::GEV, HepMC3::Units::MM);
  const unsigned int numPileUp = m_pileUpTool->numberOfPileUp();
  std::vector<HepMC3::GenEvent> eventVector;
  eventVector.reserve(numPileUp + 1);
  StatusCode sc;
  if (!m_signalProvider.empty()) {
    sc = m_signalProvider->getNextEvent(*theEvent);
  }
  if (StatusCode::SUCCESS != sc) {
    return sc;
  }
  m_vertexSmearingTool->smearVertex(*theEvent).ignore();
  if (!m_pileUpProvider.empty()) {
    for (unsigned int i_pileUp = 0; i_pileUp < numPileUp; ++i_pileUp) {
      auto puEvt = HepMC3::GenEvent();
      sc = m_pileUpProvider->getNextEvent(puEvt);
      if (StatusCode::SUCCESS != sc) {
        return sc;
      }
      m_vertexSmearingTool->smearVertex(puEvt).ignore();
      eventVector.push_back(std::move(puEvt));
    }
  }
  return m_HepMCMergeTool->merge(*theEvent, eventVector);
}

StatusCode GenAlg::finalize() { return Gaudi::Algorithm::finalize(); }
