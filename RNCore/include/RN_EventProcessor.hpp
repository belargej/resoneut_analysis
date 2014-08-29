/*************************************************************************
Class: RN_EventProcessor
Author: Sean A. Kuvin -2013

RN_EventProcessor handles the Convert2Root methods and also sorting the resulting TTree format.  Since the same variables are used to store the inputs for the TTree when writing, these same variables can be used to store the TTree data when reading.

First, the module stack must be properly configured.  This is done in the RN_Root.cpp file.  Objects of RN_Module-derived classes must be created and then added to the gModule_stack.

ie.

CAEN_ADC ADC1("ADC1",2); // name of module is ADC1 at geoaddress 2
...
//You can create as many modules as you want here, but the stack configuration performed below MUST match the format of the data being processed.
//this depends on the daqconfig.tcl file that was set during the run

void RNROOT::Initialize(){
gModule_stack.Add(&ADC1);
...
}

Now we can use the RN_EventProcessor to create the TFile:

gEventProcessor.Convert2Root("/path/to/data/run-1111-00.evt","run1111.root");

This will convert run-1111-00.evt binary file into run1111.root.

A log file will also be created with the start and stop times(in LINUX-time) including the notifications that a BeginRun() occured and EndRun() event occurred. 
If you get any errors, the most likely culprit is that the particular .evt file you are converting has a different module stack configuration than you provided in RNROOT::Initialize.

You can also use

gEventProcessor.Convert2Root("/path/to/data/","multipleruns.root");

to convert multiple .evt files into a single root file.

You will be prompted for the run numbers and you should give them in the following format:

if it is run-2121-00.evt, you should give:

2121-00
2121-01
2121-02
2122-00
...

RN_BatchMode is a compiled implementation of this Convert2Root class so I recommend using this program to perform the conversion.


Next, the new ROOT file can be sorted using the RN_EventProcessor class by doing:

gEventProcessor.InitRootFile("FileName.root");
gEventProcessor.Loop();

This will loop over all of the entries in the TTree.  The output by default is the number of entries and an update on how many entries have been sorted.
By default, nothing else happens.

This is why you must create a class which inherits from RN_EventProcessor and overrides the GetDetectorEntry() method.  RN_Analyzer is an example of this.
RNROOT::gMainAnalyzer.InitRootFile("FileName.root");
RNROOT::gMainAnalyzer.Loop();

now also sorts the data from the RN_Modules into the RN Detector classes.
Still no histograms or new trees are created from this.

Instead, what you can do is create multiple RN_Analysis-derived classes and add them to the RNROOT::gAnalyzer_stack;

In each of your RN_Analysis-derived classes you set your own Begin(), Process(), ProcessFill() and Terminate().  Each of these methods will be executed during the Loop() method at the appropriate time.

Equivalent to doing a TTree->MakeClass():

you would allocate your histograms in the Begin() method,
Do any extra calculations or filters in the Process() method,
Fill the histograms or new TTrees in the ProcessFill() method,
and finally after the run is finished you can write out the root file etc.

The ProcessFill() method is only executed if ALL of the Process() methods return a True(1) result.  This allows the user to quickly filter all of the histograms they have created by setting a condition in their Process() function to return a 0;  If this condition is not met then no ProcessFills() will be executed in ANY of the RN_Analyzers.

For example(see the RNAnalyzer folder), if I have a Si_Analyzer added to the stack and and an IC_Analyzer.  Each one handles the histograms and cuts specific to that detector alone.
If however, I load the "alphas" cut and turn on the RequireAlphas() switch, then Process() will return a 0 anytime that cut fails.

Since there was a false result from a Process(), neither the ProcessFill() for the Si_Analyzer nor the ProcessFill() for the IC_Analyzer will be executed.
Therefore, every histogram is dependent on the result of that cut.




Previously the unpacking functions to unpack the NSCL ringmaster data format  and create raw ROOT Trees was in RN_Unpack2Root.cpp. Also, sorting the converted root files was done exclusively with RN_Analyzer.
 
****************************************************************************/



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
  static TFile* fgRootFile;
  static TTree* fgDataTree;
  static TTree* fgScalerTree;
  TChain* fChain;


  //reading from evt file
  unsigned int fBufferWords; //number of short length words, each 2 bytes
  unsigned int fBufferBytes; //number of bytes to be stored
  unsigned int fBufferType;
  unsigned int fNBuffers; 
  int fBufferPhysics;
  std::ifstream fEvtFile;
  std::ofstream fLogFile;
  unsigned short * fBuffer;
  int fTimer; 
  Float_t fScalerValues[32]; //CAEN Scaler Module 32 channels  
  Double32_t fScalerSums[32]; //CAEN Scaler Module 32 channels  
  Int_t fEvent[3];  
  // ScalerNames scaler_names;
  //ScalerValues scaler_values; //CAEN Scaler Module 32 channels
  
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
  void SetRootOutputFileAndTree(const std::string& filename,const std::string& treename);  

public:
  TBranch * b_Event;
  RN_EventProcessor(const std::string&a="",const std::string&b="");
  
  
  virtual void Loop(Long64_t start = 0, Long64_t evnum = 0);
  int Convert2Root(std::vector<std::string>&run_number,std::string data_dir,std::string output_file); 
  int Convert2Root(const std::string& name,std::string output_file);

  int AttachFromEVT(const TString& evtfilename,const std::string & outputfile);
  int AttachFromEVT(const TString& evtfilename);

  void AddTree(TString a){fChain->Add(a);}
  virtual void InitRootFile(TString file);
  void SetRootOutputFile(const std::string & filename);
    void WriteOut();
  
  Long64_t TotEntries() const{return fChain->GetEntries();} 
  virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetEntry(entry, getall) : 0; }
  virtual int GetDetectorEntry(){return 1;};

  Int_t EventInfo(int i=0)const;

  ClassDef(RN_EventProcessor,1);
};

extern RN_EventProcessor gEventProcessor;


#ifdef __RNEVENTPROCESSOR_CXX
TFile * RN_EventProcessor::fgRootFile = 0;
TTree * RN_EventProcessor::fgDataTree = 0;
TTree * RN_EventProcessor::fgScalerTree = 0;

Int_t RN_EventProcessor::EventInfo(int i)const{ return ((i<3) ? fEvent[i] : 0);}
  

#endif





#endif
