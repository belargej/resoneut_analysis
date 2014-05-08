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
