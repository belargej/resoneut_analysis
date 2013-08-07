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

class RN_Sim{

private:
 
public:
  RN_PTerphCollection pterph;
  RN_S2Collection s2_det;
  RN_ParticleCollection plist;
  
  RN_AngularDistribution nDWBA;
  RN_MassTable mtable; 
  RN_VariableMap global;
  double E_deposited;
  double n_tof;
  //variables
  double q1set,q2set,q3set,q4set;
  double hi_ex_set,d_ex_set;
  double beam_energy;
  double beam_eloss;
  double beam_e;
  double fNe,fNt;
  TRandom3 myRnd;//!



  RN_Sim();
  ~RN_Sim(){};

  void Init();
  void LoadVariableFile(std::string a);
  void Reset();
  void SetVariables();
  void SetAngularDistribution(std::string filename);
  int GenerateEvents(Long64_t evnum,std::string options);

};



#endif
