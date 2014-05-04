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

#ifndef __RNEVENTPROCESSOR_H
#define __RNEVENTPROCESSOR_H


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
#include <TChain.h>

#include "RN_BaseClass.hpp"

typedef std::vector<std::string> ScalerNames;
typedef std::vector<Double32_t> ScalerValues;
typedef std::vector<Double32_t>::iterator ScalerValueIterator;


class RN_EventProcessor:public RN_BaseClass{
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
  Int_t Event[3];

  int ExtractRingBuffer();
  int UnpackBeginRun();
  int UnpackEndRun();
  int UnpackScalers();
  int UnpackPhysicsEvent();
  int UnpackPhysicsEventCounter();
  int UnpackUndefined();
  virtual void ResetTreeParameters();
  virtual bool Begin();
  virtual bool Process(){return 1;};
  virtual bool ProcessFill(){return 1;};
  virtual bool Terminate(){return 1;};
  
public:
  TBranch * b_Event;
  RN_EventProcessor(const std::string&a="",const std::string&b=""):RN_BaseClass(a.c_str(),b.c_str()){}
  
  
  virtual void Loop(Long64_t start = 0, Long64_t evnum = 0);
  int Convert2Root(std::vector<std::string>&run_number,std::string data_dir,std::string output_file); 
  int Convert2Root(const std::string& name,std::string output_file);
  int AttachFromEVT(const TString& evtfilename);

  void AddTree(TString a){fChain->Add(a);}
  virtual void InitRootFile(TString file);
  virtual bool InitStack(const std::string& configfile);
  Long64_t TotEntries() const{return fChain->GetEntries();} 
  virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetEntry(entry, getall) : 0; }
  virtual int GetDetectorEntry(){return 1;};

  ClassDef(RN_EventProcessor,1);
};

extern RN_EventProcessor gEventProcessor;

#endif
