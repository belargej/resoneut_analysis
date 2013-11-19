/***************************************************************/
//Class: NaI_Analyzer
//
//Author:Sean Kuvin
//
//NaI_Analyzer is added to the analyzer list to sort NaI parameters
/********************************************************************/


#include "NaI_Analyzer.hpp"
#include "PSD_Analyzer.hpp"



namespace gamma_nai{
  
  Double32_t nai_t(0);
  TH1D *hNaI_T;	      
  TH1D *hNaI_E;	      
  TH1D *hNaI_Position;
  TH1D *hNaI_Detector;
  
  NaI_Analyzer::NaI_Analyzer()
  {
    
  }
  
  void NaI_Analyzer::ResetGlobals(){
    nai_t=0;
  }
  
  bool NaI_Analyzer::Begin(){
 
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    hNaI_T = new TH1D("hNaI_T","hNaI_T;T",4096,0,4095);	          
    hNaI_E = new TH1D("hNaI_E","hNaI_E;E",4096,0,4095);	      
    hNaI_Position = new TH1D("hNaI_Position","hNaI_Position;T",4096,-1024,1024);	          
    hNaI_Detector = new TH1D("hNaI_Detector","hNaI_Detector;T",21,0,20);
    
    return 1;
  }
  
  bool NaI_Analyzer::Process(){ 
    nai_array.ReconstructHits(nai);
    
    return 1;
  }
  bool NaI_Analyzer::ProcessFill(){
    hNaI_T->Fill(nai_array.fT[0]);
    hNaI_E->Fill(nai_array.fE[0]);
    hNaI_Position->Fill(nai_array.fPosition[0]); 
    hNaI_Detector->Fill(nai_array.fChlist[0]);
    return 1;
  }



  bool NaI_Analyzer::Terminate(){
    return 1;
  }
  bool NaI_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    return 1;
  }

 

} 
		       



