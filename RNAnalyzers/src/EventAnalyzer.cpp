/***********************************************************/
//Class: EventAnalyzer
//
//Author:Sean Kuvin
//
//EventAnalyzer class to read out basic information from the 
//event database.
/***********************************************************/

#ifndef _EVENTANALYZER_CXX
#define _EVENTANALYZER_CXX

#include "EventAnalyzer.hpp"
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"

using namespace RNROOT;

namespace eventinfo{
  TH1I * hRunNumber;
  TH1I * hFlag; 
  TH2D * hRFvRunNumber;
  TH2D * hSiAEvRunNumber;
  TH2D * hSiABEvRunNumber;

  EventAnalyzer::EventAnalyzer():fRunMin(3000),
				 fRunMax(4023)
  {
    
  }
  
  bool EventAnalyzer::Begin(){
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      exit(EXIT_FAILURE);
    }
    fgRootFile->mkdir("EventInfo");
    fgRootFile->cd("EventInfo");
    
    hRunNumber = new TH1I("hRunNumber","hRunNumber",4096,0,4095);
    hFlag = new TH1I("hFlag","hFlag",4,0,3);
    hRFvRunNumber = new TH2D("hRFvRunNumber","hRFvRunNumber;RunNumber;RF",1024,fRunMin,fRunMax,1024,0,4095);
    hSiAEvRunNumber = new TH2D("hSiAvRunNumber","hSiAvRunNumber;RunNumber;SiAE",1024,fRunMin,fRunMax,512,0,64);
    hSiABEvRunNumber = new TH2D("hSiABvRunNumber","hSiABvRunNumber;RunNumber;SiABE",1024,fRunMin,fRunMax,512,0,64);
    
    
    return 1;
  }
  
  void EventAnalyzer::SetRunMinMax(int min, int max){
    fRunMin = min;
    fRunMax = max;
  }


  bool EventAnalyzer::Process(){
    return 1;
  }
  
  bool EventAnalyzer::ProcessFill(){
    hRunNumber->Fill(RNROOT::gMainAnalyzer.EventInfo(0));
    hFlag->Fill(RNROOT::gMainAnalyzer.EventInfo(1));
    hRFvRunNumber->Fill(RNROOT::gMainAnalyzer.EventInfo(0),rftime.fT);
    hSiAEvRunNumber->Fill(RNROOT::gMainAnalyzer.EventInfo(0),si_array.E_A());
    hSiABEvRunNumber->Fill(RNROOT::gMainAnalyzer.EventInfo(0),si_array.E_AB());
    
    
    return 1;
  }
  
  void EventAnalyzer::Reset(){
  }
  
  
  bool EventAnalyzer::Terminate(){
    return 1;
  }
  
  
  
}
#endif
