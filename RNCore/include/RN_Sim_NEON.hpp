/**********************************************************
//Class: RN_Sim
//
//Author: Sean Kuvin
//
//RN_Sim is an abstract class which simply takes simulated kinematics
//of a transfer reaction and checks if any of the detectors
//defined in RN_Root.cpp are hit. The user needs to supply
//their own class which inherits from RN_Sim and defines
//the methods Print(), Begin(), Process(), ProcessFill(), and Terminate()   
*********************************************************/

#ifndef _RNSIMNEON__H
#define _RNSIMNEON__H
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

class RN_Sim_NEON:public RN_BaseClass{

private:
  Long64_t fTotEvents;
  Double32_t fEDeposited;
  Double32_t fNTOF;
  Double32_t fNCM;
  Double32_t fSiliconResolution;
  Double32_t fProtonThreshold;
  TVector3 fBeamSpot;

 
protected:
  TTree *fSimTree;
  TFile *fSimFile;
  ofstream fSimLog;
  TString fOption;
  std::vector<Int_t> fNeutronIn;
  std::vector<Int_t> fNeutronDetected;
  std::vector<Int_t> fProtonIn;
  std::vector<Int_t> fProtonIn_NeutDet;

  // JAB
  std::vector<Int_t> fIC_In;

  
public:

  RN_Sim_NEON(const std::string &name = "sim");
  ~RN_Sim_NEON(){};


  Long64_t TotEvents(){return fTotEvents;}
  Double32_t NCM(){return fNCM;}
  Double32_t NTOF(){return fNTOF;}
  Double32_t EDeposited(){return fEDeposited;}
  TVector3 BeamSpot(){return fBeamSpot;}
  Double32_t SiliconResolution(){return fSiliconResolution;}
  Double32_t ProtonThreshold(){return fProtonThreshold;}

  void Init();
  void Reset();
  void Loop();

  int GenerateEvents(Long64_t evnum,TString options="");

  void LoadConfig(std::string input);

  virtual void Print() = 0;
  virtual int Begin() = 0;
  virtual int Process() = 0;
  virtual int ProcessFill() = 0;
  virtual int Terminate() = 0;


};



#endif
