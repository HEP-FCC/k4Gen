'''
Pythia8, integrated in the Key4hep ecosystem.

This example generates events according to a Pythia .cmd file and saves them
into a root file in EDM4hep format.
'''

import os

from GaudiKernel import SystemOfUnits as units
from Gaudi.Configuration import INFO

from Configurables import ApplicationMgr, k4DataSvc, PodioOutput
from Configurables import GaussSmearVertex, PythiaInterface, GenAlg
from Configurables import HepMCToEDMConverter, GenParticleFilter


ApplicationMgr().EvtSel = 'NONE'
ApplicationMgr().EvtMax = 2
ApplicationMgr().OutputLevel = INFO
ApplicationMgr().ExtSvc += ["RndmGenSvc"]

# Data service
podioevent = k4DataSvc("EventDataSvc")
ApplicationMgr().ExtSvc += [podioevent]

smeartool = GaussSmearVertex()
smeartool.xVertexSigma = 0.5 * units.mm
smeartool.yVertexSigma = 0.5 * units.mm
smeartool.zVertexSigma = 40.0 * units.mm
smeartool.tVertexSigma = 180.0 * units.picosecond

pythia8gentool = PythiaInterface()
# Example of a Pythia configuration file to generate events,
# taken from $K4GEN location if defined, locally otherwise
path_to_pythiafile = os.environ.get("K4GEN", "")
pythiafilename = "Pythia_standard.cmd"
pythiafile = os.path.join(path_to_pythiafile, pythiafilename)
# Example of pythia configuration file to read LH event file
# pythiafile="options/Pythia_LHEinput.cmd"
pythia8gentool.pythiacard = pythiafile
pythia8gentool.doEvtGenDecays = False
pythia8gentool.printPythiaStatistics = True
pythia8gentool.pythiaExtraSettings = [""]

pythia8gen = GenAlg("Pythia8")
pythia8gen.SignalProvider = pythia8gentool
pythia8gen.VertexSmearingTool = smeartool
pythia8gen.hepmc.Path = "hepmc"
ApplicationMgr().TopAlg += [pythia8gen]

# Reads an HepMC::GenEvent from the data service and writes a collection of
# EDM4hep Particles
hepmc_converter = HepMCToEDMConverter()
hepmc_converter.hepmc.Path = "hepmc"
hepmc_converter.hepmcStatusList = []  # convert particles with all statuses
hepmc_converter.GenParticles.Path = "GenParticles"
ApplicationMgr().TopAlg += [hepmc_converter]

# Filters generated particles
# User needs to provide a list of particle statuses that should be accepted
genfilter = GenParticleFilter("StableParticles")
genfilter.accept = [1]
genfilter.GenParticles.Path = "GenParticles"
genfilter.GenParticlesFiltered.Path = "GenParticlesStable"
ApplicationMgr().TopAlg += [genfilter]

out = PodioOutput("out")
out.outputCommands = ["keep *"]
ApplicationMgr().TopAlg += [out]
