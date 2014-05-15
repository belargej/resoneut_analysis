#ifndef _RNREACTIONINFO_H
#define _RNREACTIONINFO_H
//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

//ROOT libraties
#include <TROOT.h>
#include <TRint.h>
#include <TH1.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCutG.h>

#include <map>
#include "RN_Particle.hpp"
#include "RN_SimPhysics.hpp"


class RN_ReactionInfo:public RN_BaseClass{
private:
protected:
  Double32_t mArray[6];
  TString fNames[6];
  Double32_t fBeamEnergy;
  Double32_t fBeamELoss;
  Double32_t fhi_ex_set;
  Double32_t fd_ex_set;
  Double32_t fE_fragment_est;//for q_value calculations
  sim::RN_AngularDistribution fDWBA;
  bool fIsSet;
  
  bool GenerateDecayEvent(); //protected as it requires a succesful GenerateSimEvent()
  
public:
  TLorentzVector fLVarray[6];

  RN_ReactionInfo(const std::string & name = "");
  ~RN_ReactionInfo(){}

  //SetReaction, SetAngular Distribution and the beam parameters
  //must be set before using GenerateSimEvent();
  bool SetReaction(const std::string & beam, 
		   const std::string & target, 
		   const std::string & recoil, 
		   const std::string & fragment,
		   const std::string & decay,
		   const std::string & heavydecay);
  void SetAngularDistribution(const std::string& filename);
  //the beam parameters can be set by these methods
  void SetCalibrations(RN_VariableMap&detvar);  
  void SetBeamEnergy(Double32_t energy){fBeamEnergy = energy;}
  void SetBeamELoss(Double32_t eloss){fBeamELoss = eloss;}
  void SetEFragmentEst(Double32_t efragest){fE_fragment_est = efragest;}
  void SetHiEx(Double32_t hi_ex_set){fhi_ex_set = hi_ex_set;}
  void SetDecay_Ex(Double32_t d_ex_set){fd_ex_set = d_ex_set;}

  
  Double32_t GenerateSimEvent(); //returns the CM angle of the recoil


  //Get Beam Parameters
  inline Double32_t BeamEnergy() const {return fBeamEnergy;}
  inline Double32_t BeamEnergy_Est() const {return (fBeamEnergy - (0.5 * fBeamELoss));}
  inline Double32_t BeamELoss() const {return fBeamELoss;}
  inline Double32_t E_Fragment() const {return fE_fragment_est;}
  inline Double32_t Hi_Ex_Set() const {return fhi_ex_set;}
  inline Double32_t D_Ex_Set() const {return fd_ex_set;}


  //Get Masses
  inline const Double32_t& M_Beam()const{return mArray[0];} 
  inline const Double32_t& M_Target()const{return mArray[1];}
  inline const Double32_t& M_Recoil()const{return mArray[2];}
  inline const Double32_t& M_Fragment()const{return mArray[3];}
  inline const Double32_t& M_Decay_Product() const{return mArray[4];}
  inline const Double32_t& M_Heavy_Decay()const {return mArray[5];}  


  //Get Particle names
  inline const TString& BeamName() const {return fNames[0];}
  inline const TString& TargetName() const {return fNames[1];}
  inline const TString& RecoilName() const {return fNames[2];}
  inline const TString& FragmentName() const {return fNames[3];}
  inline const TString& DecayProductName() const {return fNames[4];}
  inline const TString& HeavyDecayName() const {return fNames[5];}
 

  //Get LorentzVectors(after GenerateSimEvents())
  inline const TLorentzVector& BeamLV() const {return fLVarray[0];}
  inline const TLorentzVector& TargetLV() const {return fLVarray[1];}
  inline const TLorentzVector& RecoilLV() const {return fLVarray[2];}
  inline const TLorentzVector& FragmentLV() const {return fLVarray[3];}
  inline const TLorentzVector& DecayProductLV() const { return fLVarray[4];}
  inline const TLorentzVector& HeavyDecayLV() const { return fLVarray[5];}

  //get kinematic curves for plotting/matching over data
  TGraph GetCurve(int Points, const double& hi_ex_set);
  TGraph GetCurve(int Points);
  TGraph GetSecondaryDecayCurve(int Points,const double & hi_ex_set,const double& decay_ex_set);
  TGraph GetSecondaryDecayCurve(int Points);

  //Use masses to estimate qval
  Double32_t DecayQValueExact();
  Double32_t DecayQValueEstimate();
  Double32_t DecayQValueEstimate(const double& decay_energy, const double& decay_theta);
  Double32_t RecoilQValue(const double& dz, const double& tof, double&nKE, double&hiKE);

  void Reset(); //reset LorentzVectors but leave masses
  void Clear(); //reset both masses and LVs and isSet=0
  bool IsSet(); //check if masses and a beam energy is set and set to 1

  ClassDef(RN_ReactionInfo,1);
};
/*
class RN_ReactionInfo_Stack:public RN_BaseClass_Stack{
private:
public:
  ClassDef(RN_ReactionInfo,1);
};
*/

#endif
