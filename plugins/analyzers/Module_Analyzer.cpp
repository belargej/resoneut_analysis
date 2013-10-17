#include "Module_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "../../include/RN_Unpack2Root.hpp"

sak::Hist1D *hTDC1[32];
sak::Hist1D *hTDC2[32];
sak::Hist1D *hTDC3[32];
sak::Hist1D *hTDC4[32];
sak::Hist1D *hADC1[32];
sak::Hist1D *hADC2[32];
sak::Hist1D *hADC3[32];
sak::Hist1D *hADC4[32];
sak::Hist1D *hADC5[32];
sak::Hist1D *hADC6[32];
sak::Hist1D *hADC7[32];
sak::Hist1D *hQDC1[32];
sak::Hist1D *hQDC2[32];
sak::Hist1D *hQDC3[32];

using namespace unpacker;


Module_Analyzer::Module_Analyzer()
{
  
}


void Module_Analyzer::ResetGlobals(){
}

bool Module_Analyzer::Begin(){

  if(!rootfile){
    std::cout<<"output file has not been created"<<std::endl;
    Clear();
    exit(EXIT_FAILURE);
  }
  rootfile->mkdir("Parameters");
  rootfile->mkdir("Parameters/TDC1");
  rootfile->mkdir("Parameters/TDC2");
  rootfile->mkdir("Parameters/TDC3");
  rootfile->mkdir("Parameters/TDC4");
  rootfile->mkdir("Parameters/ADC1");
  rootfile->mkdir("Parameters/ADC2");  
  rootfile->mkdir("Parameters/ADC3");
  rootfile->mkdir("Parameters/ADC4");
  rootfile->mkdir("Parameters/ADC5");
  rootfile->mkdir("Parameters/ADC6");
  rootfile->mkdir("Parameters/ADC7");
  rootfile->mkdir("Parameters/QDC1");
  rootfile->mkdir("Parameters/QDC2");
  rootfile->mkdir("Parameters/QDC3");

  
  for(unsigned int i=0;i<32;i++){
    rootfile->cd("Parameters/TDC1");
    hTDC1[i]=new sak::Hist1D(Form("hTDC1[%d]",i),Form("TDC1[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/TDC2");   
    hTDC2[i]=new sak::Hist1D(Form("hTDC2[%d]",i),Form("TDC2[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/TDC3");    
    hTDC3[i]=new sak::Hist1D(Form("hTDC3[%d]",i),Form("TDC3[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/TDC4");   
    hTDC4[i]=new sak::Hist1D(Form("hTDC4[%d]",i),Form("TDC4[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC1");    
    hADC1[i]=new sak::Hist1D(Form("hADC1[%d]",i),Form("ADC1[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC2");    
    hADC2[i]=new sak::Hist1D(Form("hADC2[%d]",i),Form("ADC2[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC3");    
    hADC3[i]=new sak::Hist1D(Form("hADC3[%d]",i),Form("ADC3[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC4");        
    hADC4[i]=new sak::Hist1D(Form("hADC4[%d]",i),Form("ADC4[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC5");        
    hADC5[i]=new sak::Hist1D(Form("hADC5[%d]",i),Form("ADC5[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC6");        
    hADC6[i]=new sak::Hist1D(Form("hADC6[%d]",i),Form("ADC6[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/ADC7");        
    hADC7[i]=new sak::Hist1D(Form("hADC7[%d]",i),Form("ADC7[%d]",i),4096,0,4095);       
    rootfile->cd("Parameters/QDC1");        
    hQDC1[i]=new sak::Hist1D(Form("hQDC1[%d]",i),Form("QDC1[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/QDC2");        
    hQDC2[i]=new sak::Hist1D(Form("hQDC2[%d]",i),Form("QDC2[%d]",i),4096,0,4095);    
    rootfile->cd("Parameters/QDC3");        
    hQDC3[i]=new sak::Hist1D(Form("hQDC3[%d]",i),Form("QDC3[%d]",i),4096,0,4095);    

  }

  return 1;
 
}


bool Module_Analyzer::Process(){

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


