/***************************************************************/
//Class: NaI_Analyzer
//
//Author:Sean Kuvin
//
//NaI_Analyzer is added to the analyzer list to sort NaI parameters
/********************************************************************/


#include "NaI_Analyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace gamma_nai{
  
  Double32_t nai_t(0);
  TH1D *hNaIArray_T;	      
  TH1D *hNaIArray_E;	      
  TH1D *hNaIArray_Position;
  TH1D *hNaIArray_Detector;

  TH1D *hNaI_T[20];	      
  TH1D *hNaI_T1[20];	      
  TH1D *hNaI_T2[20];	      
  TH1D *hNaI_ESum[20];	      
  TH1D *hNaI_EGamma[20];	      
  TH1D *hNaI_Position[20];
  


  
  NaI_Analyzer::NaI_Analyzer()
  {
    
  }
  
  void NaI_Analyzer::ResetGlobals(){
    nai_t=0;
  }
  
  bool NaI_Analyzer::Begin(){
 
    if(!RNROOT::gRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    RNROOT::gRootFile->mkdir("NaIArray");
    RNROOT::gRootFile->cd("NaIArray");
    hNaIArray_Detector = new TH1D("hNaIArray_Detector","hNaIArray_Detector;T",21,0,20);
    hNaIArray_T = new TH1D("hNaIArray_T","hNaIArray_T;T",4096,0,4095);	          
    hNaIArray_E = new TH1D("hNaIArray_E","hNaIArray_E;E",4096,0,4095);	      
    hNaIArray_Position = new TH1D("hNaIArray_Position","hNaIArray_Position;T",4096,-1024,1024);	             
 
    for (unsigned int i=0;i<20;i++){
      RNROOT::gRootFile->cd("NaIArray");
      gDirectory->mkdir(Form("NaI[%d]",i));
      gDirectory->cd(Form("NaI[%d]",i));
      hNaI_T[i] = new TH1D(Form("hNaI[%d]_T",i),Form("hNaI[%d]_T;T",i),4096,0,4095);	     
      hNaI_T1[i] = new TH1D(Form("hNaI[%d]_T1",i),Form("hNaI[%d]_T1;T1",i),4096,0,4095);	  
      hNaI_T2[i] = new TH1D(Form("hNaI[%d]_T2",i),Form("hNaI[%d]_T2;T2",i),4096,0,4095);	         
      hNaI_ESum[i] = new TH1D(Form("hNaI[%d]_ESum",i),Form("hNaI[%d]_ESum;E",i),4096,0,4095);	      
      hNaI_EGamma[i] = new TH1D(Form("hNaI[%d]_EGamma",i),Form("hNaI[%d]_EGamma;E",i),4096,0,4095);	      
      hNaI_Position[i] = new TH1D(Form("hNaI[%d]_Position",i),Form("hNaI[%d]_Position;T",i),4096,-1024,1024);	             
    }

    
    return 1;
  }
  
  bool NaI_Analyzer::Process(){     
    return 1;
  }
  bool NaI_Analyzer::ProcessFill(){
    if(nai_array.fMult>0){
      hNaIArray_Detector->Fill(nai_array.fChlist[0]);    
      hNaIArray_T->Fill(nai_array.fT[0]);
      hNaIArray_E->Fill(nai_array.fE[0]);
      hNaIArray_Position->Fill(nai_array.fPosition[0]); 
    }
    for (unsigned int i=0;i<20;i++){
      if(nai[i].SumE()>0){
	
	hNaI_T[i]->Fill(nai[i].T());
	hNaI_T1[i]->Fill(nai[i].T1());
	hNaI_T2[i]->Fill(nai[i].T2());
	hNaI_ESum[i]->Fill(nai[i].SumE());
	hNaI_EGamma[i]->Fill(nai[i].E_Gamma());
	hNaI_Position[i]->Fill(nai[i].Position()); 
      }
    }
    return 1;
  }



  bool NaI_Analyzer::Terminate(){
    return 1;
  }
  bool NaI_Analyzer::TerminateIfLast(){
    RNROOT::gRootFile->Write();
    RNROOT::gRootFile->Close();
    return 1;
  }

 

} 
		       



