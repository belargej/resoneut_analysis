#ifndef RNSIM_CXX
#define RNSIM_CXX
#include <numeric>
#include "RN_Sim.hpp"
#include "RN_Root.hpp"
using namespace global;
using namespace RNROOT;

#define PROTONTHRESHOLD 4.5 //MeV
#define SILICONRESOLUTION 0.1 //MeV

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
  TH1D* hQ_proton_guessLV;
  TH1D* hQ_proton_guessSiA;
  TH1D* hQ_proton_guessSiB;
  TH2D* hQ_proton_guessvpE;
  TH2D* hQ_proton_guessSiBvpE;
  TH2D* hBeamSpot;
  TH2D* hEvThetaSiA;
  TH2D* hEvThetaSiB;

  TF1* TOF_fit_n[NEUTNUM];
  TF1* Q_fit_n[NEUTNUM];
  TF1* Q_fit;
  TF1* TOF_fit;

  double E_deposited(0);
  double n_cm;
  double n_tof;
  double fNe,fNt;
  TVector3 beamspot(0,0,0);


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

    double q_val_p = gReactionInfo.DecayQValueExact();
    
    double q_val_p_guessLV = gReactionInfo.DecayQValueEstimate();
  
    hQ_proton->Fill(q_val_p);
    hQ_proton_guessLV->Fill(q_val_p_guessLV);
   
    if(si_cluster_[0].E()>0&& si_cluster_[1].E()>0){
      double q_val_p_guessSiA = gReactionInfo.DecayQValueEstimate(si_cluster_[0].E(),si_cluster_[0].Theta());
      double q_val_p_guessSiB = gReactionInfo.DecayQValueEstimate(si_cluster_[1].E(),si_cluster_[1].Theta());
      hEvThetaSiA->Fill(si_cluster_[0].Theta()*TMath::RadToDeg(),si_cluster_[0].E());
      hEvThetaSiB->Fill(si_cluster_[1].Theta()*TMath::RadToDeg(),si_cluster_[1].E());
      hQ_proton_guessSiA->Fill(q_val_p_guessSiA);
      hQ_proton_guessSiB->Fill(q_val_p_guessSiB);   
      hQ_proton_guessSiBvpE->Fill(q_val_p_guessSiB,si_cluster_[0].E());   
      hQ_proton_guessvpE->Fill(q_val_p_guessLV,si_cluster_[0].E());   
    }

    hBeamSpot->Fill(beamspot.X(),beamspot.Y());
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
  simtree->Branch("B." + RNROOT::gReactionInfo.BeamName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[0]));
  simtree->Branch("T." + RNROOT::gReactionInfo.TargetName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[1]));
  simtree->Branch("R." + RNROOT::gReactionInfo.RecoilName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[2]));
  simtree->Branch("F." + RNROOT::gReactionInfo.FragmentName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[3]));
  simtree->Branch("D." + RNROOT::gReactionInfo.DecayProductName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[4]));
  simtree->Branch("H." + RNROOT::gReactionInfo.HeavyDecayName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[5]));



  simfile->cd();
  simfile->mkdir("histograms");

  simfile->cd("histograms");
  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",4096,1,128);
  hQ_proton=new TH1D("hQ_proton","hQ_proton;Q",512,-1,10);
  hQ_proton_guessLV=new TH1D("hQ_proton_guessLV","hQ_proton_guessLV;Q",512,-1,10);  
  hQ_proton_guessSiA=new TH1D("hQ_proton_guessSiA","hQ_proton_guessSiA;Q",512,-1,10);
  hQ_proton_guessSiB=new TH1D("hQ_proton_guessSiB","hQ_proton_guessSiB;Q",512,-1,10);
  hQ_proton_guessvpE=new TH2D("hQ_proton_guessvpE","hQ_proton_guessvpE;Q",512,-1,10,512,0,31);
  hQ_proton_guessSiBvpE=new TH2D("hQ_proton_guessSiBvpE","hQ_proton_guessSiBvpE;Q",512,-1,10,512,0,31);
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hEvThetaSiA=new TH2D("hEvThetaSiA","hEvThetaSiA",512,0,45,512,0,30);
  hEvThetaSiB=new TH2D("hEvThetaSiB","hEvThetaSiB",512,0,45,512,0,30);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hBeamSpot = new TH2D("hBeamSpot","hBeamSpot",64,-15.5,15.5,64,-15.5,15.5);
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
    Reset();
    //get cm angle and generate LorentzVectors for sim events(stored in gReaction Info)
    n_cm = gReactionInfo.GenerateSimEvent(); 

    double phirand = 2*TMath::Pi()*myRnd.Rndm();
    double radiusrand = TMath::Sqrt(myRnd.Rndm()) * (gReactionInfo.BeamSpread() / 2);
    double xrand = TMath::Cos(phirand) * radiusrand;
    double yrand = TMath::Sin(phirand) * radiusrand;
    TVector3 nv = gReactionInfo.RecoilLV().Vect();
    TVector3 pv = gReactionInfo.DecayProductLV().Vect();
    beamspot.SetXYZ(xrand,yrand,0);
    
    double decayke=global::myRnd.Gaus(gReactionInfo.DecayProductLV().E()-gReactionInfo.DecayProductLV().M(),SILICONRESOLUTION);


    for(unsigned int i=0;i<neut.size();i++){
      if(neut[i].inDet(nv,beamspot)){
	NeutronIn[i]++;
	if(neut[i].NeutIn(gReactionInfo.RecoilLV(),n_tof,E_deposited,beamspot)){
	  NeutronDetected[i]++;
	  for(unsigned int s=0;s<si_.size();s++){
	    if(si_[s].inDet(pv,beamspot) && decayke > PROTONTHRESHOLD ){
	      ProtonIn_NeutDet[s]++;
	    }
	  }
	}
      }
    }
    
    for(unsigned int s=0;s<si_.size();s++){
      if(si_[s].inDet(pv,beamspot) && decayke > PROTONTHRESHOLD){
	ProtonIn[s]++;
	double chf=0;
	double chb=0;
	
	si_[s].Vect_to_ch(pv,chf,chb,beamspot);
	si_[s].front.InsertHit(decayke,0,chf);
	si_[s].back.InsertHit(decayke,0,chb);
      }
      
      si_cluster_[s].ReconstructClusters(si_[s]);
    }
    
    
    return 1;
  }   
  

  
  void RN_Sim::Reset(){
    E_deposited=0;
    n_tof=0;

    gReactionInfo.Reset();
    beamspot.SetXYZ(0,0,0);
    
    particle.Reset();

    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
      (*it).Reset();  
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      (*it).Reset(); 
    }
    for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
      (*it).Reset(); 
    }
 
    
  }

  
  void RN_Sim::WriteOut(){
    
    hQ->Fit(Q_fit,"","",-4,4);
    hn_tof->Fit(TOF_fit,"","",1,128);
    simlog<<"Reaction :"<<gReactionInfo.BeamName()<<"("<<gReactionInfo.TargetName()<<","<<gReactionInfo.RecoilName()<<")"<<gReactionInfo.FragmentName()<<"\n";
    simlog<<"Decay :"<<gReactionInfo.FragmentName()<<"->"<<gReactionInfo.DecayProductName() << "+" << gReactionInfo.HeavyDecayName()<<"\n\n";
      
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
