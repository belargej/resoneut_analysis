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
#include <TSpectrum.h>

#include "RN_Calibrator.hpp"
#include "RN_Root.hpp"


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
    if(Corr[l].GetN()<50){
      std::cout<<"Not enough points for fitting this detector/channel"<<std::endl;
      continue;
    }
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

namespace si_cal{
  TH2D * si_fch_v_bch[16];
  TH2D * si_bch_v_fch[16];
  


  void ProduceCorrelationHistograms(const unsigned int& matchfront, const unsigned int& matchback, const unsigned int & DetID){
    if (!rootfile){
      std::cout<<"rootfile path not set, use SetRootOutputFile<>\n";
      return;
    }
    rootfile->mkdir(Form("S%d/FrontMatch",DetID+1));   
    rootfile->mkdir(Form("S%d/BackMatch",DetID+1));   
    
    for(unsigned int i=0;i<16;i++){
      rootfile->cd(Form("S%d/FrontMatch",DetID+1));   
      si_fch_v_bch[i]=new TH2D(Form("s[%d]_fch[%d]_v_bch[%d]",DetID+1,matchfront,i),Form("s[%d]_fch[%d]_v_bch[%d];",DetID+1,matchfront,i),2048,0,4095,2048,0,4095);
      rootfile->cd(Form("S%d/BackMatch",DetID+1));        
      si_bch_v_fch[i]=new TH2D(Form("s[%d]_bch[%d]_v_fch[%d]",DetID+1,matchback,i),Form("s[%d]_bch[%d]_v_fch[%d];",DetID+1,matchback,i),2048,0,4095,2048,0,4095);
    }
    Long64_t totentries= MainAnalyzer.TotEntries();
    std::cout<<"Total Entries: "<<totentries<<std::endl;
    
    for (Long64_t i=0;i<totentries;i++){
      MainAnalyzer.GetDetectorEntry(i);
      if(si_[DetID].front.fChlist[0]==matchfront){
	si_fch_v_bch[(int)si_[DetID].back.fChlist[0]]->Fill(si_[DetID].Back_E(),si_[DetID].Front_E());
      }
      if(si_[DetID].back.fChlist[0]==matchback){
	si_bch_v_fch[(int)si_[DetID].front.fChlist[0]]->Fill(si_[DetID].Front_E(),si_[DetID].Front_E());		  
      }

    }




  }

  //AutoCalibrate function is used to match
  // the highest energy ring hits with the highest
  // energy segment hits and matching the charge.
  
  void AutoCalibrate(const unsigned int& matchfront,const unsigned int& matchback){
    std::cout<<"calibrating back to front match channel : "<<matchfront<<std::endl;
    RN_S2CalCollection s2front;
    RN_S2CalCollection s2back;
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      s2front.push_back(RN_S2Calibrator(16,Form("%s.front",(*it).Name().c_str()),60,3000));
      s2back.push_back(RN_S2Calibrator(16,Form("%s.back",(*it).Name().c_str()),60,3000));
      
    }
    int idx=0;
    
    Long64_t totentries= MainAnalyzer.TotEntries();
    for (Long64_t i=0;i<totentries;i++){
      MainAnalyzer.GetDetectorEntry(i);
      
      idx=0;
      for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
	
	if(idx<si_.size()){
	  if(si_[idx].front.fMult>0&&si_[idx].front.fChlist[0]==matchfront){
	    if(si_[idx].back.fMult>0)
	      (*it).AddHit(si_[idx].front.fE[0],si_[idx].back.fE[0],(int)si_[idx].back.fChlist[0]);
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
      
      std::cout<<"sleeping for 2 seconds"<<std::endl;
      usleep(2000000);
      
      
      DetVar.LoadParams(Form("%s_back2front.in",si_[idx].Name().c_str()));
      si_[idx].SetCalibrations(DetVar);
      idx++;
      
    }
    
    
    std::cout<<"repeating now for front 2 back channel : "<<matchback<<std::endl;
    
    for (Long64_t i=0;i<totentries;i++){
      MainAnalyzer.GetDetectorEntry(i);
      idx=0;
      for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
	
	if(idx<si_.size()){
	  if(si_[idx].back.fMult>0&&si_[idx].back.fChlist[0]==matchback){
	    if(si_[idx].front.fMult>0)
	      (*it).AddHit(si_[idx].Back_E(0),si_[idx].Front_E(0),(int)si_[idx].front.fChlist[0]);
	  }
	}
	idx++;
	
      } 
    }
    idx=0;
    for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
      (*it).PerformFit();
      (*it).PrintCoefficients(Form("%s_front2back.in",si_[idx].Name().c_str()));
    
      DetVar.LoadParams(Form("%s_front2back.in",si_[idx].Name().c_str()));
      si_[idx].SetCalibrations(DetVar);      

      idx++;
      
    }
    
    
    
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

  void Th228Fit(TH1D * h1,double &elin,double &eshift){
    int npeaks=6;
    TSpectrum *s = new TSpectrum(2*npeaks);
    Int_t nfound = s->Search(h1,1,"new");
    printf("Found %d candidate peaks to fitn",nfound);
    int in;
    std::cin>>in;

    Float_t xset[6]={0};
    Float_t *xtemp = s->GetPositionX();
    for(unsigned int i=0;i<6;i++){
      std::cout<<xtemp[i]<<"\n";
      xset[i]=xtemp[i];
  }
    
    Float_t yset[6]={5.42315,5.68537,6.05,6.28808,6.7783,8.78486};
    TF1 *fitter2=new TF1("fitter2","pol1",0,4096);
    TGraph peaks(6,xset,yset);
    peaks.Fit(fitter2,"","",0,4096);
    eshift=fitter2->GetParameter(0);
    elin=fitter2->GetParameter(1);
    
  
  

  }
}



#endif
