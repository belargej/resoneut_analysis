///////////////////////////////////////////////////////////////////////
//Class: RN_EnergyLoss
//
// Adapted from ANASEN code method by Joe Belarge and Grigory Rogachev
// This class reads a SRIM Table to calculate energy loss values
//
////////////////////////////////////////////////////////////////////////
#ifndef RN_ENLOSS_H
#define RN_ENLOSS_H


#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "RN_BaseClass.hpp"


namespace eneloss{
  const static int gNumLossMax(1000);
}

/////////////////////
// Class Definition:
class RN_EnergyLoss:public RN_BaseClass
{
protected:
  Double32_t fEnergy[eneloss::gNumLossMax];
  Double32_t fdEdXE[eneloss::gNumLossMax];
  Double32_t fdEdXN[eneloss::gNumLossMax];
  Int_t fNpoints;
  Int_t fNPSteps;

 public:
  RN_EnergyLoss(const std::string&name ="");
  
  /////////////
  // Functions:	
  void Init(const char* LossFileName, double EMax,Int_t steps = 1000);
  void Init(const std::string& projectile,const std::string& target,const std::string&option,const std::string& pressure,const double& EMax, Int_t steps = 1000);
  void Init(const std::string& projectile,const double& target,const std::string&option,const std::string& pressure,const double& EMax, Int_t steps = 1000);
  double GetLossLinear(double Path, double FinalEnergy, double density);
  double GetLossLinear(double Path, double InitialEnergy, double density,double& FinalEnergy);
  void Print();

  ClassDef(RN_EnergyLoss,0);
};


#endif
