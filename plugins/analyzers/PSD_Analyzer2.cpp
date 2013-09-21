#include "PSD_Analyzer2.hpp"
namespace psd{

  R__EXTERN TFile *rootfile;
  
  
  //declare Gates
  TCutG* n0_neuts;
  TCutG* n1_neuts;
  TCutG* n2_neuts;
  TCutG* n3_neuts;
  TCutG* n4_neuts;
  TCutG* n5_neuts;
  TCutG* n6_neuts;
  TCutG* n7_neuts;
  TCutG* n8_neuts;
  TCutG* n9_neuts;
  TCutG* prots1;
  TCutG* prots2;
  TCutG* prots3;
  TCutG* prots4;
  TCutG* alphas;
  TCutG* n3_evt;
  TCutG* s1_delayed_rf;
  TCutG* s2_delayed_rf;

  
  //declare histograms here
  
  R__EXTERN sak::Histogram1D *hrftime;
  sak::Hist1D *hrftime_allneut;
  R__EXTERN sak::Hist2D *hrftime_n;
  sak::Hist2D *hrftime_raw_n;
  sak::Hist2D *hrftime_gated_n;
  R__EXTERN sak::Histogram1D *hrftime_cal;
  sak::Hist1D *hrftime_allneut_cal;
  sak::Hist1D *hrftime_allneut_cal_p;
  sak::Hist1D *hrftime_allneut_cal_proton;
  sak::Hist1D *hrftime_allneut_cal_alphas;
  R__EXTERN sak::Hist1D *h_ndetMult;
  R__EXTERN sak::Hist1D *h_ndetMult_ngated;
  R__EXTERN sak::Hist1D *h_ndetMult_npgated;
  R__EXTERN sak::Hist2D *s2_tvrf;
  sak::Hist2D *s2_tvrf_neut_gated;
  sak::Hist2D *s1_tvrf;
  sak::Hist2D *s1_tvrf_neut_gated;
  R__EXTERN sak::Hist2D *s2_e_v_theta;
  sak::Hist2D *s2_e_v_theta_ngated;
  sak::Hist2D *s2_e_v_theta_protons;
  sak::Hist1D *hrftime_prots;
  sak::Hist2D *hPSD_n1_prots;
  R__EXTERN sak::Histogram2D *hPSDq_n[10];
  R__EXTERN sak::Histogram2D *hPSD_n_[10];
  R__EXTERN sak::Histogram2D *hTrel_n[10];
  sak::Histogram2D *hEvT_n[10];
  R__EXTERN sak::Hist2D* hpede;
  R__EXTERN sak::Hist2D* hpede_2;
  sak::Hist2D* hpede_ngated;
  sak::Hist1D* hTDC[32];

  sak::Hist1D* hrftime_ngated[10];     
  sak::Hist1D* hrftime_ngated_s1_delayed[10];
  sak::Hist1D* hrftime_ngated_prot[10];
  sak::Hist1D* hrftime_nevtgated[10];

  sak::Hist1D *hrftime_cal_s1_delayed;	      
  sak::Hist1D *hrftime_cal_s2_delayed;	      
  sak::Hist1D *hrftime_allneut_cal_s1_delayed;
  sak::Hist1D *hrftime_allneut_cal_s2_delayed;




NeutAnalyzer2::NeutAnalyzer2()
{

}

void NeutAnalyzer2::Begin(){
  int idx=0;

  rootfile=new TFile("psd_analysis2.root","RECREATE");
  rootfile->mkdir("rftime");
  rootfile->mkdir("rftime/single_detector");
  for(int i=0;i<10;i++)
    rootfile->mkdir(Form("rftime/single_detector/neut%d",i));
  rootfile->mkdir("neut_timing");
  rootfile->mkdir("neut_PSD");
  rootfile->mkdir("protons");

  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[arb. units]",4096,0,4095);
  hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[arb. units]",4096,0,4095);
  hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",4096,0,1023);
  hrftime_prots=new sak::Histogram1D("hrftime_prots","rftime[ns]",4096,0,1023);
  hrftime_allneut_cal=new sak::Histogram1D("hrftime_allneut_cal","rftime[ns]",4096,0,1023); 
  hrftime_allneut_cal_p=new sak::Histogram1D("hrftime_allneut_cal_p","rftime[ns]",4096,0,1023); 
  hrftime_allneut_cal_proton=new sak::Histogram1D("hrftime_allneut_cal_protons","rftime[ns]",4096,0,1023); 
  hrftime_allneut_cal_alphas=new sak::Histogram1D("hrftime_allneut_cal_alphas","rftime[ns]",4096,0,1023); 
  hrftime_cal_s1_delayed=new sak::Histogram1D("hrftime_cal_s1_delayed","rftime[ns]",4096,0,1023); ;
  hrftime_cal_s2_delayed=new sak::Histogram1D("hrftime_cal_s2_delayed","rftime[ns]",4096,0,1023); ;
  hrftime_allneut_cal_s1_delayed=new sak::Histogram1D("hrftime_allneut_cal_s1_delayed","rftime[ns]",4096,0,1023); ;
  hrftime_allneut_cal_s2_delayed=new sak::Histogram1D("hrftime_allneut_cal_s2_delayed","rftime[ns]",4096,0,1023); ;
  hrftime_n=new sak::Histogram2D("hrftime_n","Detector","rftime[ns]",17,0,16,4096,0,1023);
  hrftime_gated_n=new sak::Histogram2D("hrftime_gated_n","Detector","rftime[ns]",17,0,16,4096,0,1023);
  hrftime_raw_n =new sak::Histogram2D("hrftime_raw_n","Detector","rftime[ns]",17,0,16,4096,0,4095);

  for(int i=0;i<10;i++){
    rootfile->cd(Form("rftime/single_detector/neut%d",i));
    hrftime_ngated[i]=new sak::Histogram1D(Form("hrftime_n%d_gated",i),"rftime[ns]",4096,0,1023);     
    hrftime_ngated_s1_delayed[i]=new sak::Histogram1D(Form("hrftime_n%d_s1delaygated",i),"rftime[ns]",4096,0,1023);
    hrftime_ngated_prot[i]=new sak::Histogram1D(Form("hrftime_n%d_protgated",i),"rftime[ns]",4096,0,1023);
    hrftime_nevtgated[i]=new sak::Histogram1D(Form("hrftime_n%d_evtgated",i),"rftime[ns]",4096,0,1023);
  }


  rootfile->cd("protons");
  hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
  hpede_2=new sak::Hist2D("hpEdE_2","E [MeV]","dE [MeV]",64,0,30,64,0,30);
  hPSD_n1_prots=new sak::Hist2D("hPSD_n1_prots","fPSD","fQ_long",256,0.,1.,1024,50,4096);
  hpede_ngated=new sak::Hist2D("hpEdE_ngated","E [MeV]","dE [MeV]",256,0,32,256,0,32);
 
  for(int i=0;i<32;i++){
    hTDC[i]=new sak::Histogram1D(Form("hTDC%d",i),"t[arb]",4096,1,4095);
  }
  s2_tvrf_neut_gated=new sak::Hist2D("s2_tvrf_neut_gated","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
  s2_tvrf=new sak::Hist2D("s2_tvrf","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
  s1_tvrf_neut_gated=new sak::Hist2D("s1_tvrf_neut_gated","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
  s1_tvrf=new sak::Hist2D("s1_tvrf","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
  s2_e_v_theta_ngated=new sak::Hist2D("s2_e_v_theta_neut_gated","Theta [deg]","E[MeV]",32,10,50,32,0.,15.);
  s2_e_v_theta =new sak::Hist2D("s2_e_v_theta","Theta","E ]MeV]",32,10,50,32,0.,15.);
  s2_e_v_theta_protons =new sak::Hist2D("s2_e_v_theta_protons","Theta","E ]MeV]",32,10,50,32,0.,15.);

  for(int i=0;i<10;i++){  
    rootfile->cd("neut_PSD");
    hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",1024,50,4096,1024,50,4096);
    hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,1024,50,4096);
    rootfile->cd("neut_timing");
    hTrel_n[i]=new sak::Histogram2D(Form("hTrel_n%d",i),"Trel","E",64,0,128,512,50,2048);
    hEvT_n[i]=new sak::Histogram2D(Form("hEvT_n%d",i),"rftime[ns]","E[arb.units]",1200,0,1199,2048,50,1000);
  }
  
  h_ndetMult=new sak::Hist1D("h_ndetmult","mult",11,0,10);
  h_ndetMult_ngated=new sak::Hist1D("h_ndetmult_ngated","mult",11,0,10);
  h_ndetMult_npgated=new sak::Hist1D("h_ndetmult_npgated","mult",11,0,10);


  rootfile->cd();
  
}

void NeutAnalyzer2::Process(){


  double prot_E=0;
  double prot_dE=0;
  double prot_theta=0;

  //Fill raw parameter Histograms below this line
  /******************************************************/
  hrftime->Fill(rftime[0].fT);
  for (int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    if(neut[i].fQ_long>0){
      hrftime_raw_n->Fill(i,rftime[0].fT);
      hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
    }
  }


  ApplyCalibrations();
  //Fill calpar.Histograms below this line
  /*************************************************************/
  h_ndetMult->Fill(Narray.fMult);
  if(Narray.fMult>1)
    return;


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
  int s1_delaycheck=0;
  int s2_delaycheck=0;



  if(si_[0].front.fMult>0&&si_[1].front.fMult>0){
      prot_dE=si_[1].front.fE[0];
      prot_E=si_[0].front.fE[0]+prot_dE;
      hpede->Fill(prot_E,prot_dE);
      hpede_2->Fill(prot_E,prot_dE);
      prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
      s2_e_v_theta->Fill(prot_theta,prot_E);
  }

  //if(prots1)
  // protcheck=prots1->IsInside(prot_E,prot_dE);
  //if(prots3)
  // protcheck=prots3->IsInside(prot_E,prot_dE);

  if(prots4)
    protcheck=prots4->IsInside(prot_E,prot_dE);
  if(alphas)
    alphacheck=alphas->IsInside(prot_E,prot_dE);
  
  if(protcheck)
    hrftime_prots->Fill(rftime[0].fT);
  
  if(s2_delayed_rf->IsInside(rftime[0].fT,si_[1].back.fT[0]))
    s2_delaycheck=1;
  
  if(s1_delayed_rf->IsInside(rftime[0].fT,si_[0].back.fT[0]))
    s1_delaycheck=1;

  hrftime_cal->Fill(rftime[0].fT);
  s2_tvrf->Fill(rftime[0].fT,si_[1].back.fT[0]);
  s1_tvrf->Fill(rftime[0].fT,si_[0].back.fT[0]);


  if(s1_delaycheck){
    hrftime_cal_s1_delayed->Fill(rftime[0].fT);
    if(orcheck){
      hrftime_allneut_cal_s1_delayed->Fill(rftime[0].fT);
    }
  }

  if(s2_delaycheck){
    hrftime_cal_s2_delayed->Fill(rftime[0].fT);
    if(orcheck){
      hrftime_allneut_cal_s2_delayed->Fill(rftime[0].fT);
    }
  }

  if(orcheck){
    hrftime_allneut_cal->Fill(rftime[0].fT);
    s2_tvrf_neut_gated->Fill(rftime[0].fT,si_[1].back.fT[0]);
    s1_tvrf_neut_gated->Fill(rftime[0].fT,si_[0].back.fT[0]);
    s2_e_v_theta_ngated->Fill(prot_theta,prot_E);
    h_ndetMult_ngated->Fill(Narray.fMult);
    if(si_[1].front.fMult>0 && si_[0].front.fMult>0){
      hpede_ngated->Fill(prot_E,prot_dE);
      hrftime_allneut_cal_p->Fill(rftime[0].fT);
    }
  
    if(protcheck){
      hrftime_allneut_cal_proton->Fill(rftime[0].fT);
      h_ndetMult_npgated->Fill(Narray.fMult);
      s2_e_v_theta_protons->Fill(prot_theta,prot_E);
      hPSD_n1_prots->Fill(neut[1].fPSD,neut[1].fQ_long);
      for(int i=0;i<32;i++)
	hTDC[i]->Fill(TDC2[i]);
    }
    if(alphacheck)
      hrftime_allneut_cal_alphas->Fill(rftime[0].fT);
  }

  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);
    
    if(neut[i].fQ_long>0){
      hrftime_n->Fill(i,rftime[0].fT);
      
      if(neutcheck[i]){
	hTrel_n[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
	hrftime_gated_n->Fill(i,rftime[0].fT);
	hEvT_n[i]->Fill(rftime[0].fT,neut[i].fQ_long);
	hrftime_ngated[i]->Fill(rftime[0].fT);
	if(s1_delaycheck)
	  hrftime_ngated_s1_delayed[i]->Fill(rftime[0].fT);
	if(protcheck)
	  hrftime_ngated_prot[i]->Fill(rftime[0].fT);
	if(0)//not set yet
	  hrftime_nevtgated[i]->Fill(rftime[0].fT);


      }
    }
  }
}

  
void NeutAnalyzer2::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}


  void LoadGates(const std::string& input){
  
    sak::LoadCuts in(input.c_str());
    
    
    if(in.getCut("n0_neuts") && !n0_neuts)
      n0_neuts=new TCutG(*in.getCut("n0_neuts"));
    if(in.getCut("n1_neuts") && !n1_neuts)
      n1_neuts=new TCutG(*in.getCut("n1_neuts"));
    if(in.getCut("n2_neuts") && !n2_neuts)
      n2_neuts=new TCutG(*in.getCut("n2_neuts"));   
    if(in.getCut("n3_neuts") && !n3_neuts)
      n3_neuts=new TCutG(*in.getCut("n3_neuts"));
    if(in.getCut("n4_neuts") && !n4_neuts)
      n4_neuts=new TCutG(*in.getCut("n4_neuts"));
    if(in.getCut("n5_neuts") && !n5_neuts)
      n5_neuts=new TCutG(*in.getCut("n5_neuts"));
    if(in.getCut("n6_neuts") && !n6_neuts)
      n6_neuts=new TCutG(*in.getCut("n6_neuts"));   
    if(in.getCut("n7_neuts") && !n7_neuts)
      n7_neuts=new TCutG(*in.getCut("n7_neuts"));
    if(in.getCut("n8_neuts") && !n8_neuts)
      n8_neuts=new TCutG(*in.getCut("n8_neuts"));
    if(in.getCut("n9_neuts") && !n9_neuts)
      n9_neuts=new TCutG(*in.getCut("n9_neuts"));
    if(in.getCut("prots1") && !prots1)
      prots1=new TCutG(*in.getCut("prots1"));
    if(in.getCut("alphas") && !alphas)
      alphas=new TCutG(*in.getCut("alphas"));
    if(in.getCut("n3_evt") && !n3_evt)
      n3_evt=new TCutG(*in.getCut("n3_evt"));
    if(in.getCut("prots3") && !prots3)
      prots3=new TCutG(*in.getCut("prots3"));
    if(in.getCut("prots4") && !prots4)
      prots4=new TCutG(*in.getCut("prots4"));
    if(in.getCut("s1_delayed_rf") && !s1_delayed_rf)
      s1_delayed_rf=new TCutG(*in.getCut("s1_delayed_rf"));
    if(in.getCut("s2_delayed_rf") && !s2_delayed_rf)
      s2_delayed_rf=new TCutG(*in.getCut("s2_delayed_rf"));    


    
  }

  
  void  analysis2(const char * infile, const char * config, const char * cuts){
    NeutAnalyzer2 a;
    a.Init(infile);
    a.LoadVariableFile(config);
    a.SetCalibrations();
    LoadGates(cuts);
    a.Loop();

    
    
  }





}
