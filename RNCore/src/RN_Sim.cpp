#ifndef RNSIM_CXX
#define RNSIM_CXX
#include <numeric>
#include "RN_Sim.hpp"
#include "RN_Root.hpp"
using namespace global;
using namespace RNROOT;

namespace sim{
  TFile * simfile;
  TTree * simtree;
  RN_Particle particle;
  ofstream simlog;
  std::vector<Int_t> NeutronIn;
  std::vector<Int_t> NeutronDetected;
  std::vector<Int_t> ProtonIn;
  std::vector<Int_t> ProtonIn_NeutDet;
  //declare histograms here
  TH1D* hn_tof;
  TH1D* htof_n[NEUTNUM];
  TH2D* hE_n;
  TH2D* hE_v_theta;
  TH2D* hT_v_theta;
  TH1D* hn_CM;
  TH2D* hn_CMvLab;
  TH2D* hpos;
  TH2D* hpos_in;
  TH1D* hQ;
  TH1D* hQ_n[NEUTNUM];
  TH1D* h_nKE;
  TH1D* h_hiKE;
  TH1D* hQ_proton;
  TH1D* hQ_proton_guess;
  TF1* TOF_fit_n[NEUTNUM];
  TF1* Q_fit_n[NEUTNUM];
  TF1* Q_fit;
  TF1* TOF_fit;

  double E_deposited(0);
  double n_cm;
  double n_tof;
  double fNe,fNt;
  double q_val_p(0);
  double q_val_p_guess(0);

  Long64_t totevents;

  RN_ParticleGun *particlegun;
  
  void RN_Sim::Init(){
    NeutronIn.clear();
    NeutronDetected.clear();
    ProtonIn.clear();
    for(unsigned int i=0;i<neut.size();i++){
      NeutronIn.push_back(0);
      NeutronDetected.push_back(0);
    }
    for(unsigned int i=0;i<si_.size();i++){
      ProtonIn.push_back(0);
      ProtonIn_NeutDet.push_back(0);
    }
       
  }



  void RN_Sim::StartRun(std::string input){
    Init();
    
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
	gVariableMap.AddParam(input[1],sak::string_to_double(input[2]));
      }
      else if(input[0]=="fDWBA")
	{
	  gReactionInfo.SetAngularDistribution(input[1]);
	
	}
      else if(input[0]=="fReaction")
	{
	  std::string plist[6];
	  if (input.size()==7){
	    for(unsigned int i=0;i<6;i++){
	      plist[i] = input[i+1];
	    }
	    gReactionInfo.SetReaction(plist[0],
				      plist[1],
				      plist[2],
				      plist[3],
				      plist[4],
				      plist[5]);
	  }
	  else {
	    std::cout<<"incorrect number of entries to fReaction: "<< input.size()-1<<"but needed 6"<<std::endl;
	    exit(EXIT_FAILURE);
	  }
      }
      else if(input[0]=="fParticleGun"){
	if (input.size()==6){
	  particlegun=new RN_ParticleGun(input[1],
				       sak::string_to_double(input[2]),//emin
					 sak::string_to_double(input[3]),//emax
					 sak::string_to_double(input[4]),//thetamin
					 sak::string_to_double(input[5])//thetamax
				       );
	  
	  sim::particle.Init(input[1]);	
	}
	else {
	  std::cout<<"incorrect number of inputs for fParticle gun: "<<input.size()<<". Need particle name emin emax thetamin thetamax"<<std::endl; 
	  exit(EXIT_FAILURE);
	}
      }
      
      else if(input[0]=="fOutput")
	{
	if(input.size()==3){
	  simfile=new TFile(input[1].c_str(),"RECREATE");
	  simlog.open(input[2].c_str());
	}
	else std::cout<<"invalid number of arguments for fOutput"<<std::endl;
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
    cfg.close();
  
  Loop(totevents,option);

    

  }



  void RN_Sim::FillHistograms(){
    TLorentzVector recoilLV = gReactionInfo.RecoilLV();
    int cref(0);    
    hE_v_theta->Fill(recoilLV.Theta()*180/3.14,recoilLV.E()-recoilLV.M());
    hn_CM->Fill(n_cm*180/3.14);
    hn_CMvLab->Fill(n_cm*180/3.14,gReactionInfo.RecoilLV().Theta()*180/3.14);
    
    cref = 0 ;
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if((*it).GetHitPos().X()!=0)
	hpos_in->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
      if((*it).fTSim>0){
	hn_tof->Fill(((*it).fTSim));
	htof_n[cref]->Fill((*it).fTSim);
	hE_n->Fill(cref,(*it).fESum);
	hpos->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
	double nKE=0,hiKE=0;
	double q_value=gReactionInfo.RecoilQValue((*it).GetPosVect().Z(),(*it).fTSim,nKE,hiKE);
	hQ->Fill(q_value);
	hQ_n[cref]->Fill(q_value);
	h_nKE->Fill(nKE);
	h_hiKE->Fill(hiKE);
	hT_v_theta->Fill(particle.LV.Theta()*180/3.14,(*it).fTSim);
	
      }
      cref++;
  }

    q_val_p = gReactionInfo.DecayQValueExact();
    
    q_val_p_guess = gReactionInfo.DecayQValueEstimate();

    
    hQ_proton->Fill(q_val_p);
    hQ_proton_guess->Fill(q_val_p_guess);

    simtree->Fill();
    
  }
  
  void RN_Sim::initHists(){
    if(!simfile){
      std::cout<<"no simfile set"<<std::endl;
      exit(EXIT_FAILURE);
    }
  def=1;
  simfile->cd();
  
  simtree = new TTree("SimTree","SimTree");
  simtree->Branch("Beam." + RNROOT::gReactionInfo.BeamName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[0]));
  simtree->Branch("Target." + RNROOT::gReactionInfo.TargetName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[1]));
  simtree->Branch("Recoil." + RNROOT::gReactionInfo.RecoilName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[2]));
  simtree->Branch("Fragment." + RNROOT::gReactionInfo.FragmentName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[3]));
  simtree->Branch("DecayProduct." + RNROOT::gReactionInfo.DecayProductName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[4]));
  simtree->Branch("HeavyDecay." + RNROOT::gReactionInfo.HeavyDecayName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[5]));
  simtree->Branch("ErProtonGuess",&q_val_p_guess);
  simtree->Branch("ErProton",&q_val_p);

  simfile->cd();
  simfile->mkdir("histograms");

  simfile->cd("histograms");
  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",4096,1,128);
  hQ_proton=new TH1D("hQ_proton","hQ_proton;Q",512,-1,10);
  hQ_proton_guess=new TH1D("hQ_proton_guess","hQ_proton_guess;Q",512,-1,10);
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hpos=new TH2D("hpos","hpos",64,-256,256,64,-256,256);
  hpos_in=new TH2D("hpos_in","hpos_in",64,-256,256,64,-256,256);  
  hE_n=new TH2D("hE_n","hE_n",17,0,16,512,0,5);
  TOF_fit=new TF1("tof_fit","gaus",1,128);
  hQ=new TH1D("hQ","hQ",512,-4,4);
  Q_fit=new TF1("Q_fit","gaus",-4,4);

  for(unsigned int i=0;i<neut.size();i++){
    simfile->cd("histograms");
    gDirectory->mkdir(Form("neut%d",i));
    gDirectory->cd(Form("neut%d",i));
    gDirectory->mkdir("tof");
    gDirectory->mkdir("Q");
    simfile->cd(Form("histograms/neut%d/tof",i));
    htof_n[i]=new TH1D(Form("htof_n%d",i),Form("htof_n%d",i),512,1,128);  
    TOF_fit_n[i]=new TF1(Form("tof_fit_n%d",i),"gaus",1,128);
    simfile->cd(Form("histograms/neut%d/Q",i));
    hQ_n[i]=new TH1D(Form("hQ_n%d",i),Form("hQ_n%d",i),512,-4,4);
    Q_fit_n[i]=new TF1(Form("Q_fit_n%d",i),"gaus",-4,4);
  }




}


  void RN_Sim::Loop(Long64_t evnum,std::string options){
    initHists();
    SetCalibrations();
    Long64_t evcount=0;
    while(evcount<evnum){
      if(GenerateEvents(evcount,options)){
	evcount++;
      }
      else
	continue;
      if(def==1)FillHistograms();
      if(evcount%30000==0)std::cout<<evcount<<std::endl;
    }
    WriteOut();
  }
  
  
  void RN_Sim::SetAngularDistribution(std::string filename){
    nDWBA = RN_AngularDistribution(filename);
  }
  
  int RN_Sim::GenerateSingleParticleEvent(Long64_t evnum){
    Reset();
    particlegun->Shoot(particle.LV); //shoot a particle defined by particle gun into the LV of particle[0]
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if((*it).inDet(particle.LV.Vect())){
	(*it).NeutIn(particle.LV,n_tof,E_deposited);  
	return 1;
      }
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      if((*it).inDet(particle.LV.Vect())){
	double e=particle.LV.E()-particle.LV.M();
	double t=0;
	(*it).front.InsertHit(e,t,0);
	return 1;
      }
    }

    return 0;
  }
  
  
  int RN_Sim::GenerateEvents(Long64_t evnum,std::string options=""){
    for(unsigned int i=0;i<neut.size();i++){
      neut[i].Reset();
    }
    for(unsigned int i=0;i<si_.size();i++){
      si_[i].Reset();
    }

    n_cm = gReactionInfo.GenerateSimEvent();
   
    for(unsigned int i=0;i<neut.size();i++){
      if(neut[i].inDet(gReactionInfo.RecoilLV().Vect())){
	NeutronIn[i]++;
	if(neut[i].NeutIn(gReactionInfo.RecoilLV(),n_tof,E_deposited)){
	  NeutronDetected[i]++;
	  for(unsigned int s=0;s<si_.size();s++){
	    if(si_[s].inDet(gReactionInfo.DecayProductLV().Vect())){
	      ProtonIn_NeutDet[s]++;
	    }
	  }
	}
      }
    }
    
    for(unsigned int s=0;s<si_.size();s++){
      if(si_[s].inDet(gReactionInfo.DecayProductLV().Vect())){
	ProtonIn[s]++;
	double e=gReactionInfo.DecayProductLV().E()-gReactionInfo.DecayProductLV().M();
	double t=0;
	si_[s].front.InsertHit(e,t,0);
      }
    }
    
    
    return 1;
  }   
  

  
  void RN_Sim::Reset(){
    E_deposited=0;
    n_tof=0;
    q_val_p=0;
    q_val_p_guess = 0;
    
    particle.Reset();

    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
      (*it).Reset();  
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      (*it).Reset(); 
    }
 
    
  }

  
  void RN_Sim::WriteOut(){
    
    hQ->Fit(Q_fit,"","",-4,4);
    hn_tof->Fit(TOF_fit,"","",1,128);
    simlog<<"Reaction :"<<gReactionInfo.BeamName()<<"("<<gReactionInfo.TargetName()<<","<<gReactionInfo.RecoilName()<<")"<<gReactionInfo.FragmentName()<<"\n";
    simlog<<"Decay :x"<<gReactionInfo.FragmentName()<<"->"<<gReactionInfo.DecayProductName() << "+" << gReactionInfo.HeavyDecayName()<<"\n\n";
      
      simlog<<"Beam Energy: "<<gReactionInfo.BeamEnergy()<<"\n";
      simlog<<"Beam E_loss(thickness): "<<gReactionInfo.BeamELoss()<<"\n";
      simlog<<"Excitation Energy: "<<gReactionInfo.Hi_Ex_Set()<<"\n";
      simlog<<"Number of Neutron Detectors: "<<neut.size()<<"\n";
      simlog<<"Total Entries: "<<totevents<<"\n\n";

      simlog<<"All Detectors: \n";
      simlog<<"NeutronsDetected/NeutronsIn: "<<std::accumulate(NeutronDetected.begin(),NeutronDetected.end(),0)<<" / "<<std::accumulate(NeutronIn.begin(),NeutronIn.end(),0)<<"\n";
      
      simlog<<"Q_Value: "<<Q_fit->GetParameter(1)<<"\n ";
      simlog<<"Q_Value Resolution: "<<Q_fit->GetParameter(2) * 2.355<<"\n";      
      simlog<<"TOF_Value: "<<TOF_fit->GetParameter(1)<<"\n ";
      simlog<<"TOF_Value Resolution: "<<TOF_fit->GetParameter(2) * 2.355<<"\n\n";
      for(unsigned int i=0;i<si_.size();i++){
	simlog<<"***************************************************************\n";
	simlog<<si_[i].GetName()<<"\n";
	simlog<<"x,y,z: "<<si_[i].GetPosVect().X()<<" "
	      <<si_[i].GetPosVect().Y()<<" "
	      <<si_[i].GetPosVect().Z()<<"\n";
	simlog<<"Theta Range"<<si_[i].InnerTheta()<<" - "<<si_[i].OuterTheta()<<"\n";
	simlog<<"Protons&Neutron Coincidence / Protons In: "<< ProtonIn_NeutDet[i]<<" / "<<ProtonIn[i]<<"\n\n";
	
      }
          
      for(unsigned int i=0;i<neut.size();i++){
      
      htof_n[i]->Fit(TOF_fit_n[i],"","",1,128);
      hQ_n[i]->Fit(Q_fit_n[i],"","",-4,4);




      simlog<<"***************************************************************\n";
      simlog<<neut[i].GetName()<<"\n";
      simlog<<"Thickness: "<<neut[i].GetThickness()<<"\n";
      simlog<<"Threshold: "<<neut[i].GetThreshold()<<"\n";
      simlog<<"ArrayPos: "<<neut[i].GetArrayPos()<<"\n\n";

      simlog<<"NeutronsDetected/NeutronsIn: "<<NeutronDetected[i]<<" / "<<NeutronIn[i]<<"\n";
      simlog<<"Individual Efficiency: "<<(double)NeutronDetected[i]/NeutronIn[i]<<"\n";
      simlog<<"Q_Value: "<<Q_fit_n[i]->GetParameter(1)<<"\n ";
      simlog<<"Q_Value Resolution: "<<Q_fit_n[i]->GetParameter(2) * 2.355<<"\n";

      simlog<<"TOF_Value: "<<TOF_fit_n[i]->GetParameter(1)<<"\n ";
      simlog<<"TOF_Value Resolution: "<<TOF_fit_n[i]->GetParameter(2) * 2.355<<"\n" ;

      
    }



    simfile->Write();
    simfile->Close();
    simlog.close();
  }

}

#endif
