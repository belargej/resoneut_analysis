////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////


#ifndef RESONEUT_EVENT_H
#define RESONEUT_EVENT_H

//Methods for the RNEvent class.

//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 

//ROOT libraties
#include <TRandom3.h>
#include <TH1.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TChain.h>
#include <TBranch.h>


#include <map>
#include "RN_Unpack2Root.hpp"
#include "RN_NeutDetectorArray.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_Timing.hpp"
#include "RN_VariableMap.hpp"

class RN_Analyzer {
public:

  //constructors
  
  RN_Analyzer();
  virtual ~RN_Analyzer();

  //Detectors 
  RN_NeutDetectorArray Narray;
  RN_NeutCollection neut;
  RN_S2Collection si_;
  RN_S2ClusterCollection si_cluster_;
  RN_RFCollection rftime;
  //  RN_S2Detector si_a;
  //RN_S2Detector si_b;
  //RN_S2Cluster si_cluster_b;
  //RN_RFTime rftime;
  RN_VariableMap DetVar;

  Int_t calibrated;


  // Declaration of leaf types
  RN_module       *ADC1;
  RN_module       *ADC2;
  RN_module       *ADC3;
  RN_module       *ADC4;
  RN_module       *ADC5;
  RN_module       *TDC1;
  RN_module       *TDC2;
  RN_module       *TDC3;
  RN_module       *QDC1;
  RN_module       *QDC2;
  
  // List of branches
  TBranch        *b_ADC1;   //!
  TBranch        *b_ADC2;   //!
  TBranch        *b_ADC3;   //!
  TBranch        *b_ADC4;   //!
  TBranch        *b_ADC5;   //!
  TBranch        *b_TDC1;   //!
  TBranch        *b_TDC2;   //!
  TBranch        *b_TDC3;   //!
  TBranch        *b_QDC1;   //!
  TBranch        *b_QDC2;   //!
 
  //ROOT Initializations
  TChain * fChain;
  TRandom3 myRnd;
  
  // Methods.
  void LoadVariableFile(const std::string& f){DetVar.LoadParams(f);}
  virtual void Loop(){};
  virtual void Process(){};
  Long64_t TotEntries() const{return fChain->GetEntries();} 
  void AddTree(TString a){fChain->Add(a);}
  virtual void Init(TString rootfile);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetEntry(entry, getall) : 0; }
  virtual int GetDetectorEntry(Long64_t entry,Int_t getall=0);



  ClassDef(RN_Analyzer,0);
};



#endif
