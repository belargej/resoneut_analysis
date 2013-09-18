////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////

#ifndef __RESCALIBRATOR_CXX
#define __RESCALIBRATOR_CXX


//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

//ROOT libraties
#include <TH1.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCutG.h>
#include <TF1.h>

#include "RN_Calibrator.hpp"


void RN_S2Calibrator::AddHit(const double& ematch, const double& ech,const int& ch){
  Corr[ch].SetPoint(point[ch],ech,ematch);
  point[ch]++;

}

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void RN_S2Calibrator::PerformFit(){
  //op==1 matches all front channels to one back channel
  TF1 *fitter= new TF1("fitter","pol1",elowlimit,ehighlimit);
  unsigned int num=a0.size();
  for(unsigned int l=0;l<num;l++){
    Corr[l].Fit(fitter,"ROB=0.52","",elowlimit,ehighlimit);
    a0[l]=fitter->GetParameter(0);
    a1[l]=fitter->GetParameter(1);
    std::cout<<l<<": RESULT: "<<a0[l]<<" "<<a1[l]<<std::endl;
  }
}
 
int RN_S2Calibrator::PrintCoefficients(std::string printfile){
  ofstream coeff;
  coeff.open(printfile.c_str());
  if (!coeff.is_open()){
    std::cout << "  Could not create " << printfile << std::endl;
    return 0;
  }
 
  unsigned int num=a0.size();

  for(unsigned int j=0;j<num;j++){
    coeff<<Form("%s.a0[%d]  ",detname.c_str(),j)<<a0[j]<<std::endl;
    coeff<<Form("%s.a1[%d]  ",detname.c_str(),j)<<a1[j]<<std::endl;
  }
  
  return 1;
}


void RN_S2Calibrator::Th228Fit(const double& peak1,
			       const double& peak2,
			       const double& peak3,
			       const double& peak4,
			       const double& peak5,
			       const double& peak6,
			       double& elin,
			       double& eshift){

  double xset[6]={peak1,peak2,peak3,peak4,peak5,peak6};
  double yset[6]={5.42315,5.68537,6.05,6.28808,6.7783,8.78486};
  TF1 *fitter2=new TF1("fitter2","pol1",0,4096);
  TGraph peaks(6,xset,yset);
  peaks.Fit(fitter2,"","",0,4096);
  eshift=fitter2->GetParameter(0);
  elin=fitter2->GetParameter(1);

  
  

}



#endif
