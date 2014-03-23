#ifndef _RNSIM__
#define _RNSIM__
//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <TTree.h>
#include <TLorentzVector.h>


#include "RN_SiliconDetectors.hpp"
#include "RN_VariableMap.hpp"
#include "RN_SimPhysics.hpp"
#include "RN_Particle.hpp"
#include "RN_MassTable.hpp"
#include "sak_ReadBuffer.hpp"

namespace sim{

class RN_Sim{

private:
  RN_AngularDistribution nDWBA;

public:

  std::string option;
  int def;
  
  RN_Sim(){};
  ~RN_Sim(){};

  void Init();
  void Reset();
  void SetAngularDistribution(std::string filename);
  int GenerateSingleParticleEvent(Long64_t evnum);
  int GenerateEvents(Long64_t evnum,std::string options);
  double QValue(const double,const double,double&,double&);
  void Loop(Long64_t evnum,std::string options);
  void StartRun(std::string input);
  void initHists();
  void WriteOut();
  void FillHistograms();
};


  extern RN_ParticleGun *particlegun;
  extern std::vector<Int_t> NeutronIn;
  extern std::vector<Int_t> NeutronDetected;
  extern std::vector<Int_t> ProtonIn;
  extern std::vector<Int_t> ProtonIn_NeutDet;
  


}


#endif
