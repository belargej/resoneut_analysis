/***************************************************************/
//Class: Neut_TOF_Analyzer
//
//Author:Sean Kuvin
//
//Neut_TOF_Analyzer is added to the analyzer list to sort neutron detector
//parameters.  Make histograms, load cuts, and check cuts 
//using parameters related to the neutron detector pulse shape
// discrimination spectra.  Also, neutron timing.
/********************************************************************/
#ifndef _NEUT_TOF_ANALYZER_CXX
#define _NEUT_TOF_ANALYZER_CXX

#include "Neut_TOF_Analyzer.hpp"
#include "PSD_Analyzer.hpp"

namespace neut_tof{


  TH1D *hrftime_n[NEUTNUM];
  TH2D *hQvrftime_n[NEUTNUM];
  TH1D *hrftime_nPSD;

  Neut_TOF_Analyzer::Neut_TOF_Analyzer():RN_Analyzer("Neut_TOF_Analyzer","Neut_TOF_Analyzer")
  {
  }
  


  bool Neut_TOF_Analyzer::Begin(){
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    //create directory structure

    if(!rootfile->cd("neut")){
      rootfile->mkdir("neut");
      rootfile->cd("neut");
    }
    gDirectory->mkdir("TOF");
    gDirectory->cd("TOF");
    gDirectory->mkdir("RF");
    hrftime_nPSD=new TH1D("hrftime_nPSD","hrftime_nPSD",128,0,127);
    for(unsigned int i=0;i<NEUTNUM;i++){
      hrftime_n[i]=new TH1D(Form("hrftime_n[%d]",i),Form("hrftime_n[%d]",i),128,0,127);
      hQvrftime_n[i]=new TH2D(Form("hQvrftime_n[%d]",i),Form("hQvrftime_n[%d]",i),128,0,127,512,0,4095);

    }

    return 1;
    
  }
  void Neut_TOF_Analyzer::ResetGlobals(){

  }
  

  bool Neut_TOF_Analyzer::Process(){ 
    
    return 1; 
  }

  bool Neut_TOF_Analyzer::ProcessFill(){
        if(psd::rawneut_orcheck)
      hrftime_nPSD->Fill(rftime[0].T_Wrapped());
    for(unsigned int i=0;i<neut.size();i++){
      if(psd::rawneutcheck[i]){
	hrftime_n[i]->Fill(rftime[0].T_Wrapped());
	hQvrftime_n[i]->Fill(rftime[0].T_Wrapped(),neut[i].fQ_long);
      }

    }
 
    
    return 1;
  }

  
  bool Neut_TOF_Analyzer::Terminate(){
    return 1;
  }
  
  bool Neut_TOF_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    return 1;
  }

}

  
	

#endif	       



