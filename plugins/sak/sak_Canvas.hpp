#ifndef sak_CANVAS_H
#define sak_CANVAS_H

#include <cstdlib>
#include <iostream>
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <TSpectrum.h>
namespace sak{

  extern TCanvas* canvas;
  

  TCanvas* NewCanvas(const TString& name,const int&width = 600,const int& height = 600, const int& divisionx = 1,const int& divisiony = 1);


  TPad* DisplaySpectrum_plus(const TString& a,const TString&opt);
  TPad* DisplaySpectrum(const TString& a,const TString&opt);


  int CloseCanvas();


  void GetAllPeaks(const std::string& module);


}

#endif
