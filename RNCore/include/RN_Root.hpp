#ifndef _RNROOT_CORE_H
#define _RNROOT_CORE_H
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
#include "RN_EventProcessor.hpp"
#include "RN_Particle.hpp"
#include "RN_NeutDetectorArray.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_Timing.hpp"
#include "RN_VariableMap.hpp"
#include "RN_IonChamber.hpp"
#include "RN_NaIArray.hpp"
#include "RN_MassTable.hpp"
#include "RN_TriggerBit.hpp"
#include "RN_Analyzer.hpp"
#include "RN_Module.hpp"
#include "RN_Parameters.hpp"
#include "RN_ReactionInfo.hpp"


const static unsigned int NEUTNUM(16);
const static unsigned int SI_NUM(2);
const static unsigned int NAI_NUM(20);
const static unsigned int TRIGBIT_NUM(5);

//objects here are declared extern so that they will be added to rootcint and be available to any code which imports RN_Root.hpp

extern CAEN_ADC ADC1;
extern CAEN_ADC ADC2;
extern CAEN_ADC ADC3;
extern CAEN_ADC ADC4;
extern CAEN_ADC ADC5;
extern CAEN_ADC ADC6;
extern CAEN_ADC ADC7;
extern CAEN_TDC TDC1;
extern CAEN_TDC TDC2;
extern CAEN_TDC TDC3;
extern CAEN_TDC TDC4;
extern MESY_QDC QDC1;
extern MESY_QDC QDC2;
extern MESY_QDC QDC3;

namespace global{
  extern TRandom3 myRnd;
}

namespace RNROOT{
  //for handling the output of an RNROOT session we use these
  //objects:
  extern TFile *gRootFile;  //set with SetRootOutputFile()
  extern TTree *gNewTree;   //set both with SetRootOutputFileAndTree()
  extern int RN_RootSet;
  /******************************************************/

  //RN_ROOT objects:

  extern RN_VariableMap gVariableMap;
  extern RN_MassTable gMassTable;
  extern RN_Analyzer gMainAnalyzer;
  extern RN_ReactionInfo gReactionInfo;

  /*****************************************************/
  
  //containers for RN_ROOT objects:

  //these RN_BaseClass objects are managed by their own RN_BaseClass_Stacks which are TList (doubly-linked lists to TObject inherited object addresses)
  extern RN_Analyzer_Stack gAnalyzer_stack; //handles RN_Analyzers
  extern RN_Module_Stack gModule_stack; //handles RN_Modules
  extern RN_Parameter_Stack gParameter_stack; //handle RN_Parameters
  // extern RN_ReactionInfo_Stack gReactionInfo_stack;//handle not only the primary reaction of interest but all other potential reactions that the user wants to consider

  //right now I'm using std::vectors to handle the following RN_Baseclass objects (which inherit from TObject).  In the future I may move to using root TLists for these objects too (like I do for the 'stacks' above).  
  
  extern RN_NeutDetectorArray Narray;	     
  extern RN_TriggerBitCollection triggerbit;	     
  extern RN_NeutCollection neut;	     
  extern RN_S2Collection si_;		     
  extern RN_S2ClusterCollection si_cluster_;
  extern RN_SiArray si_array;
  extern RN_RFTime rftime;
  //extern RN_MCPDetector MCP;
  extern RN_IonChamber ic;		     
  extern RN_NaICollection nai;		     
  extern RN_NaIArray nai_array;
  /*******************************************************/

  int Initialize();
  void LoadVariableFile(const std::string & f);
  void SetRootOutputFile(const std::string& filename);
  void SetRootOutputFileAndTree(const std::string & filename, const std::string& treename);
  void SetCalibrations(RN_VariableMap &VarMap = gVariableMap);
  void ResetRNROOTDetectors();
  void ResetRNROOTGlobals();
}



#endif
