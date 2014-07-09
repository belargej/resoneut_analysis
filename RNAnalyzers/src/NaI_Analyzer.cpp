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
  TH1D *hNaIArray_T;	      
  TH1D *hNaIArray_E;	      
  TH1D *hNaIArray_Position;
  TH1D *hNaIArray_Detector;
  TH2D *hNaIArray_TvRftime;


  TH1D *hNaI_T[20];	      
  TH1D *hNaI_T1[20];	      
  TH1D *hNaI_T2[20];	      
  TH1D *hNaI_ESum[20];	      
  TH1D *hNaI_EGamma[20];	      
  TH1D *hNaI_Position[20];
  


  
  NaI_Analyzer::NaI_Analyzer()
  {
    
  }
  
  void NaI_Analyzer::Reset(){
  }
  
  bool NaI_Analyzer::Begin(){
 
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    fgRootFile->mkdir("NaIArray");
    fgRootFile->cd("NaIArray");
    hNaIArray_Detector = new TH1D("hNaIArray_Detector","hNaIArray_Detector;T",21,0,20);
    hNaIArray_T = new TH1D("hNaIArray_T","hNaIArray_T;T",4096,0,4095);	          
    hNaIArray_E = new TH1D("hNaIArray_E","hNaIArray_E;E",4096,0,4095);	      
    hNaIArray_Position = new TH1D("hNaIArray_Position","hNaIArray_Position;T",4096,-1024,1024);	             
    hNaIArray_TvRftime=new TH2D("hNaIArray_TvRftime","hNaIArray_TvRftime",128,0,127,512,0,4095);

    for (unsigned int i=0;i<20;i++){
      fgRootFile->cd("NaIArray");
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
    if(nai_array.Mult()>0){
      hNaIArray_Detector->Fill(nai_array.Ch());    
      hNaIArray_T->Fill(nai_array.TRaw());
      hNaIArray_E->Fill(nai_array.ERaw());
      hNaIArray_Position->Fill(nai_array.fPosition[0]); 
      hNaIArray_TvRftime->Fill(rftime.TMod2(),nai_array.TRaw());
    }
    for (unsigned int i=0;i<20;i++){
	hNaI_T[i]->Fill(nai[i].T());
	hNaI_T1[i]->Fill(nai[i].T1());
	hNaI_T2[i]->Fill(nai[i].T2());
	hNaI_ESum[i]->Fill(nai[i].SumE());
	hNaI_EGamma[i]->Fill(nai[i].E_Gamma());
	hNaI_Position[i]->Fill(nai[i].Position());
    }
    return 1;
  }



  bool NaI_Analyzer::Terminate(){
    return 1;
  }


 

} 
		       



