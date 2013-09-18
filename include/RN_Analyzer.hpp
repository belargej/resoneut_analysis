/////////////////////////////////////////////////////////////////////////////////////
//** Analyzer class follows the form of a simple MakeClass designed to
//** read the tree structure of data extracted by the Unpack2Root Method.
//** Approach this class in two ways,first by overriding the Begin(),Process(), 
//** and Terminate() methods to create, fill and write histograms or new
//** trees. The second by instantiating an object of RN_Analyzer and calling the
//** GetDetectorEntry() function   
//** from a Loop function in a different application. 
//
//
//                 Author: Sean Kuvin                             
//***********************************************************************************/


#ifndef RESONEUT_ANALYZER_H
#define RESONEUT_ANALYZER_H

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
#include "RN_IonChamber.hpp"
#include "RN_NaIArray.hpp"
#include "RN_Root.hpp"


class RN_Analyzer {
public:

  //constructors
  
  RN_Analyzer();
  virtual ~RN_Analyzer();

  // Declaration of leaf types

  //Make more of these if more modules are added to the ResoNEUT setup
  //even if you load an old file where these modules are not present
  //if you refer to the Init(), the modules will not be aligned to a
  //branch address in this case

  Float_t       ADC1[32];
  Float_t       ADC2[32];
  Float_t       ADC3[32];
  Float_t       ADC4[32];
  Float_t       ADC5[32];
  Float_t       ADC6[32];
  Float_t       ADC7[32];
  Float_t       TDC1[32];
  Float_t       TDC2[32];
  Float_t       TDC3[32];
  Float_t       TDC4[32];
  Float_t       QDC1[32];
  Float_t       QDC2[32];
  
  // List of branches
  TBranch        *b_ADC1;   //!
  TBranch        *b_ADC2;   //!
  TBranch        *b_ADC3;   //!
  TBranch        *b_ADC4;   //!
  TBranch        *b_ADC5;   //!
  TBranch        *b_ADC6;   //!
  TBranch        *b_ADC7;   //!
  TBranch        *b_TDC1;   //!
  TBranch        *b_TDC2;   //!
  TBranch        *b_TDC3;   //!
  TBranch        *b_TDC4;   //!
  TBranch        *b_QDC1;   //!
  TBranch        *b_QDC2;   //!
 
  //ROOT Initializations
  TChain * fChain;
  TRandom3 myRnd;
  
  // Methods.
  void LoadVariableFile(const std::string& f){DetVar.LoadParams(f);}
  void SetCalibrations();
  virtual void Begin(){};
  virtual void Loop(Long64_t start=0,Long64_t evnum=0);
  virtual void Process(){};
  virtual void Terminate(){};
  Long64_t TotEntries() const{return fChain->GetEntries();} 
  void AddTree(TString a){fChain->Add(a);}
  virtual void Init(TString rootfile);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetEntry(entry, getall) : 0; }
  virtual int GetDetectorEntry(Long64_t entry,Int_t getall=0);
  virtual void ApplyCalibrations();


  ClassDef(RN_Analyzer,0);
};



#endif
