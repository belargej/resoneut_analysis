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
using unpacker::TDC1;

namespace silicon{
  TCutG* prots1;
  TCutG* prots2;
  TCutG* ptheta_cut;
  TCutG* alphas;
  TCutG* deuterons;
  TCutG* thetatheta_cut;
  int _require_proton(0);
  int _require_alpha(0);
  int _require_deuteron(0);
  int _require_thetatheta(0);
  int _require_ptheta(0);
  double z_min(0/*mm*/); //relative to the E detector
  double z_max(1000/*mm*/);

  int protcheck(0);
  int prot2check(0);
  int pthetacheck(0);
  int alphacheck(0);
  int thetathetacheck(0);
  int deut_check(0);
  TH2D *hpede;  
  TH1D *h_si_back[2];
  TH1D *h_si_back_a[2];
  TH1D *h_si_front[2];
  TH1D *h_si_front_a[2];
  TH2D *h_evtheta[2];
  TH2D *h_evtheta_protgated[2];
  TH2D *h_evtheta_neutgated[2];
  TH2D *h_e_v_relangle;
  TH2D *h_e_v_relangle_phicut;
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

  TH2D *h_chS1_chS2_rings;
  TH2D *h_chS1_chS2_segments;

  TH2D *h_ch_f0vf1[2];
  TH2D *h_e_f0vf1[2];
  TH2D *h_ch_f0vQuadrant[2];
  TH2D *h_e_f0vQuadrant[2];


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

  TH1D *h_target_z_est[2];
  TH2D *h_zest_v_phi[2];
  TH2D *h_zest_v_theta[2];
  TH2D *h_zest_v_relangle;
  TH2D *h_theta_v_phi[2];
  TH2D *h_relangle_v_phi[2];
  TH2D *h_relangle_v_theta[2];
  TH2D *h_thetadiff_v_phi;
  TH2D *h_phi_v_phi;

  S2_Analyzer::S2_Analyzer():ind_(0)
  {
    
  }

  void S2_Analyzer::ResetGlobals(){
    
    //silicon telescope parameters
    prot_E = 0;
    prot_dE = 0;
    prot_theta = 0;  
    rel_angle = 0;
    rel_z = 0;
    rel_transverse = 0;
    target_z[0] = 0;
    target_z[1] = 0;

    //silicon telescope gate results
    protcheck = 0;
    prot2check = 0;
    alphacheck = 0;
    pthetacheck = 0;
    thetathetacheck = 0;
    deut_check = 0;
  }

  
  bool S2_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    rootfile->mkdir("Silicon/f2b_ratio");
    rootfile->mkdir("Silicon/f2b_ratio/charge_sharing");
    rootfile->mkdir("Silicon/f2b_ratio/S1");
    rootfile->mkdir("Silicon/f2b_ratio/S2");
    rootfile->mkdir("Silicon/ede");
    rootfile->mkdir("Silicon/evtheta");
    rootfile->mkdir("Silicon/Timing");
    rootfile->mkdir("mult/Silicon");
    rootfile->mkdir("Silicon/Chlist");
    rootfile->mkdir("Silicon/Theta");
   
    rootfile->cd("Silicon");
    h_si_back_a[0]=new TH1D("h_si_1_back_a","si_1_e_before;E",4096,-20,4095);
    h_si_back_a[1]=new TH1D("h_si_2_back_a","si_2_e_before;E[MeV]",4096,-20,4095);
    h_si_back[0]=new TH1D("h_si_1_back","si_1_e_after;E[MeV]",512,-1,16);
    h_si_back[1]=new TH1D("h_si_2_back","si_2_e_after;E[MeV]",512,-1,16);
    h_si_front_a[0]=new TH1D("h_si_1_front_a","si_1_e_before;E",4096,-20,4095);
    h_si_front_a[1]=new TH1D("h_si_2_front_a","si_2_e_before;E[MeV]",4096,-20,4095);
    h_si_front[0]=new TH1D("h_si_1_front","si_1_e_after;E[MeV]",512,-1,16);
    h_si_front[1]=new TH1D("h_si_2_front","si_2_e_after;E[MeV]",512,-1,16);
    rootfile->cd("Silicon/Chlist");
    h_chlistf[0]=new TH1D("h_chlistf_1","Chlistf_S1;Ch",20,-1,18);
    h_chlistf[1]=new TH1D("h_chlistf_2","Chlistf_S2;Ch",20,-1,18);
    h_chlistb[0]=new TH1D("h_chlistb_1","Chlistb_S1;Ch",20,-1,18);
    h_chlistb[1]=new TH1D("h_chlistb_2","Chlistb_S2;Ch",20,-1,18);
    h_chlist_cluster_ring[0]=new TH1D("h_chlist_cluster_ring_S1","Chlist_cluster_ring_S1;Ch",20,-1,18);
    h_chlist_cluster_ring[1]=new TH1D("h_chlist_cluster_ring_S2","Chlist_cluster_ring_S2;Ch",20,-1,18);
    h_chlist_cluster_segment[0]=new TH1D("h_chlist_cluster_segment_S1","Chlist_cluster_segment_S1;Ch",20,-1,18);
    h_chlist_cluster_segment[1]=new TH1D("h_chlist_cluster_segment_S2","Chlist_cluster_segment_S2;Ch",20,-1,18);


    h_chS1_chS2_rings=new TH2D("h_chS1_chS2_rings","h_chS1_chS2_rings;s1rings;s2rings",17,0,16,17,0,16);
    h_chS1_chS2_segments = new TH2D("h_chS1_chS2_segments","h_chS1_chS2_segments;s1segments;s2segments",17,0,16,17,0,16);

    rootfile->cd("Silicon/f2b_ratio");
    for(int i=0;i<16;i++){
      rootfile->cd("Silicon/f2b_ratio/S1");
      front[0][i]=new TH2D(Form("s1_fc%d_corr",i),Form("s1_fc%d_corr;channel;ratio",i),17,0,16,512,0,2);
      rootfile->cd("Silicon/f2b_ratio/S2");
      front[1][i]=new TH2D(Form("s2_fc%d_corr",i),Form("s2_fc%d_corr;channel;ratio",i),17,0,16,512,0,2);
    }

    rootfile->cd("Silicon/f2b_ratio/charge_sharing");
    for(unsigned int i=0;i<2;i++){
      h_ch_f0vf1[i]=new TH2D(Form("h_ch_f0vf1_s%d",i+1),Form("h_ch_f0vf1_s%d;ch_f0;ch_f1",i+1),17,0,16,17,0,16);
      h_e_f0vf1[i]=new TH2D(Form("h_e_f0vf1_s%d",i+1),Form("h_e_f0vf1_s%d;e_f0;e_f1",i+1),128,0,16,128,0,16);
      h_ch_f0vQuadrant[i]=new TH2D(Form("h_ch_f0vQuadrant_s%d",i+1),Form("h_ch_f0vQuadrant_s%d;ch_f0;Quadrant",i+1),17,0,16,5,0,4);
      h_e_f0vQuadrant[i]=new TH2D(Form("h_e_f0vQuadrant_s%d",i+1),Form("h_e_f0vQuadrant_s%d;e_f0;Quadrant",i+1),128,0,16,5,0,4);
    }
 


    rootfile->cd("Silicon/ede");
    hpede=new TH2D("hpEdE","siPID;E[MeV];dE[MeV]",1024,0,32,1024,0,32);

    rootfile->cd("Silicon/Theta");
    hS1Theta_vS2Theta = new TH2D("hS1Theta_vS2Theta","hS1Theta_vS2Theta;S1Theta;S2Theta",180,0,44,180,0,44); ; 
    hS1Theta_vS2Theta_prot = new TH2D("hS1Theta_vS2Theta_prot","hS1Theta_vS2Theta_prot;S1Theta;S2Theta",180,0,44,180,0,44); 
    h_e_v_relangle = new TH2D("h_e_v_relangle","e_v_relangle;relangle[deg];E[MeV]",256,0,50,128,0,32);
    h_e_v_relangle_phicut = new TH2D("h_e_v_relangle_phicut","e_v_relangle_phicut;relangle[deg];E[MeV]",256,0,50,128,0,32);
    h_target_z_est[0] = new TH1D("h_target_z_est_S1","target_z_est_S1;Z_est",2048,0,512);
    h_target_z_est[1] = new TH1D("h_target_z_est_S2","target_z_est_S2;Z_est",2048,0,512);
    h_zest_v_phi[0]= new TH2D("h_z_est_v_phi_S1","z_est_v_phi_S1;z_est;phi",1024,0,512,180,-180,180);
    h_zest_v_phi[1]= new TH2D("h_z_est_v_phi_S2","z_est_v_phi_S2;z_est;phi",1024,0,512,180,-180,180);
    h_zest_v_theta[0]= new TH2D("h_z_est_v_theta_S1","z_est_v_theta_S1;z_est;theta",1024,0,512,260,0,179);
    h_zest_v_theta[1]= new TH2D("h_z_est_v_theta_S2","z_est_v_theta_S2;z_est;theta",1024,0,512,260,0,179);
    h_theta_v_phi[0]= new TH2D("h_theta_v_phi_sia","theta_v_phi_sia;phi;theta",180,-180,180,128,0,63);
    h_relangle_v_phi[0]= new TH2D("h_relangle_v_phi_sia","relangle_v_phi_sia;phi;relangle",180,-180,180,128,0,63);;
    h_relangle_v_theta[0]= new TH2D("h_relangle_v_theta_sia","relangle_v_theta_sia;theta;relangle",128,0,63,128,0,63);;
    h_theta_v_phi[1]= new TH2D("h_theta_v_phi_sib","theta_v_phi_s1b;phi;theta",180,-180,180,128,0,63);
    h_relangle_v_phi[1]= new TH2D("h_relangle_v_phi_sib","relangle_v_phi_sib;phi;relangle",180,-180,180,128,0,63);;
    h_relangle_v_theta[1]= new TH2D("h_relangle_v_theta_sib","relangle_v_theta_sib;theta;relangle",128,0,63,128,0,63);;
    h_thetadiff_v_phi =new TH2D("h_thetadiff_v_phi","thetadiff_v_phi;theta_diff;phi",256,-10,10,360,-180,180);;
    h_phi_v_phi = new TH2D("h_phi_v_phi","phi_v_phi;phi;phi",360,-180,180,360,-180,180);;



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
    
    if (target_z[0] > z_max || target_z[0] < z_min)
      return 0;

    if(silicon::prots1)
      protcheck=silicon::prots1->IsInside(prot_E,prot_dE);
  
    if(silicon::prots2)
      prot2check=silicon::prots2->IsInside(prot_E,prot_dE);
    
    if(silicon::alphas)
      alphacheck=silicon::alphas->IsInside(prot_E,prot_dE);
   if(silicon::deuterons)
      deut_check=silicon::deuterons->IsInside(prot_E,prot_dE);
    
    if(silicon::thetatheta_cut)
      thetathetacheck= silicon::thetatheta_cut->IsInside(si_cluster_[0].fPos[0].Theta()*180/3.14,si_cluster_[1].fPos[0].Theta()*180/3.14);
    
    if(silicon::ptheta_cut)
      pthetacheck = silicon::ptheta_cut->IsInside(prot_theta,prot_E);

    //if proton gate is not passed return 0 (abort analyzers)
    if(_require_proton && !protcheck){
      return 0;
    }

    //if alpha gate is not passed return 0 (abort analyzers)
    if(_require_alpha && !alphacheck){
      return 0;
    }

    if(_require_deuteron && !deut_check){
      return 0;
    }

  
    if(_require_thetatheta && !thetathetacheck){
      return 0;
    }

    if(_require_ptheta && !pthetacheck){
      return 0;
    }


    return 1;
  }
  bool S2_Analyzer::ProcessFill(){

    if(rel_angle && prot_E){
      h_e_v_relangle->Fill(rel_angle,prot_E);	
      h_target_z_est[0]->Fill(target_z[0]);
      h_target_z_est[1]->Fill(target_z[1]);
     

      //require Phi correlation between detectors to be within 10%
      if(TMath::Abs(si_cluster_[0].fPos[0].Phi()-si_cluster_[1].fPos[0].Phi())/si_cluster_[1].fPos[0].Phi() <0.1)
	h_e_v_relangle_phicut->Fill(rel_angle,prot_E);
    }
   
    h_thetadiff_v_phi->Fill((si_cluster_[0].Theta()-si_cluster_[1].Theta())*180/3.14,si_cluster_[0].Phi()*180/3.14);
    h_phi_v_phi->Fill(si_cluster_[0].Phi()*180/3.14,si_cluster_[1].Phi()*180/3.14);

    h_chS1_chS2_rings->Fill(si_[0].front.Ch(),si_[1].front.Ch());
    h_chS1_chS2_segments->Fill(si_[0].back.Ch(),si_[1].back.Ch()); 
 
    h_s1_t->Fill(si_[0].Back_T(0));
    if(!unpacker::TDC1[4]>0)
      h_s1_t_noticds->Fill(si_[0].Back_T(0));
    
    for(int i=0;i<2;i++){
      int idx=(int)si_[i].front.fChlist[0];

      for(unsigned int j=0;j<si_[i].front.fMult;j++){
	h_si_front[i]->Fill(si_[i].Front_E(j));
	h_si_front_a[i]->Fill(si_[i].Front_E(j));
      }	   
      for(unsigned int j=0;j<si_[i].back.fMult;j++){
	h_si_back[i]->Fill(si_[i].Back_E(j));
	h_si_back_a[i]->Fill(si_[i].Back_E(j));
      }	    
      if(si_[i].front.fMult>0&&si_[i].back.fMult>0){
	front[i][idx]->Fill(si_[i].back.fChlist[0],(si_[i].Back_E(0)/si_[i].Front_E(0)));
	h_chlistf[i]->Fill(si_[i].front.Ch());
	h_chlistb[i]->Fill(si_[i].back.Ch());
	
	h_ch_f0vQuadrant[i]->Fill(si_[i].front.Ch(),si_[i].Quadrant());
	h_e_f0vQuadrant[i]->Fill(si_[i].Front_E(),si_[i].Quadrant());
	
	if(si_[i].front.fMult>1){
	  h_ch_f0vf1[i]->Fill(si_[i].front.Ch(),si_[i].front.Ch(1));
	  if(si_[i].front.Ch()==si_[i].front.Ch(1)+1 || si_[i].front.Ch(1)-1)
	    h_e_f0vf1[i]->Fill(si_[i].Front_E(),si_[i].Front_E(1));
	  
	}	 
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

	h_si_fmult[i]->Fill(si_[i].front.fMult);
	h_si_bmult[i]->Fill(si_[i].back.fMult);
	h_si_cluster_mult[i]->Fill(si_cluster_[i].fMult);
      }
    
    }
    if(si_cluster_[1].fMult>0&&si_cluster_[0].fMult>0){
      hS1Theta_vS2Theta->Fill(si_cluster_[0].fPos[0].Theta()*180/3.14,si_cluster_[1].fPos[0].Theta()*180/3.14);
      hpede->Fill(prot_E,prot_dE);
      h_zest_v_phi[0]->Fill(target_z[0],si_cluster_[0].fPos[0].Phi()*180/3.14);
      h_zest_v_phi[1]->Fill(target_z[1],si_cluster_[1].fPos[0].Phi()*180/3.14);
      h_zest_v_theta[0]->Fill(target_z[0],si_cluster_[0].fPos[0].Theta()*180/3.14);
      h_zest_v_theta[1]->Fill(target_z[1],si_cluster_[1].fPos[0].Theta()*180/3.14);
      h_theta_v_phi[0]->Fill(si_cluster_[0].fPos[0].Phi()*180/3.14,si_cluster_[0].fPos[0].Theta()*180/3.14);
      h_relangle_v_phi[0]->Fill(si_cluster_[0].fPos[0].Phi()*180/3.14,rel_angle);
      h_relangle_v_theta[0]->Fill(si_cluster_[0].fPos[0].Theta()*180/3.14,rel_angle);
      h_theta_v_phi[1]->Fill(si_cluster_[1].fPos[0].Phi()*180/3.14,si_cluster_[0].fPos[0].Theta()*180/3.14);
      h_relangle_v_phi[1]->Fill(si_cluster_[1].fPos[0].Phi()*180/3.14,rel_angle);
      h_relangle_v_theta[1]->Fill(si_cluster_[1].fPos[0].Theta()*180/3.14,rel_angle);
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
  
  
  
  void RequireProton(){
    _require_proton=1;
  }

  void RequireAlpha(){
    _require_alpha=1;
  }
  void RequireDeuteron(){
    _require_deuteron=1;
  }


  void RequireThetaTheta(){
    _require_thetatheta=1;
  }

  void RequirePThetaCut(){
    _require_ptheta=1;
  }

  void SetZMinMax(const double & min, const double & max){
    z_min = min;
    z_max = max;
  }


  void LoadGates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("prots1") && !prots1)
      prots1=new TCutG(*in.getCut("prots1"));
    if(in.getCut("prots2") && !prots2)
      prots2=new TCutG(*in.getCut("prots2"));
    if(in.getCut("ptheta_cut") && !ptheta_cut)
      ptheta_cut=new TCutG(*in.getCut("ptheta_cut"));
    if(in.getCut("alphas") && !alphas)
      alphas=new TCutG(*in.getCut("alphas"));   
    if(in.getCut("deuteron") && !deuterons)
      deuterons=new TCutG(*in.getCut("deuteron")); 
    if(in.getCut("thetatheta_cut") && !thetatheta_cut)
      thetatheta_cut=new TCutG(*in.getCut("thetatheta_cut"));
    in.Close();
  }
  
  void ClearGates(){
    if(prots1)delete prots1;
    if(prots2)delete prots2;
    if(deuterons)delete deuterons;
    if(ptheta_cut)delete ptheta_cut;
    if(alphas)delete alphas;
    if(thetatheta_cut)delete thetatheta_cut;


  }
}


#endif
