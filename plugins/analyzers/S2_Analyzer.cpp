/***********************************************************/
//Class: S2_Analyzer
//
//Author:Sean Kuvin
//
//S2 Analyzer class to be inserted in the analyzer list for 
//sorting S1 and S2 type detectors. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Silicon detector 
//parameters on their own. 
/***********************************************************/

#ifndef _S2_ANALYZER_CXX
#define _S2_ANALYZER_CXX

#include "S2_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
namespace si_cal{
  TCutG* prots1;
  TCutG* prots2;
  TCutG* alphas;
  TCutG* thetatheta_cut;
  int _require_proton(0);
  int _require_alpha(0);

  int protcheck(0);
  int prot2check(0);
  int alphacheck(0);

  double prot_E(0);
  double prot_dE(0);
  double prot_theta(0);

  TH2D *hpede;  
  TH1D *h_si_[2];
  TH1D *h_si_a[2];
  TH2D *h_evtheta[2];
  TH2D *h_evtheta_protgated[2];
  TH2D *h_evtheta_neutgated[2];
  TH2D *h_si_x_y[2];
  TH2D *h_si_x_y_prot[2];
  TH2D *front[2][16];
  TH1D *h_si_fmult[2];
  TH1D *h_si_bmult[2];
  TH1D *h_si_cluster_mult[2];

  TH1D *h_chlistf[2];
  TH1D *h_chlistb[2];
  TH1D *h_chlist_cluster_ring[2];
  TH1D *h_chlist_cluster_segment[2];

  TH2D *hS1Theta_vS2Theta; 
  TH2D *hS1Theta_vS2Theta_prot; 

  TH2D* rfvs1_t;
  TH2D *rfvs2_t;
  TH2D *rfvs1_t_rel;
  TH2D *rfvs2_t_rel;
  
  TH2D *rfvs1_t_prot;
  TH2D *rfvs2_t_prot;
  TH2D *rfvs1_t_rel_prot;
  TH2D *rfvs2_t_rel_prot;
  
  TH2D *rfvs1_t_prot2;
  TH2D *rfvs1_t_alpha;
  TH2D *rfvs1_t_rel_prot2;
  TH2D *rfvs1_t_rel_alpha;

  TH1D *h_s1_t;
  TH1D *h_s1_t_noticds;



  S2_Analyzer::S2_Analyzer():ind_(0)
  {
    
  }

  void S2_Analyzer::ResetGlobals(){
    prot_E=0;
    prot_dE=0;
    protcheck=0;
    prot2check=0;
    alphacheck=0;
    prot_theta=0;

  }

  //AutoCalibrate function is used to match the highest energy ring hits with the highest energy segment hits and matching the charge.

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
      GetDetectorEntry(i);
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
      
      idx++;
      
    }
    
    
    
  }
  
  bool S2_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    rootfile->mkdir("Silicon/f2b_ratio");
    rootfile->mkdir("Silicon/f2b_ratio/S1");
    rootfile->mkdir("Silicon/f2b_ratio/S2");
    rootfile->mkdir("Silicon/ede");
    rootfile->mkdir("Silicon/evtheta");
    rootfile->mkdir("Silicon/Timing");
    rootfile->mkdir("mult/Silicon");
    rootfile->mkdir("Silicon/Chlist");
    rootfile->mkdir("Silicon/Theta");
   
    rootfile->cd("Silicon");
    h_si_a[0]=new TH1D("h_si_1a","si_1_e_before;E",4096,0,4095);
    h_si_a[1]=new TH1D("h_si_2a","si_2_e_before;E[MeV]",4096,0,4095);
    h_si_[0]=new TH1D("h_si_1","si_1_e_after;E[MeV]",512,1,16);
    h_si_[1]=new TH1D("h_si_2","si_2_e_after;E[MeV]",512,1,16);
    rootfile->cd("Silicon/Chlist");
    h_chlistf[0]=new TH1D("h_chlistf_1","Chlistf_S1;Ch",20,-1,18);
    h_chlistf[1]=new TH1D("h_chlistf_2","Chlistf_S2;Ch",20,-1,18);
    h_chlistb[0]=new TH1D("h_chlistb_1","Chlistb_S1;Ch",20,-1,18);
    h_chlistb[1]=new TH1D("h_chlistb_2","Chlistb_S2;Ch",20,-1,18);
    h_chlist_cluster_ring[0]=new TH1D("h_chlist_cluster_ring_S1","Chlist_cluster_ring_S1;Ch",20,-1,18);
    h_chlist_cluster_ring[1]=new TH1D("h_chlist_cluster_ring_S2","Chlist_cluster_ring_S2;Ch",20,-1,18);
    h_chlist_cluster_segment[0]=new TH1D("h_chlist_cluster_segment_S1","Chlist_cluster_segment_S1;Ch",20,-1,18);
    h_chlist_cluster_segment[1]=new TH1D("h_chlist_cluster_segment_S2","Chlist_cluster_segment_S2;Ch",20,-1,18);

    rootfile->cd("Silicon/f2b_ratio");
    for(int i=0;i<16;i++){
      rootfile->cd("Silicon/f2b_ratio/S1");
      front[0][i]=new TH2D(Form("s1_fc%d_corr",i),Form("s1_fc%d_corr;channel;ratio",i),17,0,16,512,0,2);
      rootfile->cd("Silicon/f2b_ratio/S2");
      front[1][i]=new TH2D(Form("s2_fc%d_corr",i),Form("s2_fc%d_corr;channel;ratio",i),17,0,16,512,0,2);
    }
    rootfile->cd("Silicon/ede");
    hpede=new TH2D("hpEdE","siPID;E[MeV];dE[MeV]",64,0,20,64,0,6);

    rootfile->cd("Silicon/Theta");
    hS1Theta_vS2Theta = new TH2D("hS1Theta_vS2Theta","hS1Theta_vS2Theta;S1Theta;S2Theta",180,0,179,180,0,179); ; 
    hS1Theta_vS2Theta_prot = new TH2D("hS1Theta_vS2Theta_prot","hS1Theta_vS2Theta_prot;S1Theta;S2Theta",180,0,179,180,0,179); 
   

    
    for(int i=0;i<2;i++){
      rootfile->cd("Silicon/evtheta");
      h_evtheta[i]=new TH2D(Form("h_evtheta[%d]",i+1),Form("h_evtheta[%d];Theta;E",i+1),256,10,42,128,0,32);
      h_evtheta_protgated[i]=new TH2D(Form("h_evtheta_prot[%d]",i+1),Form("h_evtheta_prot[%d];Theta;E",i+1),256,10,42,128,0,32);
      h_si_x_y[i]=new TH2D(Form("h_si_x_y[%d]",i+1),Form("h_si_x_y[%d];X;Y",i+1),512,-100,100,512,-100,100);
      h_si_x_y_prot[i]=new TH2D(Form("h_si_x_y_prot[%d]",i+1),Form("h_si_x_y_prot[%d];X;Y",i+1),512,-100,100,512,-100,100);
      
      
      rootfile->cd("mult/Silicon");
      h_si_fmult[i]= new TH1D(Form("h_si_fmult_%d",i),Form("h_si_fmult_%d;fmult",i),32,0,31);
      h_si_bmult[i]= new TH1D(Form("h_si_bmult_%d",i),Form("h_si_bmult_%d;bmult",i),32,0,31);
      h_si_cluster_mult[i] = new TH1D(Form("h_si_%d_cluster_mult",i),Form("h_si_%d_cluster_mult;mult",i),32,0,31);
    } 
    
    rootfile->cd("Silicon/Timing");
    h_s1_t=new TH1D("h_s1_t","s1_t;time",1024,0,4096);
    h_s1_t_noticds=new TH1D("h_s1_t_noticds","s1_t_noticds;time",1024,0,4096);
    

    rootfile->cd("Silicon/Timing/rftime");
    
    rfvs1_t=new TH2D("rfvs1_t","rfvs1_t;rftime;s1_t",1024,256,2304,1024,0,4096);
    rfvs2_t=new TH2D("rfvs2_t","rfvs2_t;rftime;s2_t",1024,256,2304,1024,0,4096);
    rfvs1_t_rel=new TH2D("rfvs1_t_rel","rfvs1_t_rel;rftime;s1_t_rel",1024,256,2304,1024,-2048,2047);
    rfvs2_t_rel=new TH2D("rfvs2_t_rel","rfvs2_t_rel;rftime;s2_t_rel",1024,256,2304,1024,-2048,2047);
    
    rfvs1_t_prot=new TH2D("rfvs1_t_prot","rfvs1_t_prot;rftime;s1_t",1024,256,2304,1024,0,4096);
    rfvs2_t_prot=new TH2D("rfvs2_t_prot","rfvs2_t_prot;rftime;s2_t",1024,256,2304,1024,0,4096);
    rfvs1_t_rel_prot=new TH2D("rfvs1_t_rel_prot","rfvs1_t_rel_prot;rftime;s1_t_rel",1024,256,2304,1024,-2048,2047);
    rfvs2_t_rel_prot=new TH2D("rfvs2_t_rel_prot","rfvs2_t_rel_prot;rftime;s2_t_rel",1024,256,2304,1024,-2048,2047);
    
    rfvs1_t_prot2=new TH2D("rfvs1_t_prot2","rfvs1_t_prot2;rftime;s1_t",1024,256,2304,1024,0,4096);
    rfvs1_t_rel_prot2=new TH2D("rfvs1_t_rel_prot2","rfvs1_t_rel_prot2;rftime;s1_t_rel",1024,256,2304,1024,-2048,2047);
    rfvs1_t_alpha=new TH2D("rfvs1_t_alpha","rfvs1_t_alpha;rftime;s1_t",1024,256,2304,1024,0,4096);
    rfvs1_t_rel_alpha=new TH2D("rfvs1_t_rel_alpha","rfvs1_t_rel_alpha;rftime;s1_t_rel",1024,256,2304,1024,-2048,2047);
    
    return 1;
  }  
    
    
  

  bool S2_Analyzer::Process(){
    protcheck=0;
    prot_E=0;
    prot_dE=0;
    prot_theta=0;
 
    if(si_cluster_[1].fMult>0&&si_cluster_[0].fMult>0){
      prot_dE=si_cluster_[1].fE[0];
      prot_E=si_cluster_[0].fE[0]+prot_dE;
      hpede->Fill(prot_E,prot_dE);
      //prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
      prot_theta=si_cluster_[0].fPos[0].Theta()*(180/3.14);
   
    }

    if(si_cal::prots1)
      protcheck=si_cal::prots1->IsInside(prot_E,prot_dE);
  
    if(si_cal::prots2)
      prot2check=si_cal::prots2->IsInside(prot_E,prot_dE);
  
    if(si_cal::alphas)
      alphacheck=si_cal::alphas->IsInside(prot_E,prot_dE);

    //if proton gate is not passed return 0 (abort analyzers)
    if(_require_proton && !protcheck){
      return 0;
    }

    //if alpha gate is not passed return 0 (abort analyzers)
    if(_require_alpha && !alphacheck){
      return 0;
    }  
    return 1;
  }
  bool S2_Analyzer::ProcessFill(){
 
    h_s1_t->Fill(si_[0].Back_T(0));
    if(!unpacker::TDC1[4]>0)
      h_s1_t_noticds->Fill(si_[0].Back_T(0));
    
    for(int i=0;i<2;i++){
      int idx=(int)si_[i].front.fChlist[0];	      
      if(si_[i].front.fMult>0&&si_[i].back.fMult>0){
	front[i][idx]->Fill(si_[i].back.fChlist[0],(si_[i].Back_E(0)/si_[i].Front_E(0)));
	h_chlistf[i]->Fill(si_[i].front.Ch());
	h_chlistb[i]->Fill(si_[i].back.Ch());
			 
	if(si_cluster_[i].fMult>0){
	  h_chlist_cluster_ring[i]->Fill(si_cluster_[i].fChlist[0]);
	  h_chlist_cluster_segment[i]->Fill(si_cluster_[i].fChlist_b[0]);
	  h_evtheta[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,prot_E);
	  h_si_x_y[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
	  if(protcheck){
	    h_evtheta_protgated[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,prot_E);
	    h_si_x_y_prot[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
	  }
	}
	h_si_[i]->Fill(si_[i].Front_E(0));
	h_si_a[i]->Fill(si_[i].Front_E(0));
	h_si_fmult[i]->Fill(si_[i].front.fMult);
	h_si_bmult[i]->Fill(si_[i].back.fMult);
	h_si_cluster_mult[i]->Fill(si_cluster_[i].fMult);
      }
    
    }
    if(si_cluster_[1].fMult>0&&si_cluster_[0].fMult>0){
      hS1Theta_vS2Theta->Fill(si_cluster_[0].fPos[0].Theta()*180/3.14,si_cluster_[1].fPos[0].Theta()*180/3.14);
    }
  
    if(si_[0].Back_T(0)>0){
      rfvs1_t->Fill(rftime[0].fT,si_[0].Back_T(0));
      rfvs2_t->Fill(rftime[0].fT,si_[1].Back_T(0));
      rfvs1_t_rel->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
      rfvs2_t_rel->Fill(rftime[0].fT,rftime[0].fT-si_[1].Back_T(0));

      if(protcheck){
	rfvs1_t_prot->Fill(rftime[0].fT,si_[0].Back_T(0));
	rfvs2_t_prot->Fill(rftime[0].fT,si_[1].Back_T(0));
	rfvs1_t_rel_prot->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
	rfvs2_t_rel_prot->Fill(rftime[0].fT,rftime[0].fT-si_[1].Back_T(0));


	hS1Theta_vS2Theta_prot->Fill(si_cluster_[0].fPos[0].Theta()*180/3.14,si_cluster_[1].fPos[0].Theta()*180/3.14);

      }
    
      if(prot2check){
	rfvs1_t_prot2->Fill(rftime[0].fT,si_[0].Back_T(0));
	rfvs1_t_rel_prot2->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
      }
    
    
      if(alphacheck){
	rfvs1_t_alpha->Fill(rftime[0].fT,si_[0].Back_T(0));
	rfvs1_t_rel_alpha->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
      
      }

    
    }
  
    return 1;

  }


  bool S2_Analyzer::Terminate(){
    return 1;
  }

  bool S2_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    
    return 1;
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

  void RequireProton(){
    _require_proton=1;
  }

  void RequireAlpha(){
    _require_alpha=1;
  }


  void LoadGates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("prots1") && !prots1)
      prots1=new TCutG(*in.getCut("prots1"));
    if(in.getCut("prots2") && !prots2)
      prots2=new TCutG(*in.getCut("prots2"));
    if(in.getCut("alphas") && !alphas)
      alphas=new TCutG(*in.getCut("alphas"));   
    if(in.getCut("thetatheta_cut") && !thetatheta_cut)
      thetatheta_cut=new TCutG(*in.getCut("thetatheta_cut"));
    in.Close();
  }
  
  void ClearGates(){
    if(prots1)delete prots1;
    if(prots2)delete prots2;
    if(alphas)delete alphas;
    if(thetatheta_cut)delete thetatheta_cut;


  }
}


#endif
