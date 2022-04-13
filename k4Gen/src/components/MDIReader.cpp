
#include "MDIReader.h"

#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IEventProcessor.h"

#include "edm4hep/MCParticleCollection.h"

#include <math.h>
#include <algorithm>
#include <iostream>

DECLARE_COMPONENT(MDIReader)

MDIReader::MDIReader(const std::string& name, ISvcLocator* svcLoc):
GaudiAlgorithm(name, svcLoc),
  m_filename() {

  declareProperty("MDIFilename", m_filename="", "Name of the MDI file to read");
  declareProperty("GenParticles", m_genphandle, "Generated particles collection (output)");
  declareProperty("CrossingAngle",xing,"Half the crossing angle beam in [rad]");
  declareProperty("LongitudinalCut",cut_z,"the value for cut_z used in GP++ in [um]");
}

StatusCode MDIReader::initialize()
{
  StatusCode sc = GaudiAlgorithm::initialize();

  m_input.open(m_filename.c_str(), std::ifstream::in);

  if ( !m_input.good() )   {
    error() << "Failed to open input stream:"+m_filename << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

StatusCode MDIReader::execute()
{
  // First check the input file status
  if(m_input.eof())
    {
      error() << "End of file reached" << endmsg;
      return StatusCode::FAILURE;
    }
  
  //  Loop over particles
  int ISTHEP = 1;   // status code
  int IDHEP;    // PDG code
  int CHARGE;   // charge
  //int JMOHEP1;  // first mother
  //int JMOHEP2;  // last mother
  //int JDAHEP1;  // first daughter
  //int JDAHEP2;  // last daughter
  double PHEP1; // px in GeV/c
  double PHEP2; // py in GeV/c
  double PHEP3; // pz in GeV/c
  double PHEP4; // energy in GeV
  double PHEP5; // mass in GeV/c**2
  double VHEP1; // x vertex position in mm
  double VHEP2; // y vertex position in mm
  double VHEP3; // z vertex position in mm
  double VHEP4 = 0; // production time in mm/c

  //these variables do not end up in the EDM format
  double process; // 1=Breit-Wheeler 2=Bethe-Heitler 3=Landau-Lifshitz
  double trash;   // trash variable
  double id_ee;   // same id means they are a pair
  double temp_x,temp_y,temp_z,temp_px, temp_py,temp_pz,temp_e;

  std::cout<<"The crossing angle is "<<xing<<" [rad]"<<std::endl;
  edm4hep::MCParticleCollection* particles = new edm4hep::MCParticleCollection();


  PHEP5 = 5.11e-4;
  while(m_input.good())
    {
      m_input >> PHEP4
	      >> PHEP1 >> PHEP2 >> PHEP3
	      >> VHEP1 >> VHEP2 >> VHEP3
	      >> process >> trash >> id_ee;

      std::cout<<PHEP4<<" "<<sqrt(PHEP1*PHEP1 + PHEP2*PHEP2 + PHEP3*PHEP3)<<" "<<sqrt((PHEP1*PHEP1 + PHEP2*PHEP2 + PHEP3*PHEP3)*PHEP4*PHEP4 + PHEP5*PHEP5)<<std::endl;
      
      if(m_input.eof())break;
      else if(!m_input.good())
	{
	  std::cout<<"oopsie doopsie"<<std::endl;
	  error() << "End of file reached before reading all the hits" << endmsg;
	  return StatusCode::FAILURE;
	}

      IDHEP = 11;
      CHARGE = -1;
      if(PHEP4<0){
	IDHEP=-11;
	CHARGE=1;
      }

      VHEP1 *=1e-9; //convert from nm to m
      VHEP2 *=1e-9; //convert from nm to m
      VHEP3 *=1e-9; //convert from nm to m
      
      //---boost section
      PHEP4 = abs(PHEP4);
      temp_x  = cut_z*1e-6*tan(xing) + VHEP1*sqrt(1+pow(tan(xing),2));
      temp_px = PHEP4*tan(xing) + PHEP1*PHEP4*sqrt(1+pow(tan(xing),2));
      temp_e  = abs( PHEP4*sqrt(1+pow(tan(xing),2)) + PHEP1*PHEP4*tan(xing) );

      VHEP1 = temp_x;
      PHEP1 = temp_px;
      PHEP2 = PHEP2*PHEP4;
      PHEP3 = PHEP3*PHEP4;
      PHEP4 = temp_e;
      
      //---end boost section
      
      VHEP1 *=1e3; //convert from m to mm
      VHEP2 *=1e3; //convert from m to mm 
      VHEP3 *=1e3; //convert from m to mm

      //boost to lab frame
      
      VHEP1=0;
      VHEP2=0;
      VHEP3=0;
      edm4hep::MCParticle particle = particles->create();

      particle.setPDG(IDHEP);
      particle.setCharge(CHARGE);
      particle.setGeneratorStatus(ISTHEP); 
      particle.setMomentum({
	  (float) (PHEP1),
	  (float) (PHEP2),
	  (float) (PHEP3),
	});
      particle.setMass(PHEP5);
      particle.setVertex({
                          VHEP1,
                          VHEP2,
                          VHEP3,
                        });
      particle.setTime(VHEP4);
    }

    m_genphandle.put(particles);
  return StatusCode::SUCCESS;
}

StatusCode MDIReader::finalize() 
{
  m_input.close();
  std::cout<<"Fine di MDIreader"<<std::endl;
  return GaudiAlgorithm::finalize();
}
