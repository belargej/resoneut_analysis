#include "S2_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "PSD_Analyzer.hpp"
namespace si_cal{
  TFile *rootfile;

  sak::Histogram2D *hpede;  
  sak::Histogram1D *h_si_[2];
  sak::Histogram2D *h_evtheta[2];
  sak::Histogram2D *h_evtheta_protgated[2];
  sak::Histogram2D *h_evtheta_neutgated[2];
  sak::Histogram2D *front[2][16];
  sak::Histogram1D *h_si_fmult[2];
  sak::Histogram1D *h_si_bmult[2];
  sak::Histogram1D *h_si_cluster_mult[2];
  sak::Hist1D *h_nmult_pgated;
  sak::Hist1D *h_nmult_p_etheta_gated;
  sak::Hist1D *h_ntime[10];
  sak::Hist1D *h_ntime_pgated[10];

S2_Analyzer::S2_Analyzer():ind_(0)
{

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
    idx=0;
    for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
      
      if(idx<si_.size()){
	if(si_[idx].back.fMult>0&&si_[idx].back.fChlist[0]==matchback){
	  if(si_[idx].front.fMult>0)
	    (*it).AddHit(si_[idx].Back_E(0),si_[idx].Front_E(0),si_[idx].front.fChlist[0]);
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
 
void S2_Analyzer::Begin(){     
  rootfile=new TFile("si_analysis.root","RECREATE");

  rootfile->mkdir("f2b_ratio");
  rootfile->mkdir("f2b_ratio/S1");
  rootfile->mkdir("f2b_ratio/S2");
  rootfile->mkdir("ede");
  rootfile->mkdir("evtheta");
  rootfile->mkdir("mult");
  rootfile->mkdir("neuts");
  h_si_[0]=new sak::Hist1D("h_si_1","E[MeV]",128,1,20);
  h_si_[1]=new sak::Hist1D("h_si_2","E[MeV]",128,1,20);
  
  rootfile->cd("f2b_ratio");
  for(int i=0;i<16;i++){
    rootfile->cd("f2b_ratio/S1");
    front[0][i]=new sak::Hist2D(Form("s1_fc%d_corr",i),"channel","ratio",17,0,16,512,0,2);
    rootfile->cd("f2b_ratio/S2");
    front[1][i]=new sak::Hist2D(Form("s2_fc%d_corr",i),"channel","ratio",17,0,16,512,0,2);
  }
  rootfile->cd("ede");
   hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
  
  for(int i=0;i<2;i++){
    rootfile->cd("evtheta");
    h_evtheta[i]=new sak::Hist2D(Form("h_evtheta[%d]",i+1),"theta[deg]","E",256,0,128,128,0,32);
    h_evtheta_protgated[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]",i+1),"theta[deg]","E",256,0,128,128,0,32);
    rootfile->cd("mult");
    h_si_fmult[i]= new sak::Hist1D(Form("h_si_fmult_%d",i),"fmult",32,0,31);
    h_si_bmult[i]= new sak::Hist1D(Form("h_si_bmult_%d",i),"bmult",32,0,31);
    h_si_cluster_mult[i] = new sak::Hist1D(Form("h_si_%d_cluster_mult",i),"mult",32,0,31);
    
    
  }
  rootfile->cd("neuts");
  h_nmult_pgated = new sak::Hist1D("h_nmult_pgated","mult",10,0,9);
  h_nmult_p_etheta_gated = new sak::Hist1D("h_nmult_p_etheta_gated","mult",10,0,9);
  for(int i=0;i<10;i++){
    h_ntime[i]=new sak::Hist1D(Form("h_ntime%d",i),"mult",4096,0,4095);
    h_ntime_pgated[i]=new sak::Hist1D(Form("h_ntime%d_pgated",i),"mult",4096,0,4095);
    
  }
  
  
}

void S2_Analyzer::Process(){
  int protcheck(0);
  double prot_E=0;
  double prot_dE=0;
  double prot_theta=0;
  
  
  if(si_[1].front.fMult>0&&si_[0].front.fMult>0){
    prot_dE=si_[1].Front_E(0);
    prot_E=si_[0].Front_E(0)+prot_dE;
    hpede->Fill(prot_E,prot_dE);
    prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
  }
  if(psd::prots1)
    protcheck=psd::prots1->IsInside(prot_E,prot_dE);
  

  for(int i=0;i<2;i++){
    int idx=(int)si_[i].front.fChlist[0];	      
    if(si_[0].front.fMult>0&&si_[0].back.fMult>0){
      for (int j=0;j<10;j++){
	h_ntime[j]->Fill(neut[j].fT_Q);
	if(protcheck)
	  h_ntime_pgated[j]->Fill(neut[j].fT_Q);
      }
      front[i][idx]->Fill(si_[0].back.fChlist[0],(si_[0].Back_E(0)/si_[0].Front_E(0)));
      if(si_cluster_[i].fMult>0){
	h_evtheta[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cluster_[i].fE[0]);
	if(protcheck){
	  h_nmult_pgated->Fill(Narray.fMult);
	  h_evtheta_protgated[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cluster_[i].fE[0]);
	}
      }
    h_si_[i]->Fill(si_[i].Front_E(0));
    h_si_fmult[i]->Fill(si_[i].front.fMult);
    h_si_bmult[i]->Fill(si_[i].back.fMult);
    h_si_cluster_mult[i]->Fill(si_cluster_[i].fMult);
    }
    
  }
 

}


void S2_Analyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();

}

  void S2_Analyzer::Clear(){

  }
    
    

  
  void producehists(const char * input,const char* output,int index,const char* config){
    S2_Analyzer a;
    a.Init(input);
    if(config){
      LoadVariableFile(config);
      SetCalibrations();
    }
    a.Loop();
    
    
  }

  void autocalibrate(const char* input,int fmatch,int bmatch){
    S2_Analyzer a;
    a.Init(input);
    a.AutoCalibrate(fmatch,bmatch);
  }
  
}
