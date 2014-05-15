////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013
///The RNEvt2Root class converts an EVT file to a ROOT File with the data 
///structure of a RNEvent. Adapted from NSCL evt2root to unpack from the new
///RING MASTER buffer structure.                             
////////////////////////////////////////////////////////////////////////

/***************************************
update 08/20: Got rid of the tree structure for the unpack2root 
using RN_modules. also got rid of the need for a "config file".  Changes to the sorting method should just be coded into the init(),SortGeoChan() methods

********************************************/

#ifndef __CONVERT
#define __CONVERT


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
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCutG.h>
#include <TSelector.h>
#include <map>
#include <list>
#include <utility>

#include "DataFormat.h"
#include "sak_ReadBuffer.hpp"
#include "RN_Module.hpp"

class RN_EventProcessor():public RN_BaseClass{
 protected:
 TFile* RootFile;
 TTree* DataTree;
 TTree* ScalerTree;
 TChain* fChain;

 //reading from evt file
 unsigned int BufferWords; //number of short length words, each 2 bytes
 unsigned int BufferBytes; //number of bytes to be stored
 unsigned int BufferType;
 unsigned int NBuffers; 
 int BufferPhysics;
 std::ifstream evtfile;
 std::ofstream logfile;
 unsigned short * buffer;
 int timer; 
 ScalerNames scaler_names;
 ScalerValues scaler_values;

 int ExtractRingBuffer();
 int UnpackBeginRun();
 int UnpackEndRun();
 int UnpackScalers();
 int UnpackPhysicsEvent();
 int UnpackPhysicsEventCounter();
 int UnpackUndefined();
 bool InitStack(const std::string& configfile);
 void ResetTreeParameters() = 0;
 virtual bool Begin() = 0;
 virtual bool Process() = 0;
 virtual bool ProcessFill() = 0;
 virtual bool Terminate() = 0;
 virtual void Loop(Long64_t start = 0, Long64_t evnum = 0);
 virtual void AddTree(TString a){fChain->Add(a);}
 virtual void Init(TString file);
 Long64_t TotEntries() const{return fChain->GetEntries();} 
 

 public:



};

typedef std::vector<std::string> ScalerNames;
typedef std::vector<Double32_t> ScalerValues;
typedef std::vector<Double32_t>::iterator ScalerValueIterator;

namespace unpacker{
  extern TFile* RootFile;
  extern TTree* DataTree;
  extern TTree* ScalerTree;
 
  extern int timer; 
 
  extern Int_t Event[3];
  extern RN_Module_Stack gModule_stack;
 
  extern TBranch        *b_Event;  //!
  
  extern ScalerNames scaler_names;
  extern ScalerValues scaler_values;


  int Convert2Root(std::vector<std::string>&run_number,std::string data_dir,std::string output_file);
 int Convert2Root(const std::string&,std::string output_file);
  bool InitStack(const std::string& configfile);
  void ResetTreeParameters();

  int ExtractRingBuffer();
  int UnpackBeginRun();
  int UnpackEndRun();
  int UnpackScalers();
  int UnpackPhysicsEvent();
  int UnpackPhysicsEventCounter();
  int UnpackUndefined();



}

#endif
