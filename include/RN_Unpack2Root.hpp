////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013
///The RNEvt2Root class converts an EVT file to a ROOT File with the data 
///structure of a RNEvent. Adapted from NSCL evt2root to unpack from the new
///RING MASTER buffer structure.                             
////////////////////////////////////////////////////////////////////////



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


////////////////////////////////////////////////////////////////////////////////
///This Class converts an EVT file to a ROOT File with the data structure of a
///RNEvent.  These new root files are then used for data analysis.
///////////////////////////////////////////////////////////////////////////////



class RN_module:public TObject{
private:
  Int_t fGeoAddress;
  Int_t fNumOfCh;
  Int_t fType;
  std::string fName;

public:
  RN_module(){
    fCh=new Float_t[32];//default value
  }
  RN_module(std::string name,int geo, int num,int type):fGeoAddress(geo),
							fNumOfCh(num),
							fType(type),
							fName(name){
    fCh=new Float_t[fNumOfCh];
  }

  Float_t* fCh; //[fNumOfCh]

  std::string Name()const{return fName;}
  Int_t GeoAddress()const {return fGeoAddress;}
  Int_t NumOfCh()const {return fNumOfCh;}
  Int_t Type()const {return fType;}
  void Reset(){
    for(int i=0;i<fNumOfCh;i++)
      fCh[i]=0;
  }
	
  ClassDef(RN_module,1);
};



class RNUnpack2Root{
private:
  int mesy_num;
  Int_t caen_num;
  int adc_counter;
  int mes_counter;
 
public:

  std::vector<RN_module> caen_stack;
  std::vector<RN_module> mesy_stack;

  RNUnpack2Root():mesy_num(0),caen_num(0){}

  int Convert(std::vector<int>&run_number,std::string data_dir,std::string output_file, std::string splitoption);
  bool init(const std::string& config);
  int GetMesyNum(){return mesy_num;}
  int GetCaenNum(){return caen_num;}
  void Reset();


  
};


#endif
