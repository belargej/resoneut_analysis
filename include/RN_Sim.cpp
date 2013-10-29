#ifndef RNSIM_CXX
#define RNSIM_CXX
#include <numeric>
#include "RN_Sim.hpp"
#include "RN_Root.hpp"
using namespace global;

namespace sim{

  TFile *simfile;
  TTree *tree;
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
  TF1* TOF_fit_n[NEUTNUM];
  TF1* Q_fit_n[NEUTNUM];
  TF1* Q_fit;
  TF1* TOF_fit;

  double E_deposited;
  double n_cm;
  double n_tof;
  double fNe,fNt;

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
	  particle.clear();
	  particle.push_back(RN_Particle(input[1]));	
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
  
  Loop(totevents,option);

    

}



  void RN_Sim::FillHistograms(){
    tree->Fill();
    if(particle.size()>1){
      hE_v_theta->Fill(particle[2].LV.Theta()*180/3.14,particle[2].LV.E()-particle[2].LV.M());
      hn_CM->Fill(n_cm*180/3.14);
      hn_CMvLab->Fill(n_cm*180/3.14,particle[2].LV.Theta()*180/3.14);
    }
  int cref=0;
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if((*it).GetHitPos().X()!=0)
    hpos_in->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
    if((*it).fT_Sim>0){
      hn_tof->Fill(((*it).fT_Sim));
      htof_n[cref]->Fill((*it).fT_Sim);
      hE_n->Fill(cref,(*it).fEsum);
      hpos->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
      double nKE=0,hiKE=0;
      double q_value=QValue((*it).GetPosVect().Z(),(*it).fT_Sim,nKE,hiKE);
      hQ->Fill(q_value);
      hQ_n[cref]->Fill(q_value);
      h_nKE->Fill(nKE);
      h_hiKE->Fill(hiKE);
      if(particle.size()!=1)
	hT_v_theta->Fill(particle[2].LV.Theta()*180/3.14,(*it).fT_Sim);

    }
    cref++;
  }

  
  }

void RN_Sim::initHists(){
  if(!simfile){
    std::cout<<"no simfile set"<<std::endl;
    exit(EXIT_FAILURE);
  }
  def=1;
  simfile->cd();
  
   tree=new TTree("sim","sim");
     for(unsigned int i=0;i<particle.size();i++){
    std::string pname=particle[i].Name() + ".";
    int pidx=2;
    //check if the tree already has a particle with this name(as is the case
    //after the heavy ion fragment proton decays back to the beam nucleus).
    while(tree->GetBranch(pname.c_str())){
      pname=Form("%s%d.",particle[i].Name().c_str(),pidx);
      pidx++;
    }
    tree->Branch(pname.c_str(),"RN_Particle",&particle[i]);
   
  }

  for(unsigned int i=0;i<neut.size();i++){
    tree->Branch(Form("%s.",neut[i].Name().c_str()),"RN_NeutDetector",&neut[i]);
  }
  


  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",4096,1,128);
  
  hE_n=new TH2D("hE_n","hE_n",17,0,16,512,0,5);
  TOF_fit=new TF1("tof_fit","gaus",1,128);
  hQ=new TH1D("hQ","hQ",512,-4,4);
  Q_fit=new TF1("Q_fit","gaus",-4,4);
  for(unsigned int i=0;i<neut.size();i++){
    simfile->mkdir(Form("neut%d/tof",i));
    simfile->cd(Form("neut%d/tof",i));
    htof_n[i]=new TH1D(Form("htof_n%d",i),Form("htof_n%d",i),512,1,128);  
    TOF_fit_n[i]=new TF1(Form("tof_fit_n%d",i),"gaus",1,128);
    simfile->mkdir(Form("neut%d/Q",i));
    simfile->cd(Form("neut%d/Q",i));
    hQ_n[i]=new TH1D(Form("hQ_n%d",i),Form("hQ_n%d",i),512,-4,4);
    Q_fit_n[i]=new TF1(Form("Q_fit_n%d",i),"gaus",-4,4);
  }
  simfile->cd();
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hpos=new TH2D("hpos","hpos",64,-256,256,64,-256,256);
  hpos_in=new TH2D("hpos_in","hpos_in",64,-256,256,64,-256,256);


}


  void RN_Sim::Loop(Long64_t evnum,std::string options){
    initHists();
    SetCalibrations();
    Long64_t evcount=0;
    while(evcount<evnum){
      if(particle.size()==1){
	if(GenerateSingleParticleEvent(evcount))
	  evcount++;
	else continue;
    }
      else if(particle.size()>1){
	if(GenerateEvents(evcount,options))
	  evcount++;
	else
	  continue;
    }
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
    particlegun->Shoot(particle[0].LV); //shoot a particle defined by particle gun into the LV of particle[0]
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if((*it).inDet(particle[0].LV.Vect())){
	(*it).NeutIn(particle[0].LV,n_tof,E_deposited);  
	return 1;
      }
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      if((*it).inDet(particle[0].LV.Vect())){
	double e=particle[0].LV.E()-particle[0].LV.M();
	double t=0;
	(*it).front.InsertHit(e,t,0);
	return 1;
      }
    }

    return 0;
  }
  

int RN_Sim::GenerateEvents(Long64_t evnum,std::string options=""){
  Reset();

  double beam_energy = beam_e - myRnd.Rndm() * beam_eloss;
  
  RN_SimEvent evt1(beam_energy,particle[0].mass,particle[1].mass,particle[2].mass,particle[3].mass);
  RN_SimEvent evt2(particle[3].mass,particle[4].mass,particle[5].mass);
  
  double theta=M_PI*myRnd.Rndm();
  n_cm=theta;
  double phi = 2.*M_PI*myRnd.Rndm();
  if (myRnd.Rndm()>nDWBA.GetAD(180.- (theta / M_PI * 180.)))
    return 0;
  TVector3 nv; nv.SetMagThetaPhi(1.,theta,phi);//neutron 
  
  if(!evt1.radiate_in_CM(nv,hi_ex_set))
    return 0;
  
  
  theta = acos(-1. + 2.*myRnd.Rndm());
  phi = 2.*M_PI*myRnd.Rndm();
  TVector3 pv; pv.SetMagThetaPhi(1.,theta,phi);
  //q1set=-5.518,q2set=3.293,q3set=cancelsoutq1q2
  if(!evt2.radiate_in_CM(evt1.getLVhi(),pv,d_ex_set)){
    if(options=="proton")
      return 0;
  }
  particle[1].LV.SetPxPyPzE(0.0,0.0,0.0,particle[1].mass);
  particle[0].LV = evt1.getLVin() - particle[1].LV;
  particle[2].LV = evt1.getLVrad();
  particle[3].LV = evt1.getLVhi();
  particle[4].LV = evt2.getLVrad();
  particle[5].LV = evt2.getLVhi();
   
  for(unsigned int i=0;i<neut.size();i++){
    if(neut[i].inDet(particle[2].LV.Vect())){
      NeutronIn[i]++;
      if(neut[i].NeutIn(particle[2].LV,n_tof,E_deposited)){
	NeutronDetected[i]++;
	for(unsigned int s=0;s<si_.size();s++){
	  if(si_[s].inDet(particle[4].LV.Vect())){
	    ProtonIn_NeutDet[s]++;
	  }
	}
      }
    }
  }

  for(unsigned int s=0;s<si_.size();s++){
    if(si_[s].inDet(particle[4].LV.Vect())){
      ProtonIn[s]++;
      double e=particle[4].LV.E()-particle[4].LV.M();
      double t=0;
      si_[s].front.InsertHit(e,t,0);
    }
  }
  
  
  return 1;
}   

double RN_Sim::QValue(const double deltaz/*mm*/, const double tof /*ns*/,double& NKE, double& hiKE){
  double M_N=particle[2].mass;
  double M1=particle[0].mass;
  double M2=particle[3].mass;
  NKE=.5*M_N*(deltaz*deltaz/(tof*tof*90000.0));
  hiKE=(M1/M2)*beam_est+((M_N/M2)*NKE)+((2/M2)*sqrt((beam_est*NKE*M_N*M1)));
  
  return NKE+hiKE-beam_est;

}

void RN_Sim::Reset(){
  E_deposited=0;
  n_tof=0;
   
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
    (*it).Reset();  
  }

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).Reset(); 
  }
  for(RN_ParticleCollectionRef it=particle.begin();it!=particle.end();it++){
    (*it).Reset();
  }
  
}

  
  void RN_Sim::WriteOut(){
    
      hQ->Fit(Q_fit,"","",-4,4);
      hn_tof->Fit(TOF_fit,"","",1,128);
      simlog<<"Reaction :"<<particle[0].Name()<<"("<<particle[1].Name()<<","<<particle[2].Name()<<")"<<particle[3].Name()<<"\n";
      simlog<<"Decay :"<<particle[3].Name()<<"->"<<particle[4].Name() + "+" + particle[5].Name()<<"\n\n";
      
      simlog<<"Beam Energy: "<<global::beam_e<<"\n";
      simlog<<"Beam E_loss(thickness): "<<global::beam_eloss<<"\n";
      simlog<<"Excitation Energy: "<<global::hi_ex_set<<"\n";
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
	simlog<<si_[i].Name()<<"\n";
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
      simlog<<neut[i].Name()<<"\n";
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
