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
}
