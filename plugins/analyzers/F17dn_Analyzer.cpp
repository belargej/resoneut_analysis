/****************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "../../include/RN_Root.hpp"
#include "F17dn_Analyzer.hpp"


namespace _F17{
  TH2D *hneut_trel_v_ic_trel;
  TH2D *hneut_trel_v_sia_trel;
  TH2D *hneut_trel_v_nai_trel;
  TH2D *hneut_nai_trel_v_neut_t;
  TH2D *hsia_neut_trel_v_neut_t;
  TH2D *hproton_ede_ntime;
  TH2D *hn1QvT;
  TH1D *hneut_time;
  TH1D *hneut_nai_trel;
  TH2D *hnQv_neut_nai_trel;
  

F17dn_Analyzer::F17dn_Analyzer()
{
  
}


void F17dn_Analyzer::ResetGlobals(){

}

bool F17dn_Analyzer::Begin(){

  rootfile->mkdir("F17_dn_Analysis");
  rootfile->mkdir("F17_dn_Analysis/ede");
  rootfile->mkdir("F17_dn_Analysis/QvT");
  rootfile->cd("F17_dn_Analysis");
  hneut_trel_v_ic_trel = new TH2D("neut_rf_trel_v_ic_rf_trel","neut_rf_trel_v_ic_rf_trel;neut_trel[ns];ic_trel[ns]",1024,-1023.,1023.,1024,-1023.,1023.);
  hneut_trel_v_sia_trel = new TH2D("neut_rf_trel_v_sia_rf_trel","neut_rf_trel_v_sia_rf_trel;neut_trel[ns];sia_trel[ns]",1024,-1023.,1023.,1024,-1023.,1023.);
  hneut_nai_trel_v_neut_t = new TH2D("neut_nai_trel_v_neut_t","neut_nai_trel_v_neut_t;neut_nai_trel[ns];neut_t[ns]",1024,-1023.,1023.,1024,-1023.,1023.);
  hneut_trel_v_nai_trel = new TH2D("neut_rf_trel_v_nai_rf_trel","neut_rf_trel_v_nai_rf_trel;neut_trel[ns];nai_trel[ns]",1024,-1023.,1023.,1024,-1023.,1023.);
  
  hnQv_neut_nai_trel= new TH2D("Qv_neut_nai_trel","Qv_neut_nai_trel;neut_nai_trel[ns];Q",2048,-200.,200.,1024,0.,2047.);
 
  hsia_neut_trel_v_neut_t = new TH2D("sia_neut_trel_v_neut_t","sia_neut_trel_v_neut_t;sia_neut_trel[ns];neut_t[ns]",1024,-1023.,1023.,1024,-2049.,2049.);
  



  hneut_time=new TH1D("neut_time","neut_time;nt[ns]",1024,0,1023);
  hneut_nai_trel=new TH1D("neut_nai_trel","neut_nai_trel;neut_nai_trel[ns]",1024,-1023,1023);
  rootfile->cd("F17_dn_Analysis/ede");
  hproton_ede_ntime=new TH2D("proton_EdE_ntime","siPID;E[MeV];dE[MeV]",1024,0.,32.,1024,0.,32.);
  
  rootfile->cd("QvT");
  hn1QvT = new TH2D("n1_QvT","n1_QvT;T;Q",1024,0.,1023.,1024,0.,1023.);


  return 1;
 
}

bool F17dn_Analyzer::Process(){
  /*  if(Narray.fT_first>60&&Narray.fT_first<150)
    return 1;
  else return 0;
  */
  if(nai_array.fE[0]<500)
    return 0;


  return 1;
}

bool F17dn_Analyzer::ProcessFill(){
  if(Narray.fT_mult>0 && ic.T()>0)
    hneut_trel_v_ic_trel->Fill(coinc::neut_rf_trel,coinc::ic_rf_trel);

  if(Narray.fT_mult>0 && si_cluster_[0].fT[0]>0)
    hneut_trel_v_sia_trel->Fill(coinc::neut_rf_trel,coinc::sia_rf_trel);

  if(Narray.fT_mult>0){
    hsia_neut_trel_v_neut_t->Fill(coinc::sia_neut_trel,Narray.fT_first);
    if(nai_array.TMult()){    
      hneut_nai_trel_v_neut_t->Fill(coinc::neut_nai_trel,Narray.fT_first);
      
      hnQv_neut_nai_trel->Fill(coinc::neut_nai_trel,Narray.fQ_long[0]);
      hneut_trel_v_nai_trel->Fill(coinc::neut_rf_trel,coinc::nai_rf_trel);
    }
  }
  
  hn1QvT->Fill(neut[0].fT_Q,neut[0].fQ_long);
  if(Narray.fT_mult>0){
    hneut_time->Fill(Narray.fT_first);
    if(nai_array.TMult())
      hneut_nai_trel->Fill(coinc::neut_nai_trel);
  }
  if(si_cluster_[1].fMult>0 && si_cluster_[0].fMult>0 && Narray.fT_mult>0)
    hproton_ede_ntime->Fill(silicon::prot_E,silicon::prot_dE);

  return 1;
}
  


bool F17dn_Analyzer::Terminate(){

  return 1;
  
}

bool F17dn_Analyzer::TerminateIfLast(){
  rootfile->Write();
  rootfile->Close();

  return 1;
  
}


void Load_F17_dn_Gates(){


}
}
