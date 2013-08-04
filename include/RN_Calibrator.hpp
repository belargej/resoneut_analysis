////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////

#ifndef _RESCALIBRATOR
#define _RESCALIBRATOR

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



class RN_S2Calibrator{
private:
public:
  RN_S2Calibrator(const int& corrnum,std::string dname,const double& lowlimit=0.,const double& highlimit=4096.):a0(corrnum,double(0)),
														a1(corrnum,double(0)),
														point(corrnum,int(0)),
														Corr(corrnum,TGraph()),
														detname(dname),
														elowlimit(lowlimit),
														ehighlimit(highlimit){
  }
  std::vector<double> a0,a1;
  std::vector<int> point;
  std::vector<TGraph> Corr;//for matching all front to one back channel
  std::string detname;
  int elowlimit;
  int ehighlimit;

  void AddHit(const double & e,const double& e1, const int& ch);
  void PerformFit();
  int PrintCoefficients(std::string);
  
  
  
  
 
 
};

typedef std::vector<RN_S2Calibrator> RN_S2CalCollection;
typedef std::vector<RN_S2Calibrator>::iterator RN_S2CalCollectionRef;

#endif