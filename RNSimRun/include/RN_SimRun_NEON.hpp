#ifndef _RNSIMRUNNEON__H
#define _RNSIMRUNNEON__H
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
#include <TH2.h>

#include "RN_Sim_NEON.hpp"
#include "RN_VariableMap.hpp"
#include "RN_Particle.hpp"
#include "RN_SimPhysics.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_EnergyLoss.hpp"
#include "sak_ReadBuffer.hpp"

class RN_SimRun_NEON:public RN_Sim_NEON{
private:
  Double32_t fErProton;
  Double32_t fExHeavy;
  Double32_t fNCM_tree;
  TVector3 fICPos;
  TLorentzVector fNeutReco;
  TLorentzVector fFragmentReco;
public:

  Double32_t QVal_FragGuess;
  Double32_t QVal_FragReal;

  Double32_t QVal_InvMass;
  Double32_t QVal_It;
  Double32_t RecoilKinEn;
  Double32_t RecoilTheta;
  Double32_t NeutronEn;
  Double32_t NeutronTheta;
  Double32_t NeutronPhi;
  Double32_t NeutronThetaCM;
  Double32_t NeutronThetaCMSean;
  Double32_t NeutronMass;
  Double32_t ProtonThetaCM;
  
  Double32_t SiEn;
  Double32_t ICEn;
  Double32_t SiAng;
  Double32_t ICAngTheta;
  Double32_t ICAngPhi;

  Double32_t IterFragEnergy;


  RN_SimRun_NEON();
 
  virtual  void Print();
  virtual int Begin();
  virtual int Process();
  virtual int ProcessFill();
  virtual int Terminate();
  int SimRun_Reset();
  

};

 

#endif
