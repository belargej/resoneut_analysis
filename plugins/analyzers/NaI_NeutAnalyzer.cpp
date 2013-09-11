#include "NaI_NeutAnalyzer.hpp"


namespace psd{

  R__EXTERN TFile *rootfile;
  
  
  //declare Gates
  R__EXTERN TCutG* n0_neuts;
  R__EXTERN TCutG* n1_neuts;
  R__EXTERN TCutG* n2_neuts;
  R__EXTERN TCutG* n3_neuts;
  R__EXTERN TCutG* n4_neuts;
  R__EXTERN TCutG* n5_neuts;
  R__EXTERN TCutG* n6_neuts;
  R__EXTERN TCutG* n7_neuts;
  R__EXTERN TCutG* n8_neuts;
  R__EXTERN TCutG* n9_neuts;
  R__EXTERN TCutG* prots1;
  R__EXTERN TCutG* alphas;
  
  //declare histograms here
  
  sak::Hist1D *nai_e_lu[10];
  sak::Hist1D *nai_e_ld[10];
  sak::Hist1D *nai_e_ru[10];
  sak::Hist1D *nai_e_rd[10];
  
  sak::Hist1D *nai_sume_l[10];
  sak::Hist1D *nai_sume_r[10];
  
  sak::Hist1D *nai_t_lu[10];
  sak::Hist1D *nai_t_ld[10];
  sak::Hist1D *nai_t_ru[10];
  sak::Hist1D *nai_t_rd[10];


NaI_NeutAnalyzer::NaI_NeutAnalyzer()
{

}

  
void NaI_NeutAnalyzer::Begin(){

  rootfile=new TFile("nai_neut_analysis.root","RECREATE");
  rootfile->mkdir("e");
  rootfile->mkdir("t");
  rootfile->mkdir("esum");

  
  for(int i=0;i<10;i++){
    rootfile->cd("e");
    nai_e_lu[i]=new sak::Hist1D(Form("nai_e_lu%d",i+1),"e[arb]",4096,0,4095);
    nai_e_ld[i]=new sak::Hist1D(Form("nai_e_ld%d",i+1),"e[arb]",4096,0,4095);
    nai_e_ru[i]=new sak::Hist1D(Form("nai_e_ru%d",i+1),"e[arb]",4096,0,4095);
    nai_e_rd[i]=new sak::Hist1D(Form("nai_e_rd%d",i+1),"e[arb]",4096,0,4095);
    rootfile->cd("t");
    nai_t_lu[i]=new sak::Hist1D(Form("nai_t_lu%d",i+1),"e[arb]",4096,0,4095);
    nai_t_ld[i]=new sak::Hist1D(Form("nai_t_ld%d",i+1),"e[arb]",4096,0,4095);
    nai_t_ru[i]=new sak::Hist1D(Form("nai_t_ru%d",i+1),"e[arb]",4096,0,4095);
    nai_t_rd[i]=new sak::Hist1D(Form("nai_t_rd%d",i+1),"e[arb]",4096,0,4095);
    rootfile->cd("esum");
    nai_sume_l[i]=new sak::Hist1D(Form("nai_sume_l%d",i+1),"e[arb]",4096,0,4095);
    nai_sume_r[i]=new sak::Hist1D(Form("nai_sume_r%d",i+1),"e[arb]",4096,0,4095);



  }


  
}

void NaI_NeutAnalyzer::Process(){


  double prot_E=0;
  double prot_dE=0;
  double prot_theta=0;

  //Fill raw parameter Histograms below this line
  /******************************************************/
  for (int i=0;i<10;i++){
    nai_e_lu[i]->Fill(nai[i].fE[0]);
    nai_e_ld[i]->Fill(nai[i].fE[1]);
    nai_e_ru[i]->Fill(nai[i+10].fE[0]);
    nai_e_rd[i]->Fill(nai[i+10].fE[1]);
    nai_t_lu[i]->Fill(nai[i].fT[0]);
    nai_t_ld[i]->Fill(nai[i].fT[1]);
    nai_t_ru[i]->Fill(nai[i+10].fT[0]);
    nai_t_rd[i]->Fill(nai[i+10].fT[1]);
    nai_sume_l[i]->Fill(nai[i].SumE());
    nai_sume_r[i]->Fill(nai[i+10].SumE());

  }



  ApplyCalibrations();
  //Fill calpar.Histograms below this line
  /*************************************************************/
  
  
  int neutcheck[10]={0};
  int orcheck=0;
  neutcheck[0] = n0_neuts->IsInside(neut[0].fPSD,neut[0].fQ_long);
  neutcheck[1] = n1_neuts->IsInside(neut[1].fPSD,neut[1].fQ_long);
  neutcheck[2] = n2_neuts->IsInside(neut[2].fPSD,neut[2].fQ_long);
  neutcheck[3] = n3_neuts->IsInside(neut[3].fPSD,neut[3].fQ_long);
  neutcheck[4] = n4_neuts->IsInside(neut[4].fPSD,neut[4].fQ_long);
  neutcheck[5] = n5_neuts->IsInside(neut[5].fPSD,neut[5].fQ_long);
  neutcheck[6] = n6_neuts->IsInside(neut[6].fPSD,neut[6].fQ_long);
  neutcheck[7] = n7_neuts->IsInside(neut[7].fPSD,neut[7].fQ_long);
  neutcheck[8] = n8_neuts->IsInside(neut[8].fPSD,neut[8].fQ_long);
  neutcheck[9] = n9_neuts->IsInside(neut[9].fPSD,neut[9].fQ_long);
  
  for (int i=0;i<10;i++)	    
    orcheck = neutcheck[i] || orcheck;
  
  int protcheck=0;
  int alphacheck=0;

  if(si_[1].front.fMult>0&&si_[0].front.fE[0]){
      prot_dE=si_[1].front.fE[0];
      prot_E=si_[0].front.fE[0]+prot_dE;
      prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
  }

  if(prots1)
    protcheck=prots1->IsInside(prot_E,prot_dE);
  if(alphas)
    alphacheck=alphas->IsInside(prot_E,prot_dE);
  

}

  
void NaI_NeutAnalyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}




  void  nai_neut_analysis(const char * infile, const char * config, const char * cuts){
    NaI_NeutAnalyzer a;
    a.Init(infile);
    a.LoadVariableFile(config);
    a.SetCalibrations();
    LoadGates(cuts);
    a.Loop();

    
    
  }

}
