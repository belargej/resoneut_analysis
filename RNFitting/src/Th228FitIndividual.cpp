/***********************************************************/
//Class: Th228FitIndividual
//
//Author:Sean Kuvin
//
//This class calculates any angular dependence on the phi segment energy
//The dependence is used to determine the dead layer thickness for the detector.
//The only input that is necessary is a root file which contains data
//from a Th228 alpha source.  
/***********************************************************/

#ifndef _TH228FITIND_CXX
#define _TH228FITIND_CXX

#include "TGraph.h"
#include "TSpectrum.h"
#include "TF1.h"
#include "Th228FitIndividual.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace si_cal{
  TH1D *hrings[SI_NUM][16];
  TH1D *hsegs[SI_NUM][16];
  TF1  *fitrings[SI_NUM][16];
  TF1  *fitsegs[SI_NUM][16];
  
  
  Th228FitIndividual::Th228FitIndividual()
  {
    
  }
  
  void Th228FitIndividual::Reset(){
    
  }

  
  bool Th228FitIndividual::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    fgRootFile->mkdir("Th228FitIndividual");
    for(unsigned int i=0;i<si_.size();i++){
      fgRootFile->cd("Th228FitIndividual");
      gDirectory->mkdir(si_[i].GetName());
      gDirectory->cd(si_[i].GetName()); 
      for(unsigned int j = 0 ; j<16;j++){
	hrings[i][j] = new TH1D(Form("%s_ring%d",si_[i].GetName(),j),Form("%s_ring%d",si_[i].GetName(),j),512,300,2347);
	hsegs[i][j] = new TH1D(Form("%s_segs%d",si_[i].GetName(),j),Form("%s_segs%d",si_[i].GetName(),j),512,300,2347);
	fitrings[i][j] = new TF1(Form("%sfitrings%d",si_[i].GetName(),j),"pol1",0,4096);
	fitsegs[i][j] = new TF1(Form("%sfitsegs%d",si_[i].GetName(),j),"pol1",0,4096);

      }
    }
    
  
    return 1;
  }  
  
  

  bool Th228FitIndividual::Process(){

       
    return 1;
  }
  bool Th228FitIndividual::ProcessFill(){

     for(unsigned int k=0;k<si_.size();k++){
      int bch = (int) si_[k].back.ChRaw();
      int fch = (int) si_[k].front.ChRaw();
      double be = si_[k].back.EOffset();
      double fe = si_[k].front.EOffset();
      if(bch>=0){
	hsegs[k][bch]->Fill(be);
      }
      if(fch>=0){
	hrings[k][fch]->Fill(fe);
      }
      
    }
    
    return 1;
    
  }


  bool Th228FitIndividual::Terminate(){
    //Double_t th228values[6] = {5.42315,5.68537,6.05,6.28808,6.77803,8.74886};
    //Double_t th228valuesB[5] = {5.42315,5.68537,6.28808,6.77803,8.74886};
    //    Double_t th228values[6] = {5.27815,5.54170,5.91958,6.15230,6.64851,8.67163};
    //Double_t th228valuesB[5] = {5.27815,5.54170,6.15230,6.64851,8.67163};
    Double_t th228values[6] = {5.330104,5.595797,5.97154,6.216245,6.698578,8.717222};
    Double_t th228valuesB[5] = {5.330104,5.595797,6.216245,6.698578,8.717222};



    ofstream coeff;
    coeff.open("th228fitOutput.out");
    if (!coeff.is_open()){
      std::cout << "  Could not create th228fitOutput.out" << std::endl;
      return 0;
    }
    
    TSpectrum *sring = new TSpectrum();
    TSpectrum *sseg = new TSpectrum();
      for(unsigned int i = 0;i<si_.size();i++){
	TString detname = si_[i].GetName();
	for(unsigned int j=0;j<16;j++){
	  TGraph * graphring = new TGraph();
	  TGraph * graphseg = new TGraph();
	  Int_t PeaksFoundRing =  sring->Search(hrings[i][j],2,"",0.3);
	  Int_t PeaksFoundSeg =  sseg->Search(hsegs[i][j],2,"",0.3);
	  Float_t *xpeaksring = sring->GetPositionX();
	  Float_t *xpeaksseg = sseg->GetPositionX();
	  std::vector<Float_t> sortedpeaksring;
	  std::vector<Float_t> sortedpeaksseg;
	  for(int k=0;k<PeaksFoundRing;k++){
	    sortedpeaksring.push_back(xpeaksring[k]);
	  }
	  for(int k=0;k<PeaksFoundSeg;k++){
	    sortedpeaksseg.push_back(xpeaksseg[k]);
	  }
	  std::sort(sortedpeaksring.begin(),sortedpeaksring.end());
	  std::sort(sortedpeaksseg.begin(),sortedpeaksseg.end());
	  for(int k=0;k<PeaksFoundRing;k++){
	    if(PeaksFoundRing==6){
	      graphring->SetPoint(k,sortedpeaksring[k],th228values[k]);
	    }
	    else if(PeaksFoundRing == 5){
	      graphring->SetPoint(k,sortedpeaksring[k],th228valuesB[k]);
	    }

	    coeff<<"peak "<<k<<" "<<sortedpeaksring[k]<<"\n";      
	  }
	  for(int k=0;k<PeaksFoundSeg;k++){
	    if(PeaksFoundSeg==6){
	      graphseg->SetPoint(k,sortedpeaksseg[k],th228values[k]);
	    }
	    else if(PeaksFoundSeg == 5){
	      graphseg->SetPoint(k,sortedpeaksseg[k],th228valuesB[k]);
	    }
	    coeff<<"peak "<<k<<" "<<sortedpeaksseg[k]<<"\n";      
	  }
	  if(PeaksFoundRing==5 || PeaksFoundRing == 6){
	    graphring->Fit(fitrings[i][j]);
	  }
	  else{
	    coeff<<"not fitting "<<detname +".ring"<<j<<"\n";
	  }
	  if(PeaksFoundSeg==6 || PeaksFoundSeg == 5){
	    graphseg->Fit(fitsegs[i][j]);
	  }
	  else{
	    coeff<<"not fitting "<<detname +".seg"<<j<<"\n";
	  }
	  delete graphring;
	  delete graphseg;
	}
      }
      for(unsigned int i=0;i<si_.size();i++){
	TString detname = si_[i].GetName();
	for(unsigned int j=0;j<16;j++){
	  coeff<<detname+ ".back.a0["<<j<<"]"<<" "<<fitsegs[i][j]->GetParameter(0)<<"\n";
	  coeff<<detname+ ".back.a1["<<j<<"]"<<" "<<fitsegs[i][j]->GetParameter(1)<<"\n";	
	  coeff<<detname+ ".front.a0["<<j<<"]"<<" "<<fitrings[i][j]->GetParameter(0)<<"\n";
	  coeff<<detname+ ".front.a1["<<j<<"]"<<" "<<fitrings[i][j]->GetParameter(1)<<"\n";	
	}
      }
    
      return 1;
  }
  void Th228FitIndividual::Clear(){
    
  }
  
  
  
}


#endif
