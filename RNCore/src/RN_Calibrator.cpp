/***************************************************************
Class: RN_S2Calibrator
Author: Sean A. Kuvin -2013
**************************************************************/
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
#include <TSpectrum.h>

#include "RN_Calibrator.hpp"
#include "RN_Root.hpp"
#include "sak_ReadBuffer.hpp"
using namespace RNROOT;

RN_S2Calibrator::RN_S2Calibrator(const int& corrnum,
				 std::string dname,
				 const double& lowlimit,
				 const double& highlimit):fA0(corrnum,double(0)),
  fA1(corrnum,double(1)),
  fPoint(corrnum,int(0)),
  fCorr(corrnum,TGraph()),
  fDetName(dname),
  fELowLimit(lowlimit),
  fEHighLimit(highlimit)
{
}

void RN_S2Calibrator::AddHit(const double& ematch, const double& ech,const int& ch){
  fCorr[ch].SetPoint(fPoint[ch],ech,ematch);
  fPoint[ch]++;

}

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void RN_S2Calibrator::PerformFit(){
  //op==1 matches all front channels to one back channel
  TF1 *fitter= new TF1("fitter","pol1",fELowLimit,fEHighLimit);
  unsigned int num=fA0.size();
  bool option;
  for(unsigned int l=0;l<num;l++){
    if(fCorr[l].GetN()<30){
      std::cout<<"Not enough points for fitting this detector/channel (<30)\n";      
      std::cout<<"Has:" <<fCorr[l].GetN()<<" 0-skip 1-proceed \n";      
      std::cin>>option;
      if(!option){
	continue;
      }
    }
    fCorr[l].Fit(fitter,"ROB=0.52","",fELowLimit,fEHighLimit);
    fA0[l]=fitter->GetParameter(0);
    fA1[l]=fitter->GetParameter(1);
    std::cout<<l<<": RESULT: "<<fA0[l]<<" "<<fA1[l]<<std::endl;
  }
}
 
int RN_S2Calibrator::PrintCoefficients(std::string printfile){
  ofstream coeff;
  coeff.open(printfile.c_str());
  if (!coeff.is_open()){
    std::cout << "  Could not create " << printfile << std::endl;
    return 0;
  }
 
  unsigned int num=fA0.size();

  for(unsigned int j=0;j<num;j++){
    coeff<<Form("%s.a0[%d]  ",fDetName.c_str(),j)<<fA0[j]<<std::endl;
    coeff<<Form("%s.a1[%d]  ",fDetName.c_str(),j)<<fA1[j]<<std::endl;
  }
  
  return 1;
}

namespace si_cal{
  TH2D * si_fch_v_bch[16];
  TH2D * si_bch_v_fch[16];
  


  void ProduceCorrelationHistograms(const unsigned int& matchfront, 
				    const unsigned int& matchback, 
				    const unsigned int & DetID,
				    RN_EventProcessor& EventProcessor,
				    const double& xbins,
				    const double& xmin,
				    const double& xmax,
				    const double& ybins,
				    const double& ymin, 
				    const double& ymax){
    if (!gRootFile){
      std::cout<<"gRootFile path not set, use RNROOT::SetRootOutputFile<>\n";
      return;
    }
    gRootFile->mkdir(Form("S%d/FrontMatch",DetID+1));   
    gRootFile->mkdir(Form("S%d/BackMatch",DetID+1));   
    
    for(unsigned int i=0;i<16;i++){
      gRootFile->cd(Form("S%d/FrontMatch",DetID+1));   
      si_fch_v_bch[i]=new TH2D(Form("s[%d]_fch[%d]_v_bch[%d]",DetID+1,matchfront,i),Form("s[%d]_fch[%d]_v_bch[%d];",DetID+1,matchfront,i),xbins,xmin,xmax,ybins,ymin,ymax);
      gRootFile->cd(Form("S%d/BackMatch",DetID+1));        
      si_bch_v_fch[i]=new TH2D(Form("s[%d]_bch[%d]_v_fch[%d]",DetID+1,matchback,i),Form("s[%d]_bch[%d]_v_fch[%d];",DetID+1,matchback,i),xbins,xmin,xmax,ybins,ymin,ymax);
    }
    Long64_t totentries= EventProcessor.TotEntries();
    std::cout<<"Total Entries: "<<totentries<<std::endl;
    
    for (Long64_t i=0;i<totentries;i++){
      EventProcessor.GetEntry(i);
      EventProcessor.GetDetectorEntry();
      if(si_[DetID].front.Mult() < 1 || si_[DetID].back.Mult() < 1)
	continue;
      if(si_[DetID].front.ChRaw(0)==matchfront){
	si_fch_v_bch[(int)si_[DetID].back.ChRaw(0)]->Fill(si_[DetID].Back_E(),si_[DetID].Front_E());
      }
      if(si_[DetID].back.ChRaw(0)==matchback){
	si_bch_v_fch[(int)si_[DetID].front.ChRaw(0)]->Fill(si_[DetID].Front_E(),si_[DetID].Back_E());		  
      }

    }

    int response(2);
    while(response!=1 || response !=0){
      std::cout<<"1: Write out gRootFile and close, 0: Keep gRootFile open and write manually\n";	
      std::cin>>response;
      if(response==1){
	gRootFile->Write();
	gRootFile->Close();
	delete gRootFile;
	break;
      }
      else if(response==0)
	break;
      else 
	continue;
    }
    
    



  }

  //AutoCalibrate function is used to match
  // the highest energy ring hits with the highest
  // energy segment hits and matching the charge.
  
  void AutoCalibrate(const unsigned int& matchfront,const unsigned int& matchback,RN_EventProcessor& EventProcessor){
    std::cout<<"calibrating back to front match channel : "<<matchfront<<std::endl;
    RN_S2CalCollection s2front;
    RN_S2CalCollection s2back;
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      s2front.push_back(RN_S2Calibrator(16,Form("%s.front",(*it).Name().c_str()),60,3000));
      s2back.push_back(RN_S2Calibrator(16,Form("%s.back",(*it).Name().c_str()),60,3000));
      
    }
    unsigned int idx=0;
    Long64_t totentries= EventProcessor.TotEntries();
    
    for (Long64_t i=0;i<totentries;i++){
      EventProcessor.GetEntry(i);
      EventProcessor.GetDetectorEntry();    
      idx=0;
      for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
	
	if(idx<si_.size()){
	  if(si_[idx].back.Mult()==1&&si_[idx].back.ChRaw(0)==matchback){
	    if(si_[idx].front.Mult()==1)
	      (*it).AddHit(si_[idx].Back_E(0),si_[idx].Front_E(0),(int)si_[idx].front.ChRaw(0));
	  }
	}
	idx++;
	
      } 
    }
    idx=0;
    for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
      (*it).PerformFit();
      (*it).PrintCoefficients(Form("%s_front2back.in",si_[idx].Name().c_str()));
      
      gVariableMap.LoadParams(Form("%s_front2back.in",si_[idx].Name().c_str()));
      si_[idx].SetCalibrations(gVariableMap);      
      
      idx++;
      
    }
    
    
    
    

    //match all back to one lead front channel
    std::cout<<"repeating now for back 2 front channel : "<<matchfront<<std::endl;
    
    for (Long64_t i=0;i<totentries;i++){
      EventProcessor.GetEntry(i);
      EventProcessor.GetDetectorEntry();
      idx=0;
      for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
	
	if(idx<si_.size()){
	  if(si_[idx].front.Mult()==1&&si_[idx].front.ChRaw(0)==matchfront){
	    if(si_[idx].back.Mult()==1)
	      (*it).AddHit(si_[idx].Front_E(),si_[idx].Back_E(),(int)si_[idx].back.ChRaw(0));
	  }
	}
	idx++;
	
      } 
    }
    idx=0;
    for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
      (*it).PerformFit();
      (*it).PrintCoefficients(Form("%s_back2front.in",si_[idx].Name().c_str()));
      //now load file and apply it to that detector
      
      gVariableMap.LoadParams(Form("%s_back2front.in",si_[idx].Name().c_str()));
      si_[idx].SetCalibrations(gVariableMap);
      idx++;
      
    }
    
    
    
    
  }

  Double_t GetPulserOffset(TH1D *h1,         //histogram to be searched
			   const int& Peaks, //number of pulser points
			   Float_t * inputs,  //V inputs from pulser
			   double & offset,			   
			   const double& sigma,
			   const double& threshold,
			   const double& xmin,
			   const double& xmax
			   ){
    TSpectrum *s = new TSpectrum();
    Int_t PeaksFound =  s->Search(h1,sigma," ",threshold);
    if(Peaks != PeaksFound){
      std::cout<<"Peaks found: "<<PeaksFound<<" different from peaks expected: "<<Peaks<<std::endl;
      return 0xffff;
    }
    Float_t *xpeaks = s->GetPositionX();
    TGraph PulserData(PeaksFound,xpeaks,inputs);
    TF1 *fitter=new TF1("fitter","pol1",xmin,xmax);
    PulserData.Fit(fitter,"","",xmin,xmax);
    offset = -1 * (fitter->GetParameter(0) / fitter->GetParameter(1));

    return offset;
  }
  
  
  void Th228Fit(const double& peak1,
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

  
  void Th228Fit_corr(const double& peak1,
		const double& peak2,
		const double& peak3,
		const double& peak4,
		const double& peak5,
		const double& peak6,
		double& elin,
		double& eshift){
    
    double xset[6]={peak1,peak2,peak3,peak4,peak5,peak6};
    double yset[6]={5.27815,5.54170,5.91958,6.1523,6.64851,8.67163};
    TF1 *fitter2=new TF1("fitter2","pol1",0,4096);
    TGraph peaks(6,xset,yset);
    peaks.Fit(fitter2,"","",0,4096);
    eshift=fitter2->GetParameter(0);
    elin=fitter2->GetParameter(1);
    
    
    

  }

  void Th228Fit_proton(const double& peak1,
		       const double& peak2,
		       const double& peak3,
		       const double& peak4,
		       const double& peak5,
		       const double& peak6,
		       double& elin,
		       double& eshift){
    
    double xset[6]={peak1,peak2,peak3,peak4,peak5,peak6};
    double yset[6]={5.29775,5.56047,5.93717,6.16916,6.66382,8.67163};
    TF1 *fitter2=new TF1("fitter2","pol1",0,4096);
    TGraph peaks(6,xset,yset);
    peaks.Fit(fitter2,"","",0,4096);
    eshift=fitter2->GetParameter(0);
    elin=fitter2->GetParameter(1);
    
    
    

  }


  void FitPulserPeaks(const std::string& detname,const std::string& module, const std::string &pulserinputs){
    std::vector<Double32_t> pulservalues;
    
    std::ifstream cal;
    std::string key;double value;
    cal.open(pulserinputs.c_str());
    if (!cal.is_open()){
    std::cout << "  Could not open " << pulserinputs << std::endl;
    return;
    }
    do{
      std::vector<std::string>input;
      sak::ReadLine(cal,input,1);
      if (input.size()!=1){
	std::cout<<"pulser input file has line with diff than 1 entries"<<std::endl;
	continue;
      }
      value=sak::string_to_double(input[0]);
      
      pulservalues.push_back(value);
      
    }while(!cal.eof());
    
    TSpectrum *s = new TSpectrum();
    for(unsigned int i=0;i<16;i++){
      if(!gDirectory->Get(Form("h%s[%d]",module.c_str(),i))){
	std::cout<<"histograms not found: "<<Form("h%s[%d]",module.c_str(),i)<<"\n";
	break;
      }
      TF1 *fitter = new TF1(Form("fitter%d",i),"pol1",0,4096);
      TGraph *graph = new TGraph(Form("graph%d",i));
      TH1D* hist = (TH1D*)gDirectory->Get(Form("h%s[%d]",module.c_str(),i));
      Int_t PeaksFound =  s->Search(hist);
      if(pulservalues.size() < PeaksFound){
	std::cout<<"more peaks found than pulser inputs"<<std::endl;
	return ;
      }
      std::cout<<module<<"."<<i<<": \n";
      Float_t *xpeaks = s->GetPositionX();
      std::vector<Float_t> sortedpeaks;
      for(unsigned int j=0;j<PeaksFound;j++){
	sortedpeaks.push_back(xpeaks[j]);
      }
      std::sort(sortedpeaks.begin(),sortedpeaks.end());
      for(unsigned int j=0;j<PeaksFound;j++){
	graph->SetPoint(j,sortedpeaks[j],pulservalues[j]);
	std::cout<<"peak "<<j<<" "<<sortedpeaks[j]<<"\n";      
      }
      graph->Fit(fitter);
      std::cout<<detname+ ".a0["<<i<<"]"<<" "<<fitter->GetParameter(0);
      std::cout<<detname+ ".a1["<<i<<"]"<<" "<<fitter->GetParameter(1);
    }
    
    
  }
}








#endif
