#ifndef _RNSIMRUN__
#define _RNSIMRUN__
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

#include "RN_Sim.hpp"
#include "RN_VariableMap.hpp"
#include "RN_Particle.hpp"
#include "RN_SimPhysics.hpp"
#include "RN_SiliconDetectors.hpp"
#include "sak_ReadBuffer.hpp"

class RN_SimRun:public RN_Sim{
private:

public:
  TFile * rootfile;
  TTree * tree;
  int def;
  //declare histograms here
  TH1D* hn_tof;
  TH2D* htof_n;
  TH2D* hE_n;
  TH2D* hE_v_theta;
  TH2D* hT_v_theta;
  TH1D* hn_CM;
  TH2D* hn_CMvLab;
  TH2D* hpos;
  TH1D* hQ;
  TH1D* h_nKE;
  TH1D* h_hiKE;
  



  Long64_t totevents;
  std::string option;

  RN_SimRun();
  void Loop(Long64_t evnum,std::string options);
  void StartRun(std::string input);
  void initHists();
  
  void FillHistograms();
};

#endif
