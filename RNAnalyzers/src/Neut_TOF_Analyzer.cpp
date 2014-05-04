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
#include "RN_Root.hpp"

using namespace RNROOT;

namespace neut_tof{


  TH1D *hrftime_n[NEUTNUM];
  TH1D *hrftime_PSDn[NEUTNUM];
  TH1D *hT_n[NEUTNUM];
  TH1D *hT_PSDn[NEUTNUM];
  TH1D *hTRelRF_PSDn[NEUTNUM];
  TH2D *hQvrftime_n[NEUTNUM];
 
  TH2D *hQvT_n[NEUTNUM];
  TH1D *hrftime_nPSD;

  Neut_TOF_Analyzer::Neut_TOF_Analyzer():RN_Analyzer("Neut_TOF_Analyzer","Neut_TOF_Analyzer")
  {
  }
  


  bool Neut_TOF_Analyzer::Begin(){
    if(!RNROOT::gRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    //create directory structure

    if(!RNROOT::gRootFile->cd("neut")){
      RNROOT::gRootFile->mkdir("neut");
      RNROOT::gRootFile->cd("neut");
    }
    gDirectory->mkdir("TOF");
    gDirectory->cd("TOF");
    gDirectory->mkdir("RF");
    hrftime_nPSD=new TH1D("hrftime_nPSD","hrftime_nPSD",128,0,127);
    for(unsigned int i=0;i<NEUTNUM;i++){
      hT_n[i]=new TH1D(Form("hT_n[%d]",i),Form("hT_n[%d]",i),2048,0,1024);
      hT_PSDn[i]=new TH1D(Form("hT_PSDn[%d]",i),Form("hT_PSDn[%d]",i),2048,0,1024);
      hTRelRF_PSDn[i]=new TH1D(Form("hTRelRF_PSDn[%d]",i),Form("hTRelRF_PSDn[%d]",i),2048,0,1024);
      hrftime_n[i]=new TH1D(Form("hrftime_n[%d]",i),Form("hrftime_n[%d]",i),128,0,127);
      hrftime_PSDn[i]=new TH1D(Form("hrftime_PSDn[%d]",i),Form("hrftime_PSDn[%d]",i),128,0,127);
      hQvrftime_n[i]=new TH2D(Form("hQvrftime_n[%d]",i),Form("hQvrftime_n[%d]",i),128,0,127,512,0,4095);
      hQvT_n[i]=new TH2D(Form("hQvT_n[%d]",i),Form("hQvT_n[%d]",i),512,0,1024,512,0,4095);

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
      hrftime_nPSD->Fill(rftime.T_Wrapped());
    for(unsigned int i=0;i<neut.size();i++){
      
      if(neut[i].T()>0){
	hT_n[i]->Fill(neut[i].T());
	hrftime_n[i]->Fill(rftime.T_Wrapped());
      }
      if(psd::rawneutcheck[i]){
	hT_PSDn[i]->Fill(neut[i].T());
	hTRelRF_PSDn[i]->Fill(neut[i].T()-rftime.T_Wrapped());
	hrftime_PSDn[i]->Fill(rftime.T_Wrapped());
	hQvrftime_n[i]->Fill(rftime.T_Wrapped(),neut[i].fQ_long);
	hQvT_n[i]->Fill(neut[i].T(),neut[i].fQ_long);
      }

    }
 
    
    return 1;
  }

  
  bool Neut_TOF_Analyzer::Terminate(){
    return 1;
  }
  
  bool Neut_TOF_Analyzer::TerminateIfLast(){
    RNROOT::gRootFile->Write();
    RNROOT::gRootFile->Close();
    return 1;
  }

}

  
	

#endif	       



