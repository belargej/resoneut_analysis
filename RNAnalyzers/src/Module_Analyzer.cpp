/***********************************************************/
//Class: Module_Analyzer
//
//Author:Sean Kuvin
//
//Module_Analyzer serves the simple purpose of creating 1D histograms
//for all of the module parameters (just as SpecTcl does)
/***********************************************************/

#include "Module_Analyzer.hpp"
#include "RN_Root.hpp"

TH1D *hTDC1[32];
TH1D *hTDC2[32];
TH1D *hTDC3[32];
TH1D *hTDC4[32];
TH1D *hADC1[32];
TH1D *hADC2[32];
TH1D *hADC3[32];
TH1D *hADC4[32];
TH1D *hADC5[32];
TH1D *hADC6[32];
TH1D *hADC7[32];
TH1D *hQDC1[32];
TH1D *hQDC2[32];
TH1D *hQDC3[32];


using namespace RNROOT;

Module_Analyzer::Module_Analyzer()
{
  
}


void Module_Analyzer::ResetGlobals(){
}

bool Module_Analyzer::Begin(){

  if(!RNROOT::gRootFile){
    std::cout<<"output file has not been created"<<std::endl;
    Clear();
    exit(EXIT_FAILURE);
  }

  //make directory structure
  RNROOT::gRootFile->mkdir("Parameters");
  RNROOT::gRootFile->cd("Parameters");
  gDirectory->mkdir("TDC1");
  gDirectory->mkdir("TDC2");
  gDirectory->mkdir("TDC3");
  gDirectory->mkdir("TDC4");
  gDirectory->mkdir("ADC1");
  gDirectory->mkdir("ADC2");  
  gDirectory->mkdir("ADC3");
  gDirectory->mkdir("ADC4");
  gDirectory->mkdir("ADC5");
  gDirectory->mkdir("ADC6");
  gDirectory->mkdir("ADC7");
  gDirectory->mkdir("QDC1");
  gDirectory->mkdir("QDC2");
  gDirectory->mkdir("QDC3");

  
  //create histograms
  for(unsigned int i=0;i<32;i++){
    RNROOT::gRootFile->cd("Parameters/TDC1");
    hTDC1[i]=new TH1D(Form("hTDC1[%d]",i),Form("TDC1[%d];TDC1[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/TDC2");   
    hTDC2[i]=new TH1D(Form("hTDC2[%d]",i),Form("TDC2[%d];TDC2[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/TDC3");    
    hTDC3[i]=new TH1D(Form("hTDC3[%d]",i),Form("TDC3[%d];TDC3[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/TDC4");   
    hTDC4[i]=new TH1D(Form("hTDC4[%d]",i),Form("TDC4[%d];TDC4[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC1");    
    hADC1[i]=new TH1D(Form("hADC1[%d]",i),Form("ADC1[%d];ADC1[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC2");    
    hADC2[i]=new TH1D(Form("hADC2[%d]",i),Form("ADC2[%d];ADC2[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC3");    
    hADC3[i]=new TH1D(Form("hADC3[%d]",i),Form("ADC3[%d];ADC3[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC4");        
    hADC4[i]=new TH1D(Form("hADC4[%d]",i),Form("ADC4[%d];ADC4[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC5");        
    hADC5[i]=new TH1D(Form("hADC5[%d]",i),Form("ADC5[%d];ADC5[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC6");        
    hADC6[i]=new TH1D(Form("hADC6[%d]",i),Form("ADC6[%d];ADC6[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/ADC7");        
    hADC7[i]=new TH1D(Form("hADC7[%d]",i),Form("ADC7[%d];ADC7[%d]",i,i),4096,0,4095);       
    RNROOT::gRootFile->cd("Parameters/QDC1");        
    hQDC1[i]=new TH1D(Form("hQDC1[%d]",i),Form("QDC1[%d];QDC1[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/QDC2");        
    hQDC2[i]=new TH1D(Form("hQDC2[%d]",i),Form("QDC2[%d];QDC2[%d]",i,i),4096,0,4095);    
    RNROOT::gRootFile->cd("Parameters/QDC3");        
    hQDC3[i]=new TH1D(Form("hQDC3[%d]",i),Form("QDC3[%d];QDC3[%d]",i,i),4096,0,4095);    

  }

  return 1;
 
}

bool Module_Analyzer::Process(){
  return 1;
}

bool Module_Analyzer::ProcessFill(){

  for(unsigned int i=0;i<32;i++){
    if(TDC1[i]>0) hTDC1[i]->Fill(TDC1[i]);
    if(TDC2[i]>0) hTDC2[i]->Fill(TDC2[i]);
    if(TDC3[i]>0) hTDC3[i]->Fill(TDC3[i]);
    if(TDC4[i]>0) hTDC4[i]->Fill(TDC4[i]);
    if(ADC1[i]>0) hADC1[i]->Fill(ADC1[i]);
    if(ADC2[i]>0) hADC2[i]->Fill(ADC2[i]);
    if(ADC3[i]>0) hADC3[i]->Fill(ADC3[i]);
    if(ADC4[i]>0) hADC4[i]->Fill(ADC4[i]);
    if(ADC5[i]>0) hADC5[i]->Fill(ADC5[i]);
    if(ADC6[i]>0) hADC6[i]->Fill(ADC6[i]);
    if(ADC7[i]>0) hADC7[i]->Fill(ADC7[i]);
    if(QDC1[i]>0) hQDC1[i]->Fill(QDC1[i]);
    if(QDC2[i]>0) hQDC2[i]->Fill(QDC2[i]);
    if(QDC3[i]>0) hQDC3[i]->Fill(QDC3[i]);
    
  }    


  return 1;

}
  


bool Module_Analyzer::Terminate(){
  return 1;
  
}


