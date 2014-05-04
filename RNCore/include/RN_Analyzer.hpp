/*************************************************************************
Class: RN_Analyzer
Author: Sean Kuvin

Analyzer class follows the form of a simple MakeClass designed to
read the tree structure of data extracted by the Unpack2Root Method.
Approach this class in two ways,first by overriding the Begin(),Process(), 
and Terminate() methods to create, fill and write histograms or new
trees. The second by instantiating an object of RN_Analyzer and calling the
GetDetectorEntry() function   
from a Loop function in a different application. 
Add user analyzers to the "analyzer" list(which is global in RN_Root) 

*******************************************************************/


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
#include <TList.h>
#include <TCollection.h>

#include <map>
#include "RN_EventProcessor.hpp"


class RN_Analyzer:public RN_EventProcessor{
public:

 //constructors
  
  RN_Analyzer(const std::string&a="",const std::string& b="");
  virtual ~RN_Analyzer();

  // Methods.
  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual bool Terminate();
  virtual void Reset();
  virtual int GetDetectorEntry();

  ClassDef(RN_Analyzer,1);
};

class RN_Analyzer_Stack:public RN_BaseClass_Stack{
protected:

public: 
  RN_Analyzer_Stack(const TString& name=""):RN_BaseClass_Stack(name){};
  ~RN_Analyzer_Stack(){}
  
  virtual UInt_t Add(RN_BaseClass * base);
  virtual bool Begin();
  virtual bool Process(); 
  virtual bool ProcessFill();
  virtual bool Terminate();

 
  ClassDef(RN_Analyzer_Stack,1);
};



#endif
