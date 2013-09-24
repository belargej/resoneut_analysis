#ifndef RN_SIMRUNCXX
#define RN_SIMRUNCXX
#include "RN_SimRun.hpp"
#include "RN_Root.hpp"

namespace sim{

  TFile * rootfile;
  TTree * tree;
  
  //declare histograms here
  TH1D* hn_tof;
  TH2D* htof_n;
  TH2D* hE_n;
  TH2D* hE_v_theta;
  TH2D* hT_v_theta;
  TH1D* hn_CM;
  TH2D* hn_CMvLab;
  TH2D* hpos;
  TH1D* hQ;
  TH1D* h_nKE;
  TH1D* h_hiKE;
  

  
  
RN_SimRun::RN_SimRun():def(0){
}


void RN_SimRun::Loop(Long64_t evnum,std::string options){
  SetCalibrations();
  Long64_t evcount=0;
  while(evcount<evnum){
    if(GenerateEvents(evcount,options))
      evcount++;
    else
      continue;
    if(def==1)FillHistograms();
    if(evcount%30000==0)std::cout<<evcount<<std::endl;
  }  
}

void RN_SimRun::initHists(){
  def=1;
  for(unsigned int i=0;i<particle.size();i++){
    tree->Branch(Form("%s.",particle[i].Name().c_str()),"RN_Particle",&particle[i]);
  }
  for(unsigned int i=0;i<neut.size();i++){
    tree->Branch(Form("%s.",neut[i].Name().c_str()),"RN_NeutDetector",&neut[i]);
  }


  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",4096,1,128);
  htof_n=new TH2D("htof_n0","htof_n0",17,0,16,512,1,128);
  hE_n=new TH2D("hE_n","hE_n",17,0,16,512,0,5);
  hQ=new TH1D("hQ","hQ",4096,-4,0);
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hpos=new TH2D("hpos","hpos",1024,-256,256,1024,-256,256);
}

void RN_SimRun::FillHistograms(){
  tree->Fill();
  hE_v_theta->Fill(particle[2].LV.Theta()*180/3.14,particle[2].LV.E()-particle[2].LV.M());
  hn_CM->Fill(n_cm*180/3.14);
  hn_CMvLab->Fill(n_cm*180/3.14,particle[2].LV.Theta()*180/3.14);
 
  int cref=0;
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
    if((*it).fT_Sim>0){
      hn_tof->Fill(((*it).fT_Sim));
      htof_n->Fill(cref,(*it).fT_Sim);
      hE_n->Fill(cref,(*it).fEsum);
      hT_v_theta->Fill(particle[2].LV.Theta()*180/3.14,(*it).fT_Sim);
      hpos->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
      double nKE=0,hiKE=0;
      double q_value=QValue((*it).GetPosVect().Z(),(*it).fT_Sim,nKE,hiKE);
      hQ->Fill(q_value);
      h_nKE->Fill(nKE);
      h_hiKE->Fill(hiKE);
    }
    cref++;
  }
 
}

void RN_SimRun::StartRun(std::string input){
  Init();
  RN_RootInit();

  std::string temp;
  ifstream cfg(input.c_str());
  if(!cfg){
    std::cout<<"could not cfg "<<input<<std::endl;
    std::cin>>temp;
    return ;
  }

  do{
    std::vector<std::string>input;
    sak::ReadLine(cfg,input);
    if(input.size()==0)
      continue;
    if(input[0]=="fParam")
      {      
	DetVar.AddParam(input[1],sak::string_to_double(input[2]));
      }
    else if(input[0]=="fDWBA")
      {
	SetAngularDistribution(input[1]);
	
      }
    else if(input[0]=="fReaction")
      {
	if (input.size()==7){
	  particle.clear();
	  for(unsigned int i=1;i<7;i++)
	    particle.push_back(RN_Particle(input[i]));
	}
	else std::cout<<"incorrect number of entries to fReaction: "<< input.size()-1<<"but needed 6"<<std::endl;
      }
    else if(input[0]=="fOutput")
      {
	rootfile=new TFile(input[1].c_str(),"RECREATE");
	tree=new TTree("sim","sim");
	if(input.size()==2)
	  initHists();
	else
	  std::cout<<"invalid number of arguments for fOutput"<<std::endl;
      }
    else if(input[0]=="fEvents")
      {
	totevents=sak::string_to_int(input[1]);
      }
    else if(input[0]=="fOption")
      {
	option=input[1];
      }

    else
      continue;
  } while(!cfg.eof());
  
  Loop(totevents,option);

    

}


void RN_SimRun::WriteOut(){

  rootfile->Write();
  rootfile->Close();

}


}
#endif
