/***********************************************************/
//Class: Si_Analyzer
//
//Author:Sean Kuvin
//
//S2 Analyzer class to be inserted in the analyzer list for 
//sorting S1 and S2 type detectors. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Silicon detector 
//parameters on their own. 
/***********************************************************/

#ifndef _Si_ANALYZER_CXX
#define _Si_ANALYZER_CXX

#include "Si_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace silicon{
  TCutG* prots1;
  TCutG* prots2;
  TCutG* ptheta_cut;
  TCutG* ptheta2_cut;
  TCutG* alphas;
  TCutG* deuterons;
  TCutG* thetatheta_cut;

  static int gRequireProton(0);
  static int gRequireProton2(0);
  static int gRequireAlpha(0);
  static int gRequireDeuteron(0);
  static int gRequireThetaTheta(0);
  static int gRequirePTheta(0);
  static int gRequirePTheta2(0);
 
  int protcheck(0);
  int prot2check(0);
  int pthetacheck(0);
  int ptheta2check(0);
  int alphacheck(0);
  int thetathetacheck(0);
  int deutcheck(0);

  TH2D *hpede;
  TH2D *hpede_arb_front;  
  TH2D *hpede_arb_back;
  TH2D *hpede_arb;  
  TH2D *hpede_cluster;
  TH2D *hpede_arb_cluster;    
  TH2D *h_ringsA_v_ringsB;
  TH2D *h_segmentsA_v_segmentsB;
  TH2D *hThetaA_vThetaB; 
  TH2D *hPhiA_vPhiB;  
  TH1D *h_si_back[SI_NUM];
  TH1D *h_si_back_a[SI_NUM];
  TH1D *h_si_front[SI_NUM];
  TH1D *h_si_front_a[SI_NUM];
  TH1D *h_theta[SI_NUM];
  TH2D *h_evtheta_arb[SI_NUM];
  TH2D *h_evtheta[SI_NUM];
  TH2D *h_evtheta_protgated[SI_NUM];
  TH2D *h_evtheta_neutgated[SI_NUM];
  TH2D *h_si_x_y[SI_NUM];
  TH2D *h_si_x_y_prot[SI_NUM];
  TH2D *front[SI_NUM][16];
  TH1D *h_si_fmult[SI_NUM];
  TH1D *h_si_bmult[SI_NUM];
  TH1D *h_si_cluster_mult[SI_NUM];
  
  TH1D *h_chlistf[SI_NUM];
  TH1D *h_chlistb[SI_NUM];
  TH1D *h_chlist_cluster_ring[SI_NUM];
  TH1D *h_chlist_cluster_segment[SI_NUM];
  

  TH2D *h_ch_f0vf1[SI_NUM];
  TH2D *h_e_f0vf1[SI_NUM];
 
  TH2D* hsi_EvT[SI_NUM];
  TH2D* rfvt_si[SI_NUM];	  
  TH2D* rfvtrel_si[SI_NUM];  	  
  TH2D* rfvt_prot_si_[SI_NUM];	  
  TH2D* rfvtrel_prot_si[SI_NUM];  
  TH1D* h_t[SI_NUM];


  
  Si_Analyzer::Si_Analyzer()
  {
    
  }
  
  void Si_Analyzer::Reset(){
  
    //silicon telescope gate results
    protcheck = 0;
    prot2check = 0;
    alphacheck = 0;
    pthetacheck = 0;
    ptheta2check = 0;
    thetathetacheck = 0;
    deutcheck = 0;
  }

  
  bool Si_Analyzer::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    fgRootFile->mkdir("Silicon");
    fgRootFile->cd("Silicon");
    gDirectory->mkdir("DE-E");    
    for(unsigned int i=0;i<si_.size();i++){
      fgRootFile->cd("Silicon");
      gDirectory->mkdir(si_[i].GetName());
      gDirectory->cd(si_[i].GetName());
      gDirectory->mkdir("F2B_Ratio");    
      gDirectory->mkdir("EvTheta");      
      gDirectory->mkdir("Timing");
      gDirectory->mkdir("Mult"); 
      gDirectory->cd("Timing");
      gDirectory->mkdir("rftime");
   }
    
    //create histograms

    fgRootFile->cd("Silicon");
    if(si_.size()>1){
      h_ringsA_v_ringsB=new TH2D("h_ringsA_v_ringsB","h_ringsA_v_ringsB;sia_rings;sib_rings",17,0,16,17,0,16);
      h_segmentsA_v_segmentsB = new TH2D("h_segmentsA_v_segmentsB","h_segmentsA_v_segmentsB;sia_segments;sib_segments",17,0,16,17,0,16);    
      hThetaA_vThetaB = new TH2D("hThetaA_vThetaB","hThetaA_vThetaB;ThetaB;ThetaA",180,0,44,180,0,44); 
      hPhiA_vPhiB = new TH2D("hPhiA_v_PhiB","phiA_v_phiB;phiB;phiA",360,-180,180,360,-180,180);      
      fgRootFile->cd("Silicon/DE-E");
      hpede=new TH2D("hpEdE","siPID;E[MeV];dE[MeV]",1024,0,63,1024,0,63);
      hpede_arb=new TH2D("hpEdE_arb","siPID;E[arb];dE[arb]",1024,0,4095,1024,0,4095);
      hpede_arb_front=new TH2D("hpEdE_arb_front","siPID_front;E[arb];dE[arb]",1024,0,4095,1024,0,4095);
      hpede_arb_back=new TH2D("hpEdE_arb_back","siPID_back;E[arb];dE[arb]",1024,0,4095,1024,0,4095);
      hpede_cluster=new TH2D("hpEdE_cluster","siPID;E[MeV];dE[MeV]",1024,0,63,1024,0,63);
      hpede_arb_cluster=new TH2D("hpEdE_arb_cluster","siPID;E[arb];dE[arb]",1024,0,4095,1024,0,4095);
    }
    
    
    for(unsigned int i=0;i<si_.size();i++){
      fgRootFile->cd(Form("Silicon/%s",si_[i].GetName()));
      h_si_back_a[i]=new TH1D(Form("h_%s_back_a",si_[i].GetName()),Form("%s_e_before;E",si_[i].GetName()),4096,-20,4095);
      h_si_back[i]=new TH1D(Form("h_%s_back",si_[i].GetName()),Form("%s_e_after;E[MeV]",si_[i].GetName()),512,-1,16);
      h_si_front_a[i]=new TH1D(Form("h_%s_front_a",si_[i].GetName()),Form("%s_e_before;E",si_[i].GetName()),4096,-20,4095);
      h_si_front[i]=new TH1D(Form("h_%s_front",si_[i].GetName()),Form("%s_e_after;E[MeV]",si_[i].GetName()),512,-1,16);
      fgRootFile->cd(Form("Silicon/%s/Mult",si_[i].GetName()));
      h_si_fmult[i]= new TH1D(Form("h_si_fmult_%s",si_[i].GetName()),Form("h_si_fmult_%s;fmult",si_[i].GetName()),32,0,31);
      h_si_bmult[i]= new TH1D(Form("h_si_bmult_%s",si_[i].GetName()),Form("h_si_bmult_%s;bmult",si_[i].GetName()),32,0,31);
      h_si_cluster_mult[i] = new TH1D(Form("h_si_%s_cluster_mult",si_[i].GetName()),Form("h_si_%s_cluster_mult;mult",si_[i].GetName()),32,0,31);
      h_chlistf[i]=new TH1D(Form("h_chlistf_%s",si_[i].GetName()),Form("Chlistf_%s;Ch",si_[i].GetName()),20,-1,18);
      h_chlistb[i]=new TH1D(Form("h_chlistb_%s",si_[i].GetName()),Form("Chlistb_%s;Ch",si_[i].GetName()),20,-1,18);
      h_chlist_cluster_ring[i]=new TH1D(Form("h_chlist_cluster_ring_%s",si_[i].GetName()),Form("Chlist_cluster_ring_%s;Ch",si_[i].GetName()),20,-1,18);
      h_chlist_cluster_segment[i]=new TH1D(Form("h_chlist_cluster_segment_%s",si_[i].GetName()),Form("Chlist_cluster_segment_%s;Ch",si_[i].GetName()),20,-1,18);
      fgRootFile->cd(Form("Silicon/%s/F2B_Ratio",si_[i].GetName()));
      h_ch_f0vf1[i]=new TH2D(Form("h_ch_f0vf1_%s",si_[i].GetName()),Form("h_ch_f0vf1_%s;ch_f0;ch_f1",si_[i].GetName()),17,0,16,17,0,16);
      h_e_f0vf1[i]=new TH2D(Form("h_e_f0vf1_%s",si_[i].GetName()),Form("h_e_f0vf1_%s;e_f0;e_f1",si_[i].GetName()),128,0,16,128,0,16);
      for(int j=0;j<16;j++){
	front[i][j]=new TH2D(Form("%s_fc%d_corr",si_[i].GetName(),j),Form("%s_fc%d_corr;channel;ratio",si_[i].GetName(),j),17,0,16,512,0,2);
      }
      fgRootFile->cd(Form("Silicon/%s/EvTheta",si_[i].GetName()));
      h_theta[i] = new TH1D(Form("%s_theta",si_[i].GetName()),Form("%s_theta;theta",si_[i].GetName()),180,0,45);
      h_evtheta_arb[i]=new TH2D(Form("h_evtheta_arb[%s]",si_[i].GetName()),Form("h_evtheta[%s];Ch;E",si_[i].GetName()),17,0,16,1024,0,4095);   
      h_evtheta[i]=new 
TH2D(Form("h_evtheta[%s]",si_[i].GetName()),Form("h_evtheta[%s];Theta;E",si_[i].GetName()),256,0,42,128,0,32);
      h_evtheta_protgated[i]=new TH2D(Form("h_evtheta_prot[%s]",si_[i].GetName()),Form("h_evtheta_prot[%s];Theta;E",si_[i].GetName()),256,0,42,128,0,32);
      h_si_x_y[i]=new TH2D(Form("h_si_x_y[%s]",si_[i].GetName()),Form("h_si_x_y[%s];X;Y",si_[i].GetName()),512,-100,100,512,-100,100);
      h_si_x_y_prot[i]=new TH2D(Form("h_si_x_y_prot[%s]",si_[i].GetName()),Form("h_si_x_y_prot[%s];X;Y",si_[i].GetName()),512,-100,100,512,-100,100);

      fgRootFile->cd(Form("Silicon/%s/Timing",si_[i].GetName()));
      h_t[i]=new TH1D(Form("h_%s_t",si_[i].GetName()),Form("%s_t;time",si_[i].GetName()),1024,0,4096);
      hsi_EvT[i]=new TH2D(Form("h%s_evt",si_[i].GetName()),Form("h%s_evt;T;E",si_[i].GetName()),1024,0,4095,512,0,4095);    
    
    fgRootFile->cd(Form("Silicon/%s/Timing/rftime",si_[i].GetName()));
    rfvt_si[i] = new TH2D(Form("rfvt_%s",si_[i].GetName()),Form("rfvt_%s;rftime;s1_t",si_[i].GetName()),1024,256,2304,1024,0,4096);;	  
    rfvtrel_si[i] =new TH2D(Form("rfvtrel_%s",si_[i].GetName()),Form("rfvtrel_%s;rftime;trelRF",si_[i].GetName()),1024,256,2304,1024,0,4096);  	
    rfvt_prot_si_[i]=new TH2D(Form("rfvt_prot_%s",si_[i].GetName()),Form("rfvt_prot_%s;rftime;s1_t",si_[i].GetName()),1024,256,2304,1024,0,4096); 
    rfvtrel_prot_si[i]=new TH2D(Form("rfvtrel_prot_%s",si_[i].GetName()),Form("rfvtrel_prot_%s;rftime;trelRF",si_[i].GetName()),1024,256,2304,1024,0,4096);  
    }
    return 1;
  }  
  
  

  bool Si_Analyzer::Process(){
    
    //prot_E and prot_dE require cluster reconstruction
    if(silicon::prots1)
      protcheck=silicon::prots1->IsInside(si_array.E_AB(),si_array.E_A());
    
    //prots2 gates on a front hit in si_a and si_b
    if(silicon::prots2)
      prot2check=silicon::prots2->IsInside(si_[0].front.E()+si_[1].front.E(),si_[0].front.E());
    
    if(silicon::alphas)
      alphacheck=silicon::alphas->IsInside(si_array.E_AB(),si_array.E_A());
    
    if(silicon::deuterons)
      deutcheck=silicon::deuterons->IsInside(si_array.E_AB(),si_array.E_A());
    
    if(silicon::thetatheta_cut)
      thetathetacheck= silicon::thetatheta_cut->IsInside(si_array.Theta_A()*RadToDeg(),si_array.Theta_B()*RadToDeg());
    
    if(silicon::ptheta_cut)
      pthetacheck = silicon::ptheta_cut->IsInside(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    
    if(silicon::ptheta2_cut)
      ptheta2check = silicon::ptheta2_cut->IsInside(si_[1].front.Ch(),si_[0].front.E()+si_[1].front.E());
    

    //if proton gate is not passed return 0 (abort analyzers)
    if(gRequireProton && !protcheck){
      return 0;
    }

    //if proton gate is not passed return 0 (abort analyzers)
    if(gRequireProton2 && !prot2check){
      return 0;
    }

    //if alpha gate is not passed return 0 (abort analyzers)
    if(gRequireAlpha && !alphacheck){
      return 0;
    }

    if(gRequireDeuteron && !deutcheck){
      return 0;
    }

  
    if(gRequireThetaTheta && !thetathetacheck){
      return 0;
    }

    if(gRequirePTheta && !pthetacheck){
      return 0;
    }

    if(gRequirePTheta2 && !ptheta2check){
      return 0;
    }
    /*
    if(!(si_[1].back.TRaw()>0))
      return 0;
    */

    return 1;
  }
  bool Si_Analyzer::ProcessFill(){
    Double32_t Prot_E_Arb(0) ;
    
    if(si_.size()>1){
      Prot_E_Arb= si_[0].front.E() + si_[1].front.E();
      h_ringsA_v_ringsB->Fill(si_[0].front.Ch(),si_[1].front.Ch());
      h_segmentsA_v_segmentsB->Fill(si_[0].back.Ch(),si_[1].back.Ch());
      hThetaA_vThetaB->Fill(si_array.Theta_A()*RadToDeg(),si_array.Theta_B()*RadToDeg());
      hPhiA_vPhiB->Fill(si_array.Phi_A()*RadToDeg(),si_array.Phi_B()*RadToDeg());
      hpede->Fill(si_array.E_AB(),si_array.E_A());
      hpede_arb_front->Fill(si_[0].front.E()+si_[1].front.E(),si_[0].front.E());
      hpede_arb_back->Fill(si_[0].back.E()+si_[1].back.E(),si_[0].back.E());
      hpede_arb->Fill(si_[0].front.E()+si_[1].back.E(),si_[0].front.E());
      hpede_cluster->Fill(si_array.E_AB(),si_array.E_A());
      hpede_arb_cluster->Fill(si_array.E_AB(),si_array.E_A());
    }
    

    for(unsigned int i=0;i<si_.size();i++){
      for(unsigned int j=0;j<si_[i].back.Mult();j++){
	h_si_back_a[i]->Fill(si_[i].back.E(j));
	h_si_back[i]->Fill(si_[i].Back_E(j));
      }
      for(unsigned int j=0;j<si_[i].front.Mult();j++){
	h_si_front_a[i]->Fill(si_[i].front.E(j));
	h_si_front[i]->Fill(si_[i].Front_E(j));
      }
      
      h_si_fmult[i]->Fill(si_[i].front.Mult());
      h_si_bmult[i]->Fill(si_[i].back.Mult());
      h_si_cluster_mult[i]->Fill(si_cluster_[i].Mult());
      h_chlistf[i]->Fill(si_[i].front.Ch());
      h_chlistb[i]->Fill(si_[i].back.Ch());
      h_chlist_cluster_ring[i]->Fill(si_cluster_[i].ChRaw(0));
      h_chlist_cluster_segment[i]->Fill(si_cluster_[i].fChlist_b[0]);
      h_ch_f0vf1[i]->Fill(si_[i].front.Ch(0),si_[i].front.Ch(1));
      h_e_f0vf1[i]->Fill(si_[i].Front_E(0),si_[i].Front_E(1));
      
      int idx=(int)si_[i].front.ChRaw(0);
      if(idx>=0){
	front[i][idx]->Fill(si_[i].back.ChRaw(0),(si_[i].Back_E(0)/si_[i].Front_E(0))); 
      }
      h_theta[i]->Fill(si_cluster_[i].Theta()*180/TMath::Pi());
      h_evtheta_arb[i]->Fill(si_[i].front.Ch(),Prot_E_Arb);
      h_evtheta[i]->Fill(si_cluster_[i].Theta()*180/TMath::Pi(),si_array.E_AB());
      h_si_x_y[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
      h_t[i]->Fill(si_[i].Back_T());
      hsi_EvT[i]->Fill(si_[i].back.T(),si_[i].back.E());    
      rfvt_si[i]->Fill(rftime.TRaw(),si_[i].Back_T(0));
      rfvtrel_si[i]->Fill(rftime.TRaw(),si_[i].Back_T(0)); 
      
     if(protcheck){
	h_evtheta_protgated[i]->Fill(si_cluster_[i].Theta()*180/TMath::Pi(),si_array.E_AB());
	h_si_x_y_prot[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
	rfvt_prot_si_[i]->Fill(rftime.TRaw(),si_[i].Back_T(0));
	rfvtrel_prot_si[i]->Fill(rftime.TRaw(),rftime.TRaw() - si_[i].Back_T(0));
     }
    }
    
    return 1;
    
  }


  bool Si_Analyzer::Terminate(){
    return 1;
  }

  bool Si_Analyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    
    return 1;
  }
  
  void Si_Analyzer::Clear(){
    
  }
  
  
  
  void RequireProton(int i){
    gRequireProton = i;
  }
 
  void RequireProton2(int i){
    gRequireProton2 = i;
  }

  void RequireAlpha(int i){
    gRequireAlpha = i;
  }

  void RequireDeuteron(int i){
    gRequireDeuteron = i;
  }

  void RequireThetaTheta(int i){
    gRequireThetaTheta = i;
  }

  void RequirePThetaCut(int i){
    gRequirePTheta = i;
  }

  void RequirePTheta2Cut(int i){
    gRequirePTheta2 = i;
  }

  
  void LoadGates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("prots1") && !prots1)
      prots1=new TCutG(*(TCutG*)in.Get("prots1"));
    if(in.Get("prots2") && !prots2)
      prots2=new TCutG(*(TCutG*)in.Get("prots2"));
    if(in.Get("ptheta_cut") && !ptheta_cut)
      ptheta_cut=new TCutG(*(TCutG*)in.Get("ptheta_cut"));
    if(in.Get("ptheta2_cut") && !ptheta2_cut)
      ptheta2_cut=new TCutG(*(TCutG*)in.Get("ptheta2_cut"));
    if(in.Get("alphas") && !alphas)
      alphas=new TCutG(*(TCutG*)in.Get("alphas"));   
    if(in.Get("deuteron") && !deuterons)
      deuterons=new TCutG(*(TCutG*)in.Get("deuteron")); 
    if(in.Get("thetatheta_cut") && !thetatheta_cut)
      thetatheta_cut=new TCutG(*(TCutG*)in.Get("thetatheta_cut"));
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
