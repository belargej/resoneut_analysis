/***********************************************************/
//Class: NamedTree_Analyzer
//
//Author:Joe Belarge
//
// NamedTree_Analyzer class is to be inserted in the analyzer
// list for producing a next level tree in your root file. 
// It will be made of data that has been sorted out of the
// ADC,QDC,TDC format and into the SiA,SiB,Neut0... format.
// It can be added as another tree to a root file, and it will
// have a base structure similar to the NewTree_Analyzer class,
// so that a new set of analyzers can be designed to be used
// on this tree, and can then be used as wished.
/***********************************************************/

#ifndef _NAMEDTREEANALYZER_H
#define _NAMEDTREEANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Root.hpp"
#include "TTree.h"

class NamedTree_Analyzer:public RN_Analyzer{
private:
  TTree *fNamedTree;
  TVector3 fBeamSpot;
  TVector3 fSiAVector;
  TVector3 fSiBVector;
  Double32_t fSiAAngle;
  Double32_t fSiBAngle;
  Double32_t fErProton;
  Double32_t fDispersion;

  bool OneHitRecon;
  bool TwoHitRecon;
  
  Double32_t ICOff;
public:  
  NamedTree_Analyzer();
  virtual ~NamedTree_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Reset();
  void SetDispersionScaleFactor(double factor);
  void TwoHitReconOn(){TwoHitRecon = true;};
  void SetICOffset(double Off){ICOff=Off;};


  // Branch Variables:
  Double32_t Energy_A;
  Double32_t Energy_B;
  Double32_t Energy_AB;
  Double32_t RawEnergyF_A;
  Double32_t RawEnergyF_B;
  Double32_t RawEnergyB_A;
  Double32_t RawEnergyB_B;
  Double32_t A_X;
  Double32_t A_Y;
  Double32_t B_X;
  Double32_t B_Y;
  Double32_t FrontChan_A;
  Double32_t FrontChan_B;
  Double32_t BackChan_A;
  Double32_t BackChan_B;
  Double32_t Si_Time_A;
  Double32_t Si_Time_B;
  Double32_t TotEn_IC;
  Double32_t ICEn_Cal;
  Double32_t ICEn_Cal_InDet;
  Double32_t ICdE_Cal;
  Double32_t ICE_Cal;
  Double32_t ICETot_WalkBack;
  Double32_t dE_IC;
  Double32_t IC_X;
  Double32_t IC_Y;
  Double32_t IC_X_Raw;
  Double32_t IC_Y_Raw;
  Double32_t ThetaLab_A;
  Double32_t ThetaLab_B;
  Double32_t PhiLab_A;
  Double32_t PhiLab_B;
  Double32_t ICTheta;
  Double32_t ICPhi;
  Double32_t QVal_Theta_A;
  Double32_t QVal_Theta_B;
  Double32_t QVal_NoIt;
  Double32_t RecEn_Iter;
  Double32_t ICEn_CenterOfTarget;

  Double32_t NaIEnergy;
  Double32_t NaIPos;
  Double32_t NaIDet;
  Double32_t NaITime;
  Double32_t NaIMult;
  Double32_t NaIEnergy_First;
  Double32_t NaIDet_First;
  Double32_t NaITime_First;
  Double32_t NaIEn_Sum;
  Double32_t NaI_Ang;
  Double32_t NaIEn_DC;
  

  Double32_t QVal_InvMass;
  Double32_t QVal_Neut;
  Double32_t RecoilKinEn;
  Double32_t RecoilTheta;
  Double32_t NeutronEn;
  Double32_t NeutronTheta;
  Double32_t NeutronThetaCM;
  Double32_t ProtonThetaCM;

  Double32_t SpotCorr_QVal_ThA;
  Double32_t SpotCorr_ThA;
  
  Double32_t NeutLong_0;
  Double32_t NeutLong_1;
  Double32_t NeutLong_2;
  Double32_t NeutLong_3;
  Double32_t NeutLong_4;
  Double32_t NeutLong_5;
  Double32_t NeutLong_6;
  Double32_t NeutLong_7;
  Double32_t NeutLong_8;
  Double32_t NeutLong_9;
  Double32_t NeutShort_0;
  Double32_t NeutShort_1;
  Double32_t NeutShort_2;
  Double32_t NeutShort_3;
  Double32_t NeutShort_4;
  Double32_t NeutShort_5;
  Double32_t NeutShort_6;
  Double32_t NeutShort_7;
  Double32_t NeutShort_8;
  Double32_t NeutShort_9;
  Double32_t RunNumber;
  

  Int_t ZeroCounter;
  Int_t FillCounter;





  ClassDef(NamedTree_Analyzer,0);
};





#endif
