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
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCutG.h>
#include <TSelector.h>
#include <map>

#include "sak_ReadBuffer.hpp"

class RNUnpack2Root{
private:
  int adc_counter;
  int mes_counter;
  Int_t Event[2];//stores RunNum/flag
 
public:
  float ADC1[32];
  float ADC2[32];
  float ADC3[32];
  float ADC4[32];
  float ADC5[32];
  float ADC6[32];
  float TDC1[32];
  float TDC2[32];
  float TDC3[32];
  float QDC1[32];
  float QDC2[32];

  std::vector<short> caen_stack;
  std::vector<short> mesy_stack;
  TRandom3 Rnd;

  RNUnpack2Root();

  int Convert(std::vector<int>&run_number,std::string data_dir,std::string output_file);
  bool init();
  int GetMesyNum(){return mesy_stack.size();}
  int GetCaenNum(){return caen_stack.size();}
  int SortGeoChan(short geoaddress,short chan,short val);
  void Reset();


 
};


#endif
