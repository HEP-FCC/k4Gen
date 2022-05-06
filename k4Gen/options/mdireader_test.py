from Gaudi.Configuration import *
import os

# Data service
from Configurables import FCCDataSvc
podioevent = FCCDataSvc("EventDataSvc")

from Configurables import MDIReader
#mdi_converter = MDIReader("Reader",MDIFilename="k4Gen/options/mdireader_testparticles.dat")
mdi_converter = MDIReader("Reader",MDIFilename=(os.path.join(os.environ["K4GEN"],'../options/mdireader_testparticles.dat')))
mdi_converter.GenParticles.Path = "allGenParticles"
mdi_converter.CrossingAngle = 0.0
mdi_converter.LongitudinalCut = 0
mdi_converter.InputType = "xtrack"
mdi_converter.BeamEnergy = 45.6

# DD4hep geometry service
# Parses the given xml file
from Configurables import GeoSvc
geoservice = GeoSvc("GeoSvc", detectors=[
                                         #os.path.join(os.environ["FCCDETECTORS"], 'Detector/DetFCCeeCLD/compact/FCCee_DectMaster.xml'),
                                         #os.path.join(os.environ["FCCDETECTORS"], 'Detector/DetFCCeeCLD/compact/FCCee_o2_v02/FCCee_o2_v02.xml'),
                                         os.path.join(os.environ["FCCDETECTORS"], 'Detector/DetCommon/compact/Box.xml'),
                                         ])

# Geant4 service
# Configures the Geant simulation: geometry, physics list and user actions
from Configurables import SimG4Svc, SimG4FullSimActions, SimG4UserLimitPhysicsList, SimG4UserLimitRegion
from GaudiKernel.SystemOfUnits import mm


# giving the names of tools will initialize the tools of that type
geantservice = SimG4Svc("SimG4Svc", detector='SimG4DD4hepDetector', physicslist="SimG4FtfpBert"
                        )


# Geant4 algorithm
# Translates EDM to G4Event, passes the event to G4, writes out outputs via tools
from Configurables import SimG4Alg, SimG4PrimariesFromEdmTool

particle_converter = SimG4PrimariesFromEdmTool("EdmConverter")
particle_converter.GenParticles.Path = "allGenParticles"


geantsim = SimG4Alg("SimG4Alg",
                    outputs= [
        ],
                    eventProvider=particle_converter)


# PODIO algorithm
from Configurables import PodioOutput
out = PodioOutput("out",
                  filename="mdireader_test_out.root",
                  OutputLevel=INFO)
out.outputCommands = ["keep *"]

# ApplicationMgr
from Configurables import ApplicationMgr
ApplicationMgr( TopAlg = [mdi_converter, geantsim, out],
                #TopAlg = [mdi_converter, out], 
                EvtSel = 'NONE',
                EvtMax   = 1,
                # order is important, as GeoSvc is needed by SimG4Svc
                ExtSvc = [podioevent, geoservice, geantservice],
                #ExtSvc = [podioevent],
                OutputLevel=INFO
               )
