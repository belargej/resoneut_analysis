#include "S2_Analyzer.hpp"


S2_Analyzer::S2_Analyzer():ind_(0)
{

}

void S2_Analyzer::LoadGates(const char *a){
 


}

void S2_Analyzer::AutoCalibrate(int matchfront, int matchback){
  std::cout<<"calibrating back to front match channel : "<<matchfront<<std::endl;
  RN_S2CalCollection s2front;
  RN_S2CalCollection s2back;
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    s2front.push_back(RN_S2Calibrator(16,Form("%s.front",(*it).Name().c_str()),60,3000));
    s2back.push_back(RN_S2Calibrator(16,Form("%s.back",(*it).Name().c_str()),60,3000));
    
  }
  int idx=0;

  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);
    
    idx=0;
    for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
      
      if(idx<si_.size()){
	if(si_[idx].front.fMult>0&&si_[idx].front.fChlist[0]==matchfront){
	  if(si_[idx].back.fMult>0)
	    (*it).AddHit(si_[idx].front.fE[0],si_[idx].back.fE[0],si_[idx].back.fChlist[0]);
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

    std::cout<<"sleeping for 5 seconds"<<std::endl;
    usleep(5000000);
    
      
    DetVar.LoadParams(Form("%s_back2front.in",si_[idx].Name().c_str()));
    si_[idx].SetCalibrations(DetVar);
    idx++;
    
  }


  std::cout<<"repeating now for front 2 back channel : "<<matchback<<std::endl;

  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);
    ApplyCalibrations();
    idx=0;
    for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
      
      if(idx<si_.size()){
	if(si_[idx].back.fMult>0&&si_[idx].back.fChlist[0]==matchback){
	  if(si_[idx].front.fMult>0)
	    (*it).AddHit(si_[idx].back.fE[0],si_[idx].front.fE[0],si_[idx].front.fChlist[0]);
	}
      }
      idx++;

    } 
  }
  idx=0;
  for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
    (*it).PerformFit();
    (*it).PrintCoefficients(Form("%s_front2back.in",si_[idx].Name().c_str()));
    
    idx++;
    
  }



}
  



void S2_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}

void S2_Analyzer::initHists(std::string output,int ind){ 
  ind_ = ind;
  rootfile=new TFile(output.c_str(),"RECREATE");
  for(int i=0;i<16;i++){
    front[i]=new sak::Hist2D(Form("si_fc%d_corr",i),"channel","ratio",17,0,16,512,0,2);
  }

  
  
}

void S2_Analyzer::Process(){
  ApplyCalibrations();
  int idx=(int)si_[ind_].front.fChlist[0];
  if(si_[ind_].front.fMult>0&&si_[ind_].back.fMult>0)
    front[idx]->Fill(si_[ind_].back.fChlist[0],(si_[ind_].back.fE[0]/si_[ind_].front.fE[0]));
  

}
void S2_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
