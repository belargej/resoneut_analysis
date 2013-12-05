/*************************************************************
sak_Canvas will serve to emulate SpecTcl Xamine interface for 
drawing histograms on multiple pads.
Eventually these functions will be called by handlers for the gui 
interface

Author: Sean Kuvin

*************************************************************/

#include "sak_Canvas.hpp"

namespace sak{
  TCanvas *canvas;
  Int_t cindex(0);
  Int_t cmax(1);


  TCanvas* NewCanvas(const TString& name,const int& width, const int& height, const int& divisionx,const int&divisiony){
    if(canvas)delete canvas;
    canvas=new TCanvas(name,name,width,height);
    canvas->Divide(divisionx,divisiony);
    cindex=1;
    cmax=divisionx*divisiony;
    canvas->cd(cindex);
    return canvas;
} 


  TPad* DisplaySpectrum_plus(const TString& a,const TString & opt){
    TH1* b = (TH1*)gDirectory->Get(a);
    b->Draw(opt);

    if(++cindex>cmax)
      cindex=1; //loop back to first pad;

    canvas->cd(cindex);

    return 0;
  } 
  
  TPad* DisplaySpectrum(const TString& a, const TString & opt){
    TH1* b = (TH1*)gDirectory->Get(a);
    b->Draw(opt);
    return 0;
  } 
  

  int CloseCanvas(){
    if(canvas)
      canvas->Close();
    
    
  }



  void GetAllPeaks(const std::string& module){
    TSpectrum *s = new TSpectrum();
    for(unsigned int i=0;i<32;i++){
      if(!gDirectory->Get(Form("h%s[%d]",module.c_str(),i))){
	std::cout<<"histograms not found: "<<Form("h%s[%d]",module.c_str(),i)<<"\n";
	break;
      }
      TH1D* hist = (TH1D*)gDirectory->Get(Form("h%s[%d]",module.c_str(),i));
      Int_t PeaksFound =  s->Search(hist);
      std::cout<<module<<"."<<i<<": \n";
      Float_t *xpeaks = s->GetPositionX();
      for(unsigned int j=0;j<PeaksFound;j++){
      std::cout<<"peak."<<j<<": :"<<xpeaks[j]<<"\n";
      }
      
    }
    
    
  }
}
