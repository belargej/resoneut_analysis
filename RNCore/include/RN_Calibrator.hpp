/***************************************************************************
Class: RN_S2Calibrator

RN_S2Calibrator performs the basic function
of matching the charge from front channels to back channels and vica
versa.  

Encapsulated in si_cal is an implematation called AutoCalibrate:
This function calibrates the silicon detectors which are added to the
S2Collection in RN_Root.hpp.

The AutoCalibrate function requires the input of an RN_EventProcessor derived class.  This implementation of an RN_EventProcessor should have overridden 
the GetDetectorEntry() method to sort data into the appropriate detectors.
We can use RNROOT::gMainAnalyzer (which is an RN_Analyzer, an implementation of RN_EventProcessor)

To Perform the AutoCalibrate method then we run:

si_cal::AutoCalibrate(3,3,RNROOT::gMainAnalyzer);
to use ring ch3 and phi ch3 as the matching channels for the gain matching.
Multiple output files are produced with the calibration results. Two per detector for the "front_2_back" and "back_2_front" matches.  


  Author: Sean Kuvin- 2013                             
**************************************************************************/

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

#include "RN_Root.hpp"


class RN_S2Calibrator{
private:
public:
  RN_S2Calibrator(const int& corrnum,std::string dname,const double& lowlimit=0.,const double& highlimit=4096.):a0(corrnum,double(0)),
														a1(corrnum,double(1)),
														point(corrnum,int(0)),
														Corr(corrnum,TGraph()),
														detname(dname),
														elowlimit(lowlimit),
														ehighlimit(highlimit){
  }
  std::vector<Double32_t> a0,a1;
  std::vector<int> point;
  std::vector<TGraph> Corr;//for matching all front to one back channel
  std::string detname;
  int elowlimit;
  int ehighlimit;

  void AddHit(const double & e,const double& e1, const int& ch);
  void PerformFit();
  int PrintCoefficients(std::string);


};

namespace si_cal{

  void ProduceCorrelationHistograms(
				    const unsigned int& matchfront,
				    const unsigned int& matchback, 
				    const unsigned int& DetID,
				    RN_EventProcessor& EventProcessor = RNROOT::gMainAnalyzer,
				    const double& xbins=2048,
				    const double& xmin=0,
				    const double& xmax=4095,
				    const double& ybins=2048,
				    const double& ymin=0,
				    const double& ymax=4095);
  
  void AutoCalibrate(const unsigned int& matchfront, 
		     const unsigned int& matchback,
		     RN_EventProcessor& EventProcessor = RNROOT::gMainAnalyzer);
  
  Double_t GetPulserOffset(TH1D * h1,
			   const int&Peaks,
			   Float_t * inputs, 
			   double & offset,
			   const double& sigma = 2, 
			   const double& threshold = 0.05,
			   const double& xmin=0,
			   const double & xmax=4095);
    
  
  void Th228Fit(const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		double&, 
		double&);
  void Th228Fit_corr(const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		double&, 
		double&);
  void Th228Fit_proton(const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		const double&,
		double&, 
		double&);
    


  void Th228Fit(TH1D * h1,
		double &elin,
		double &eshift);
 
 
}

typedef std::vector<RN_S2Calibrator> RN_S2CalCollection;
typedef std::vector<RN_S2Calibrator>::iterator RN_S2CalCollectionRef;

#endif
