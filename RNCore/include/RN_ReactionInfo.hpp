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

class RN_DecayChannel;
class RN_PrimaryReaction;
class RN_ReactionInfo;


class RN_PrimaryReaction:public RN_BaseClass{
private:
protected:
  RN_Particle pBeam;
  RN_Particle pTarget;
  RN_Particle pRecoil;
  RN_Particle pFragment;
  Double32_t fBeamEnergy;
  Double32_t fBeamELoss;
  Double32_t E_fragment_est;
  sim::RN_AngularDistribution fDWBA;
  TList *fDecayChannels;
public:

  RN_PrimaryReaction();
  RN_PrimaryReaction(TString beam, 
		     TString target, 
		     TString recoil, 
		     TString fragment);

  void SetReaction(TString beam,
		   TString target,
		   TString recoil,
		   TString fragment);

  void SetReaction(TString beam,
		   TString target,
		   TString recoil,
		   TString fragment,
		   TString product,
		   TString heavy);  
  void SetAngularDistribution(const std::string filename);
  bool GenerateSimEvent();
  bool GenerateDecayEvents();

  inline Double32_t BeamEnergy() const {return fBeamEnergy;}
  inline Double32_t BeamEnergy_Est() const {return (fBeamEnergy - (0.5 * fBeamELoss));}
  inline Double32_t BeamELoss() const {return fBeamELoss;}
  inline Double32_t M_Beam()const{return pBeam.M();} 
  inline Double32_t M_Target()const{return pTarget.M();}
  inline Double32_t M_Recoil()const{return pRecoil.M();}
  inline Double32_t M_Fragment()const{return pFragment.M();}
  inline Double32_t Ex_Fragment()const{return pFragment.Ex();}
  Double32_t M_Decay_Product(int i=0);
  Double32_t M_Heavy_Decay(int i=0);
  inline RN_Particle& Beam(){return pBeam;}
  inline RN_Particle& Target(){return pTarget;}
  inline RN_Particle& Recoil(){return pRecoil;}
  inline RN_Particle& Fragment(){return pFragment;}
  RN_Particle& DecayProduct(int i=0);
  RN_Particle& HeavyProduct(int i=0);
  inline TLorentzVector& BeamLV(){return pBeam.LV;}
  inline TLorentzVector& TargetLV(){return pTarget.LV;}
  inline TLorentzVector& RecoilLV(){return pRecoil.LV;}
  inline TLorentzVector& FragmentLV(){return pFragment.LV;}
  TLorentzVector& DecayProductLV(int i=0);
  TLorentzVector& HeavyDecayLV(int i=0);
  inline Double32_t E_Fragment() const {return E_fragment_est;}
  Double32_t DecayQValueExact();
  Double32_t DecayQValueEstimate();
  Double32_t RecoilQValue(const double& dz, const double& tof, const double&nKE, const double&hiKE);

  void SetEFragmentGuess(const Double32_t& efrag ){E_fragment_est = efrag;}
  void SetCalibrations(RN_VariableMap&detvar);
  void Reset();

  TList * GetListOfDecayChannels(){return fDecayChannels;}
  RN_DecayChannel *AddDecayChannel(TString decay);
  RN_DecayChannel *AddDecayChannel(TString decay,TString heavy);
				  
};

class RN_DecayChannel:public RN_BaseClass{
private:
  RN_PrimaryReaction* fMotherReaction;
  RN_Particle pProduct;
  RN_Particle pHIDecay;
  
  static int decayID;
 
protected:
  TList * fParticleList;
  Double32_t fExEnergy;
  
public:
  RN_DecayChannel(){};
  RN_DecayChannel(RN_PrimaryReaction* parent, TString decay, TString heavy);
  Double32_t M_Decay_Product()const{return pProduct.M();}
  Double32_t M_Heavy_Decay()const{return pHIDecay.M();}
  void Init(RN_PrimaryReaction* parent,TString decay,TString heavy);
  bool GenerateDecayEvent();

  TLorentzVector& DecayProductLV(){return pProduct.LV;}
  TLorentzVector& HeavyDecayLV(){return pHIDecay.LV;}
  RN_Particle& DecayProduct(){return pProduct;}
  RN_Particle& HeavyDecay(){return pHIDecay;}

};

//RN_ReactionInfo:
//stack(TList) of primary reactions which in turn have TLists of decay channels. By Looping over all Primary Reactions and Looping over all decay channels we can quickly produce Kinematic Curves and simulation data for a large set of possible reactions.
class RN_ReactionInfo:public RN_BaseClass_Stack{
  
  

};

#endif
