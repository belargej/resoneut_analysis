#include "PSD_Analyzer.hpp"

namespace psd{


  TFile* rootfile;
  
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
  TCutG* n0_neuts_raw;
  TCutG* n1_neuts_raw;
  TCutG* n2_neuts_raw;
  TCutG* n3_neuts_raw;
  TCutG* n4_neuts_raw;
  TCutG* n5_neuts_raw;
  TCutG* n6_neuts_raw;
  TCutG* n7_neuts_raw;
  TCutG* n8_neuts_raw;
  TCutG* n9_neuts_raw;
  TCutG* prots1;
  TCutG* prots2;
  TCutG* alphas;
  TCutG* s1_delayed_rf;
  TCutG* s2_delayed_rf;
  TCutG* n0_evt;
  TCutG* n1_evt;
  TCutG* n2_evt;
  TCutG* n3_evt;
  TCutG* n4_evt;
  TCutG* n5_evt;
  TCutG* n6_evt;
  TCutG* n7_evt;
  TCutG* n8_evt;
  TCutG* n9_evt;



  //declare histograms here
  //raw
  sak::Histogram1D *hrftime;
  sak::Histogram1D *hrftime_prots;
  sak::Hist2D *hrftime_n;
  sak::Hist2D *hrftime_gated_n;
  sak::Hist1D *hrftime_allneut;

  //cal
  sak::Hist1D *hrftime_cal; 
  sak::Hist1D *hrftime_cal_prots;
  sak::Hist1D *hrftime_cal_s1_delayed;	      
  sak::Hist1D *hrftime_cal_s2_delayed;	
  sak::Hist1D *hrftime_cal_allneut;
  sak::Hist1D *hrftime_cal_allneut_proton;
  sak::Hist1D *hrftime_cal_allneut_alphas;     
  sak::Hist1D *hrftime_cal_allneut_s1_delayed;
  sak::Hist1D *hrftime_cal_allneut_s2_delayed;
  sak::Hist1D *hrftime_cal_ngated[10];     
  sak::Hist1D *hrftime_cal_ngated_s1_delayed[10];
  sak::Hist1D *hrftime_cal_ngated_prot[10];
  sak::Hist1D *hrftime_cal_nevtgated[10];
  sak::Hist2D *s2_tvrf;
  sak::Hist2D *s2_tvrf_neut_gated;
  sak::Hist2D *s1_tvrf;
  sak::Hist2D *s1_tvrf_neut_gated;
  sak::Histogram2D *hTrel_cal_n[10];
  sak::Hist2D * hTrel_cal_ngated[10];
  sak::Histogram2D *hEvT_cal_n[10];

  //cal2
  sak::Hist1D *hrftime_cal2; 
  sak::Hist1D *hrftime_cal2_prots;
  sak::Hist1D *hrftime_cal2_s1_delayed;	      
  sak::Hist1D *hrftime_cal2_s2_delayed;	
  sak::Hist1D *hrftime_cal2_allneut;
  sak::Hist1D *hrftime_cal2_allneut_proton;
  sak::Hist1D *hrftime_cal2_allneut_alphas;     
  sak::Hist1D *hrftime_cal2_allneut_s1_delayed;
  sak::Hist1D *hrftime_cal2_allneut_s2_delayed;
  sak::Hist1D *hrftime_cal2_ngated[10];     
  sak::Hist1D *hrftime_cal2_ngated_s1_delayed[10];
  sak::Hist1D *hrftime_cal2_ngated_prot[10];
  sak::Hist1D *hrftime_cal2_ngated_mult2_tadded[10];   
  sak::Hist1D *hrftime_cal2_ngated_mult1plus_tadded[10];
  sak::Hist1D *hrftime_cal2_ngated_mult2plus_tadded[10];
  sak::Hist1D* hnKE[10];
  sak::Hist1D* hnKE_gated[10];
  sak::Hist1D* hQval[10];
  sak::Hist2D* hnKE_vnE[10];
  sak::Hist2D* hnKE_gated_vnE[10];
  sak::Histogram2D *hEvT_cal2_n[10];

  //n_detmult
  sak::Hist1D *h_ndetMult;
  sak::Hist1D *h_ndetMult_ngated;
  sak::Hist1D *h_ndetMult_npgated;

  //neut_psd
  sak::Histogram2D *hPSDq_n[10];
  sak::Histogram2D *hPSD_n_[10];

  //protons
  sak::Hist2D* hpede;
  sak::Hist2D* hpede_2;    
  sak::Hist2D* hpede_ngated;

  //trel
  
  sak::Hist2D * hRf_v_trel[10];

  //gamma
  
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
  
  sak::Hist1D *hTDC3[32];
  sak::Hist1D *hTDC4[32];
  sak::Hist1D *hADC4[32];
  sak::Hist1D *hADC7[32];

  sak::Hist2D *nai_t_lu_v_neut1e[10];
  sak::Hist2D *nai_t_ld_v_neut1e[10];
  sak::Hist2D *nai_t_ru_v_neut1e[10];
  sak::Hist2D *nai_t_rd_v_neut1e[10];
  
  sak::Hist1D *hnai_delay;
  sak::Hist1D *hnai_hit;

  sak::Hist1D *hrftime_cal_allneut_gamma_delay;
  sak::Hist1D *hrftime_cal_allneut_gamma_gamma_delay;
  sak::Hist1D *hrftime_cal_allneut_gamma;
  sak::Hist1D *hrftime_cal_allneut_gamma_gamma;
  sak::Hist1D *hrftime_cal_allneut_gamma_prot;
  sak::Hist1D* hrftime_cal_ngated_gamma[10];     
  sak::Hist1D* hrftime_cal_ngated_gamma_gamma[10];
  sak::Hist1D* hrftime_cal_n_gamma[10];
  sak::Hist1D* hrftime_cal_n_gamma_gamma[10];
  sak::Hist1D *hrftime_cal2_allneut_gamma_delay;
  sak::Hist1D *hrftime_cal2_allneut_gamma_gamma_delay;
  sak::Hist1D *hrftime_cal2_allneut_gamma;
  sak::Hist1D *hrftime_cal2_allneut_gamma_gamma;
  sak::Hist1D *hrftime_cal2_allneut_gamma_prot;
  sak::Hist1D* hrftime_cal2_ngated_gamma[10];     
  sak::Hist1D* hrftime_cal2_ngated_gamma_delay[10];     
  sak::Hist1D* hrftime_cal2_ngated_gamma_gamma[10];
  sak::Hist1D* hrftime_cal2_ngated_gamma_gamma_delay[10];
  sak::Hist1D* hrftime_cal2_n_gamma[10];
  sak::Hist1D* hrftime_cal2_n_gamma_gamma[10];
  sak::Hist1D * hrftime_cal2_ngated_mult2_tadded_gamma[10];   
  sak::Hist1D * hrftime_cal2_ngated_mult1plus_tadded_gamma[10];
  sak::Hist1D * hrftime_cal2_ngated_mult2plus_tadded_gamma[10];
  sak::Hist1D * hnKE_gamma[10];
  sak::Hist1D * hnKE_gamma_gamma[10];

  sak::Hist1D * hrftime_cal_evtgated[10];	     
  sak::Hist1D * hrftime_cal_evtgated_gamma[10];	     
  sak::Hist1D * hrftime_cal_evtgated_gamma_gamma[10];
  sak::Hist1D * hrftime_cal_evtgated_s1delayed[10];  
  sak::Hist1D * hrftime_cal_evtgated_s2delayed[10];  

  sak::Hist1D * hrftime_cal2_evtgated[10];
  sak::Hist1D * hrftime_cal2_evtgated_gamma[10];
  sak::Hist1D * hrftime_cal2_evtgated_gamma_gamma[10];
  sak::Hist1D * hrftime_cal2_evtgated_gamma_delay[10];
  sak::Hist1D * hrftime_cal2_evtgated_gamma_gamma_delay[10];
  sak::Hist1D * hrftime_cal2_evtgated_s1delayed[10];
  sak::Hist1D * hrftime_cal2_evtgated_s2delayed[10];
  
  sak::Hist1D * hrftime_cal2_allneut_evt;
  sak::Hist1D * hrftime_cal2_allneut_evt_s1_delayed;
  sak::Hist1D * hrftime_cal2_allneut_evt_s2_delayed;
  sak::Hist1D * hrftime_cal2_allneut_evt_gamma;
  sak::Hist1D * hrftime_cal2_allneut_evt_gamma_gamma;
  sak::Hist1D * hrftime_cal2_allneut_evt_gamma_delay;
  sak::Hist1D * hrftime_cal2_allneut_evt_gamma_gamma_delay;

  sak::Hist1D * hrftime_cal_allneut_evt;
  sak::Hist1D * hrftime_cal_allneut_evt_s1_delayed;
  sak::Hist1D * hrftime_cal_allneut_evt_s2_delayed;
  sak::Hist1D * hrftime_cal_allneut_evt_gamma;
  sak::Hist1D * hrftime_cal_allneut_evt_gamma_gamma;

  NeutAnalyzer::NeutAnalyzer()
  {
    
  }



  void NeutAnalyzer::Begin(){
    int idx=0;
    
    rootfile=new TFile("psd_analysis.root","RECREATE");
    rootfile->mkdir("rftime/raw");
    rootfile->mkdir("rftime/cal");
    rootfile->mkdir("rftime/cal2");
    rootfile->mkdir("rftime/cal/single_detector");
    rootfile->mkdir("rftime/cal2/single_detector");
    for(int i=0;i<10;i++){
    rootfile->mkdir(Form("rftime/cal/single_detector/neut%d",i));
    rootfile->mkdir(Form("rftime/cal2/single_detector/neut%d",i));
    }  
    rootfile->mkdir("neut_PSD/raw");
    rootfile->mkdir("neut_PSD/cal");
    rootfile->mkdir("protons");
    rootfile->mkdir("gammas");
    rootfile->mkdir("gammas/e");
    rootfile->mkdir("gammas/e/left");
    rootfile->mkdir("gammas/e/right");
    rootfile->mkdir("gammas/t");
    rootfile->mkdir("gammas/t/left");
    rootfile->mkdir("gammas/t/right");
    rootfile->mkdir("gammas/esum");
    rootfile->mkdir("gammas/esum/left");
    rootfile->mkdir("gammas/esum/right");
    
    rootfile->cd("rftime/raw");
    hrftime=new sak::Histogram1D("hrftime","rftime[arb. units]",4096,0,4095);
    hrftime_prots=new sak::Histogram1D("hrftime_prots","rftime[ns]",4096,0,1023);    
    hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[arb. units]",4096,0,4095);
    hrftime_n=new sak::Histogram2D("hrftime_n","Detector","rftime[ns]",17,0,16,4096,0,4095);
    hrftime_gated_n=new sak::Histogram2D("hrftime_gated_n","Detector","rftime[ns]",17,0,16,4096,0,4095);
    
    
    rootfile->cd("rftime/cal");
    hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",1270,0,1269);
    hrftime_cal_prots=new sak::Histogram1D("hrftime_cal_prots","rftime[ns]",4096,0,1023);
    hrftime_cal_s1_delayed=new sak::Histogram1D("hrftime_cal_s1_delayed","rftime[ns]",4096,0,1023); 
    hrftime_cal_s2_delayed=new sak::Histogram1D("hrftime_cal_s2_delayed","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut=new sak::Histogram1D("hrftime_cal_allneut","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut_proton=new sak::Histogram1D("hrftime_cal_allneut_protons","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut_alphas=new sak::Histogram1D("hrftime_cal_allneut_alphas","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut_s1_delayed=new sak::Histogram1D("hrftime_cal_allneut_s1_delayed","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut_s2_delayed=new sak::Histogram1D("hrftime_cal_allneut_s2_delayed","rftime[ns]",4096,0,1023);
    hrftime_cal_allneut_gamma_delay=new sak::Histogram1D("hrftime_cal_allneut_gamma_delay","rftime[ns]",4096,0,1023);
    hrftime_cal_allneut_gamma_gamma_delay=new sak::Histogram1D("hrftime_cal_allneut_gamma_gamma_delay","rftime[ns]",4096,0,1023);
    hrftime_cal_allneut_gamma=new sak::Histogram1D("hrftime_cal_allneut_gamma","rftime[ns]",4096,0,1023);
    hrftime_cal_allneut_gamma_gamma=new sak::Histogram1D("hrftime_cal_allneut_gamma_gamma","rftime[ns]",4096,0,1023); 
    hrftime_cal_allneut_evt=new sak::Histogram1D("hrftime_cal2_allneut_evt","rftime[ns]",256,0,85); 
    hrftime_cal_allneut_evt_s1_delayed=new sak::Histogram1D("hrftime_cal2_allneut_evt_s1_delayed","rftime[ns]",256,0,85); 
    hrftime_cal_allneut_evt_s2_delayed=new sak::Histogram1D("hrftime_cal2_allneut_evt_s2_delayed","rftime[ns]",256,0,85); 
    hrftime_cal_allneut_evt_gamma=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma","rftime[ns]",256,0,85); 
    hrftime_cal_allneut_evt_gamma_gamma=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma_gamma","rftime[ns]",256,0,85); 
    
    rootfile->cd("rftime/cal2");
    hrftime_cal2=new sak::Histogram1D("hrftime_cal2","rftime[ns]",256,0,85);
    hrftime_cal2_prots=new sak::Histogram1D("hrftime_cal2_prots","rftime[ns]",256,0,85);
    hrftime_cal2_s1_delayed=new sak::Histogram1D("hrftime_cal2_s1_delayed","rftime[ns]",256,0,85); 
    hrftime_cal2_s2_delayed=new sak::Histogram1D("hrftime_cal2_s2_delayed","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut=new sak::Histogram1D("hrftime_cal2_allneut","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_proton=new sak::Histogram1D("hrftime_cal2_allneut_protons","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_alphas=new sak::Histogram1D("hrftime_cal2_allneut_alphas","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_s1_delayed=new sak::Histogram1D("hrftime_cal2_allneut_s1_delayed","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_s2_delayed=new sak::Histogram1D("hrftime_cal2_allneut_s2_delayed","rftime[ns]",256,0,85);
    hrftime_cal2_allneut_gamma_delay=new sak::Histogram1D("hrftime_cal2_allneut_gamma_delay","rftime[ns]",256,0,85);
    hrftime_cal2_allneut_gamma_gamma_delay=new sak::Histogram1D("hrftime_cal2_allneut_gamma_gamma_delay","rftime[ns]",256,0,85);
    hrftime_cal2_allneut_gamma=new sak::Histogram1D("hrftime_cal2_allneut_gamma","rftime[ns]",256,0,85);
    hrftime_cal2_allneut_gamma_gamma=new sak::Histogram1D("hrftime_cal2_allneut_gamma_gamma","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt=new sak::Histogram1D("hrftime_cal2_allneut_evt","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_s1_delayed=new sak::Histogram1D("hrftime_cal2_allneut_evt_s1_delayed","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_s2_delayed=new sak::Histogram1D("hrftime_cal2_allneut_evt_s2_delayed","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_gamma=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_gamma_gamma=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma_gamma","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_gamma_delay=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma_delay","rftime[ns]",256,0,85); 
    hrftime_cal2_allneut_evt_gamma_gamma_delay=new sak::Histogram1D("hrftime_cal2_allneut_evt_gamma_gamma_delay","rftime[ns]",256,0,85); 

    for(int i=0;i<10;i++){
      
      rootfile->cd(Form("rftime/cal/single_detector/neut%d",i));
      hrftime_cal_ngated[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_gated",i),"rftime[ns]",4096,0,1023);     
      hrftime_cal_ngated_s1_delayed[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_s1delaygated",i),"rftime[ns]",4096,0,1023);
      hrftime_cal_ngated_prot[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_protgated",i),"rftime[ns]",4096,0,1023);
      hrftime_cal_ngated_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_gated_gamma",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_ngated_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_gated_gamma_gamma",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_n_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_gamma",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_n_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_gamma_gamma",i),"rftime[ns]",4096,0,1023); 
      hTrel_cal_n[i]=new sak::Histogram2D(Form("hTrel_cal_n%d",i),"Trel","E",64,0,128,512,50,2500);
      hTrel_cal_ngated[i]=new sak::Histogram2D(Form("hTrel_cal_ngated%d",i),"Trel","E",64,0,128,512,50,2500);
      hEvT_cal_n[i]=new sak::Histogram2D(Form("hEvT_n%d",i),"rftime[ns]","E[arb.units]",1200,0,1199,2048,50,1000);

      hrftime_cal_evtgated[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_evtgated",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_evtgated_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_evtgated_gamma",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_evtgated_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_evtgated_gamma_gamma",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_evtgated_s1delayed[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_evtgated_s1delayed",i),"rftime[ns]",4096,0,1023); 
      hrftime_cal_evtgated_s2delayed[i]=new sak::Histogram1D(Form("hrftime_cal_n%d_evtgated_s2delayed",i),"rftime[ns]",4096,0,1023); 



      rootfile->cd(Form("rftime/cal2/single_detector/neut%d",i));
      hrftime_cal2_ngated[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated",i),"rftime[ns]",256,0,85);     
      hrftime_cal2_ngated_s1_delayed[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_s1delaygated",i),"rftime[ns]",256,0,85);
      hrftime_cal2_ngated_prot[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_protgated",i),"rftime[ns]",256,0,85);
      hrftime_cal2_ngated_mult2_tadded[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m2_tadd",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_mult2plus_tadded[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m2plus_tadd",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_mult1plus_tadded[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m1plus_tadd",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_gamma_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_n_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_n_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gamma_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_mult2_tadded_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m2_tadd_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_mult2plus_tadded_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m2plus_tadd_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hrftime_cal2_ngated_mult1plus_tadded_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_gated_m1plus_tadd_gamma",i),"rftime[ns]",256,0.0,85.0); 
      hEvT_cal2_n[i]=new sak::Histogram2D(Form("hEvT_n%d",i),"rftime[ns]","E[arb.units]",256,0,85,2048,50,1000);

      hrftime_cal2_evtgated[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_gamma",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_gamma_delay[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_gamma_delay",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_gamma_gamma[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_gamma_gamma",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_gamma_gamma_delay[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_gamma_gamma_delay",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_s1delayed[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_s1delayed",i),"rftime[ns]",256,0,85); 
      hrftime_cal2_evtgated_s2delayed[i]=new sak::Histogram1D(Form("hrftime_cal2_n%d_evtgated_s2delayed",i),"rftime[ns]",256,0,85); 


      hnKE[i]=new sak::Histogram1D(Form("hKE_n%d",i),"nKE[MeV]",512,0.0,5.0);
      hnKE_gated[i]=new sak::Histogram1D(Form("hKE_n%d_gated",i),"nKE[MeV]",512,0.0,5.0);
      hnKE_vnE[i]=new sak::Histogram2D(Form("hKE_n%d_vnE",i),"nKE[MeV]","E[arb]",512,0.0,5.0,1024,0,1024);
      hnKE_gated_vnE[i]=new sak::Histogram2D(Form("hKE_n%d_gated_vnE",i),"nKE[MeV]","E[arb]",512,0.0,5.0,1024,0,1024);

      hnKE_gamma[i]=new sak::Histogram1D(Form("hKE_n%d_gamma",i),"nKE[MeV]",512,0.0,5.0);
      hnKE_gamma_gamma[i]=new sak::Histogram1D(Form("hKE_n%d_gamma_gamma",i),"nKE[MeV]",512,0.0,5.0);

      hQval[i]=new sak::Histogram1D(Form("hQval_n%d_gated",i),"Q_value [MeV]",320,-20.0,20.0);
    
    }
    
    rootfile->cd("protons");
    hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_2=new sak::Hist2D("hpEdE_2","E [MeV]","dE [MeV]",64,0,30,64,0,30);
    hpede_ngated=new sak::Hist2D("hpEdE_ngated","E [MeV]","dE [MeV]",64,0,30,64,0,30);
    s2_tvrf=new sak::Hist2D("s2_tvrf","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
    s2_tvrf_neut_gated=new sak::Hist2D("s2_tvrf_neut_gated","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
    s1_tvrf=new sak::Hist2D("s1_tvrf","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
    s1_tvrf_neut_gated=new sak::Hist2D("s1_tvrf_neut_gated","rftime[ns]","TDC2",1200,0,1199,256,1.,4096.);
  
  
    
    for(int i=0;i<10;i++){  
      rootfile->cd("neut_PSD/raw");
      hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",1024,0,1023,1024,0,1023);
      rootfile->cd("neut_PSD/cal");
      hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,1024,50,4096);
    } 

    rootfile->cd("gammas");
    hnai_delay=new sak::Hist1D("hnai_delay","mult",20,0,19);
    hnai_hit=new sak::Hist1D("hnai_hit","mult",20,0,19);
 

    for(int i=0;i<10;i++){
      rootfile->cd("gammas/e/left");
      nai_e_lu[i]=new sak::Hist1D(Form("nai_e_lu%d",i+1),"e[arb]",4096,1,4096);
      nai_e_ld[i]=new sak::Hist1D(Form("nai_e_ld%d",i+1),"e[arb]",4096,1,4096);
      rootfile->cd("gammas/e/right");    
      nai_e_ru[i]=new sak::Hist1D(Form("nai_e_ru%d",i+1),"e[arb]",4096,1,4096);
      nai_e_rd[i]=new sak::Hist1D(Form("nai_e_rd%d",i+1),"e[arb]",4096,1,4096);
      rootfile->cd("gammas/t/left");
      nai_t_lu[i]=new sak::Hist1D(Form("nai_t_lu%d",i+1),"e[arb]",4096,1,4096);
      nai_t_ld[i]=new sak::Hist1D(Form("nai_t_ld%d",i+1),"e[arb]",4096,1,4096);
      rootfile->cd("gammas/t/right");    
      nai_t_ru[i]=new sak::Hist1D(Form("nai_t_ru%d",i+1),"e[arb]",4096,1,4096);
      nai_t_rd[i]=new sak::Hist1D(Form("nai_t_rd%d",i+1),"e[arb]",4096,1,4096);
      rootfile->cd("gammas/esum/left");
      nai_sume_l[i]=new sak::Hist1D(Form("nai_sume_l%d",i+1),"e[arb]",4096,1,4096);
      rootfile->cd("gammas/esum/right");
      nai_sume_r[i]=new sak::Hist1D(Form("nai_sume_r%d",i+1),"e[arb]",4096,1,4096);
    }


    
    rootfile->cd();
    
    h_ndetMult=new sak::Hist1D("h_ndetmult","mult",11,0,10);
    h_ndetMult_ngated=new sak::Hist1D("h_ndetmult_ngated","mult",11,0,10);
    h_ndetMult_npgated=new sak::Hist1D("h_ndetmult_npgated","mult",11,0,10);
    
    
    
  
    
  }
  

  void NeutAnalyzer::Process(){ 
    double prot_E=0;
    double prot_dE=0;
    double prot_theta=0;

    double T_cal=rftime[0].T();

    double T_wrapped=rftime[0].T_Wrapped();

    SetEvtGates();
    int evtcheck[10]={0};
    int evt_orcheck=0;
    int neutcheck[10]={0};
    int orcheck=0;
    /*
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
    */

    neutcheck[0] = n0_neuts_raw->IsInside(neut[0].fQ_long,neut[0].fQ_short);
    neutcheck[1] = n1_neuts_raw->IsInside(neut[1].fQ_long,neut[1].fQ_short);
    neutcheck[2] = n2_neuts_raw->IsInside(neut[2].fQ_long,neut[2].fQ_short);
    neutcheck[3] = n3_neuts_raw->IsInside(neut[3].fQ_long,neut[3].fQ_short);
    neutcheck[4] = n4_neuts_raw->IsInside(neut[4].fQ_long,neut[4].fQ_short);
    neutcheck[5] = n5_neuts_raw->IsInside(neut[5].fQ_long,neut[5].fQ_short);
    neutcheck[6] = n6_neuts_raw->IsInside(neut[6].fQ_long,neut[6].fQ_short);
    neutcheck[7] = n7_neuts_raw->IsInside(neut[7].fQ_long,neut[7].fQ_short);
    neutcheck[8] = n8_neuts_raw->IsInside(neut[8].fQ_long,neut[8].fQ_short);
    neutcheck[9] = n9_neuts_raw->IsInside(neut[9].fQ_long,neut[9].fQ_short);

    evtcheck[0] = n0_evt->IsInside(T_wrapped,neut[0].fQ_long);
    evtcheck[1] = n1_evt->IsInside(T_wrapped,neut[1].fQ_long);
    evtcheck[2] = n2_evt->IsInside(T_wrapped,neut[2].fQ_long);
    evtcheck[3] = n3_evt->IsInside(T_wrapped,neut[3].fQ_long);
    evtcheck[4] = n4_evt->IsInside(T_wrapped,neut[4].fQ_long);
    evtcheck[5] = n5_evt->IsInside(T_wrapped,neut[5].fQ_long);
    evtcheck[6] = n6_evt->IsInside(T_wrapped,neut[6].fQ_long);
    evtcheck[7] = n7_evt->IsInside(T_wrapped,neut[7].fQ_long);
    evtcheck[8] = n8_evt->IsInside(T_wrapped,neut[8].fQ_long);
    evtcheck[9] = n9_evt->IsInside(T_wrapped,neut[9].fQ_long);

    for (int i=0;i<10;i++){	    
      orcheck = neutcheck[i] || orcheck;
      evt_orcheck = evtcheck[i] || evt_orcheck;
    }


  
  int protcheck=0;
  int protcheck2=0;
  int alphacheck=0;
  int s1_delaycheck=0;
  int s2_delaycheck=0;
  int nai_hitcheck=0;
  int nai_delaycheck=0;

  if(si_[1].front.fMult>0&&si_[0].front.fMult>0){
    prot_dE=si_[1].Front_E(0);
    prot_E=si_[0].Front_E(0)+prot_dE;
    hpede->Fill(prot_E,prot_dE);
    hpede_2->Fill(prot_E,prot_dE);
    prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
  }



  if(prots1)
    protcheck=prots1->IsInside(prot_E,prot_dE);
  if(prots2)
    protcheck2=prots2->IsInside(prot_E,prot_dE);
  if(alphas)
    alphacheck=alphas->IsInside(prot_E,prot_dE);
  if(s2_delayed_rf)
    if(s2_delayed_rf->IsInside(T_cal,si_[1].Back_T(0)))
      s2_delaycheck=1;
  if(s1_delayed_rf)
    if(s1_delayed_rf->IsInside(T_cal,si_[0].Back_T(0)))
      s1_delaycheck=1;  

  for(int i=0;i<20;i++){
    if((nai[i].fE[0]>300&&nai[i].fE[0]<4000)
       ||(nai[i].fE[1]>300&&nai[i].fE[1]<4000))
      nai_hitcheck++;
    if((nai[i].fT[0]>0&&nai[i].fT[0]<nai[i].TZero(0))||
       (nai[i].fT[1]>0&&nai[i].fT[1]<nai[i].TZero(1)))
      {
	nai_delaycheck++;
      }

    
  }



  //Fill raw parameter Histograms below this line
  /******************************************************/
  hrftime->Fill(rftime[0].fT);
  for (int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
    

  }

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
 

  //Fill calpar.Histograms below this line
  /*************************************************************/
  h_ndetMult->Fill(Narray.fMult);  
  hrftime_cal->Fill(T_cal);
  hrftime_cal2->Fill(T_wrapped);

  if(protcheck){
    hrftime_prots->Fill(rftime[0].fT);
    hrftime_cal_prots->Fill(T_cal);
    hrftime_cal2_prots->Fill(T_wrapped);
    
  }
  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hTrel_cal_n[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);
    if(neut[i].fQ_long>0){
      hrftime_n->Fill(i,rftime[0].T());
    }
  }
  s2_tvrf->Fill(T_cal,si_[1].Back_T(0));
  s1_tvrf->Fill(T_cal,si_[0].Back_T(0));

    
    
    
    
    
  
  if(s1_delaycheck){
    hrftime_cal_s1_delayed->Fill(T_cal);
    hrftime_cal2_s1_delayed->Fill(T_wrapped);
    if(orcheck){
      if(evt_orcheck){
	hrftime_cal_allneut_evt_s1_delayed->Fill(T_cal);
	hrftime_cal2_allneut_evt_s1_delayed->Fill(T_wrapped);
      }
      hrftime_cal_allneut_s1_delayed->Fill(T_cal);
      hrftime_cal2_allneut_s1_delayed->Fill(T_wrapped);
    }
  }
  if(s2_delaycheck){
    hrftime_cal_s2_delayed->Fill(T_cal);
    hrftime_cal2_s2_delayed->Fill(T_wrapped);
    if(orcheck){
      if(evt_orcheck){
	hrftime_cal2_allneut_evt_s2_delayed->Fill(T_wrapped);
	hrftime_cal_allneut_evt_s2_delayed->Fill(T_cal);
      }
      hrftime_cal_allneut_s2_delayed->Fill(T_cal);
      hrftime_cal2_allneut_s2_delayed->Fill(T_wrapped);
    }
  }

  if(orcheck){
    if(evt_orcheck){
      hrftime_cal_allneut_evt->Fill(T_cal);
      hrftime_cal2_allneut_evt->Fill(T_wrapped);
      if(nai_delaycheck)
	hrftime_cal2_allneut_evt_gamma_delay->Fill(T_wrapped);
      if(nai_delaycheck>1)
	hrftime_cal2_allneut_evt_gamma_gamma_delay->Fill(T_wrapped);
    }
    hrftime_cal_allneut->Fill(T_cal);
    hrftime_cal2_allneut->Fill(T_wrapped);
    s2_tvrf_neut_gated->Fill(T_cal,si_[1].back.fT[0]);
    s1_tvrf_neut_gated->Fill(T_cal,si_[0].back.fT[0]);
    h_ndetMult_ngated->Fill(Narray.fMult);
    if(si_[1].front.fMult>0 && si_[0].front.fMult>0){
      hpede_ngated->Fill(prot_E,prot_dE);
    }
    if(protcheck){
      hrftime_cal_allneut_proton->Fill(T_cal);
      hrftime_cal2_allneut_proton->Fill(T_wrapped);
      h_ndetMult_npgated->Fill(Narray.fMult);
    }

    if(alphacheck)
      hrftime_cal_allneut_alphas->Fill(T_cal);
  }

  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    
    double n_RFT=T_wrapped;
    double n_ke=neut[i].nKE(n_RFT);
    double hi_ke=0,q_v=0;

    neut[i].Q_value_est(n_RFT,
			global::m_beam,
			global::m_frag,
			global::beam_est,
			hi_ke,
			q_v);



      

    
    
    if(neut[i].fQ_long>0){
      hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);
      hnKE[i]->Fill(n_ke);
      hnKE_vnE[i]->Fill(n_ke,neut[i].fQ_long);
      if(nai_hitcheck){
	hrftime_cal_n_gamma[i]->Fill(T_cal);
	hrftime_cal2_n_gamma[i]->Fill(T_wrapped);
	
      }
      if(nai_hitcheck>1){
	hrftime_cal_n_gamma_gamma[i]->Fill(T_cal);
	hrftime_cal2_n_gamma_gamma[i]->Fill(T_wrapped);
	
      }
     
      if(neutcheck[i]){
	if(evtcheck[i]){
	  hrftime_cal_evtgated[i]->Fill(T_cal);
	  hrftime_cal2_evtgated[i]->Fill(T_wrapped);
	  
	}
	hrftime_cal2_ngated_mult1plus_tadded[i]->Fill(n_RFT);
	if(Narray.fMult>1){
	  hrftime_cal2_ngated_mult2plus_tadded[i]->Fill(n_RFT);
	}
	if(Narray.fMult==2){
	  hrftime_cal2_ngated_mult2_tadded[i]->Fill(n_RFT);
	}
	if(nai_hitcheck){
	  if(evtcheck[i]){
	    hrftime_cal_evtgated_gamma[i]->Fill(T_cal);
	    hrftime_cal2_evtgated_gamma[i]->Fill(T_wrapped);  
	  }//neut,evtgate,gamma
	  hnKE_gamma[i]->Fill(n_ke);
	  hrftime_cal_ngated_gamma[i]->Fill(T_cal);
	  hrftime_cal2_ngated_gamma[i]->Fill(T_wrapped);
	  hrftime_cal2_ngated_mult1plus_tadded_gamma[i]->Fill(n_RFT);
	  if(Narray.fMult==2){
	    hrftime_cal2_ngated_mult2_tadded_gamma[i]->Fill(n_RFT);
	  }//neut,mult==2,gamma
	  if(Narray.fMult>1){	    
	    hrftime_cal2_ngated_mult2plus_tadded_gamma[i]->Fill(n_RFT);
	  
	  }//neut,mult>1,gamma
	
	}//neut,gamma
	if(nai_delaycheck && evtcheck[i]){
	  hrftime_cal2_evtgated_gamma_delay[i]->Fill(T_wrapped);
	}
	if(nai_delaycheck>1 && evtcheck[i]){
	  hrftime_cal2_evtgated_gamma_gamma_delay[i]->Fill(T_wrapped);
	}

	if(nai_hitcheck>1){
	
	  if(evtcheck[i]){
	    hrftime_cal_evtgated_gamma_gamma[i]->Fill(T_cal);
	    hrftime_cal2_evtgated_gamma_gamma[i]->Fill(T_wrapped);
	  }//neut,evt,gamma gamma

	  hrftime_cal_ngated_gamma_gamma[i]->Fill(T_cal);
	  hrftime_cal2_ngated_gamma_gamma[i]->Fill(T_wrapped);
	  hnKE_gamma_gamma[i]->Fill(n_ke);
	}//gamma gamma

	hnKE_gated[i]->Fill(n_ke);
	hnKE_gated_vnE[i]->Fill(n_ke,neut[i].fQ_long);
	hQval[i]->Fill(q_v);
	hTrel_cal_ngated[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
	hrftime_gated_n->Fill(i,rftime[0].fT);
	hEvT_cal_n[i]->Fill(T_cal,neut[i].fQ_long);
	hEvT_cal2_n[i]->Fill(T_wrapped,neut[i].fQ_long);
	hrftime_cal_ngated[i]->Fill(T_cal);
	hrftime_cal2_ngated[i]->Fill(T_wrapped);
	if(s1_delaycheck){
	  
	  if(evtcheck[i]){
	    hrftime_cal_evtgated_s1delayed[i]->Fill(T_cal);
	    hrftime_cal2_evtgated_s1delayed[i]->Fill(T_wrapped);
	    
	  }


	  hrftime_cal_ngated_s1_delayed[i]->Fill(T_cal);
	  hrftime_cal2_ngated_s1_delayed[i]->Fill(T_wrapped);
	}	
	if(s2_delaycheck){
	  if(evtcheck[i]){
	    hrftime_cal_evtgated_s2delayed[i]->Fill(T_cal);
	    hrftime_cal2_evtgated_s2delayed[i]->Fill(T_wrapped);
	  }
	}
	if(protcheck){
	  hrftime_cal_ngated_prot[i]->Fill(T_cal);
	  hrftime_cal2_ngated_prot[i]->Fill(T_wrapped);
	}	
      }
    }
  }

 
  if(nai_delaycheck&&orcheck){
    hrftime_cal_allneut_gamma_delay->Fill(T_cal);
    hrftime_cal2_allneut_gamma_delay->Fill(T_wrapped);
  }
  if(nai_hitcheck&&orcheck){
    hrftime_cal_allneut_gamma->Fill(T_cal);
    hrftime_cal2_allneut_gamma->Fill(T_wrapped);
    if(evt_orcheck){
      hrftime_cal_allneut_evt_gamma->Fill(T_cal);
      hrftime_cal2_allneut_evt_gamma->Fill(T_wrapped);
    }
  }
  if(nai_delaycheck>1 &&orcheck){
    hrftime_cal_allneut_gamma_gamma_delay->Fill(T_cal);
    hrftime_cal2_allneut_gamma_gamma_delay->Fill(T_wrapped);
  }
  if(nai_hitcheck>1 &&orcheck){
    hrftime_cal_allneut_gamma_gamma->Fill(T_cal);
    hrftime_cal2_allneut_gamma_gamma->Fill(T_wrapped);

    if(evt_orcheck){
      hrftime_cal_allneut_evt_gamma_gamma->Fill(T_cal);
      hrftime_cal2_allneut_evt_gamma_gamma->Fill(T_wrapped);
    }
  }
  
  hnai_delay->Fill(nai_delaycheck);
  hnai_hit->Fill(nai_hitcheck);



				 

  }

  
void NeutAnalyzer::Terminate(){
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
    if(in.getCut("n0_neuts_raw") && !n0_neuts_raw)
      n0_neuts_raw=new TCutG(*in.getCut("n0_neuts_raw"));
    if(in.getCut("n1_neuts_raw") && !n1_neuts_raw)
      n1_neuts_raw=new TCutG(*in.getCut("n1_neuts_raw"));
    if(in.getCut("n2_neuts_raw") && !n2_neuts_raw)
      n2_neuts_raw=new TCutG(*in.getCut("n2_neuts_raw"));   
    if(in.getCut("n3_neuts_raw") && !n3_neuts_raw)
      n3_neuts_raw=new TCutG(*in.getCut("n3_neuts_raw"));
    if(in.getCut("n4_neuts_raw") && !n4_neuts_raw)
      n4_neuts_raw=new TCutG(*in.getCut("n4_neuts_raw"));
    if(in.getCut("n5_neuts_raw") && !n5_neuts_raw)
      n5_neuts_raw=new TCutG(*in.getCut("n5_neuts_raw"));
    if(in.getCut("n6_neuts_raw") && !n6_neuts_raw)
      n6_neuts_raw=new TCutG(*in.getCut("n6_neuts_raw"));   
    if(in.getCut("n7_neuts_raw") && !n7_neuts_raw)
      n7_neuts_raw=new TCutG(*in.getCut("n7_neuts_raw"));
    if(in.getCut("n8_neuts_raw") && !n8_neuts_raw)
      n8_neuts_raw=new TCutG(*in.getCut("n8_neuts_raw"));
    if(in.getCut("n9_neuts_raw") && !n9_neuts_raw)
      n9_neuts_raw=new TCutG(*in.getCut("n9_neuts_raw"));
    if(in.getCut("prots1") && !prots1)
      prots1=new TCutG(*in.getCut("prots1"));
    if(in.getCut("alphas") && !alphas)
      alphas=new TCutG(*in.getCut("alphas"));
    if(in.getCut("prots2") && !prots2)
      prots2=new TCutG(*in.getCut("prots2"));
    if(in.getCut("s1_delayed_rf") && !s1_delayed_rf)
      s1_delayed_rf=new TCutG(*in.getCut("s1_delayed_rf"));
    if(in.getCut("s2_delayed_rf") && !s2_delayed_rf)
      s2_delayed_rf=new TCutG(*in.getCut("s2_delayed_rf"));    
     
   
  }


  void SetEvtGates(){
    //   double nX[8]={13,13,24,38.5,48,75,75,13};
    double nX[8]={13,13,20,32,40,65,65,13};
    double n0Y[8]={100,1000,650,320,251,126,100,100};
    double n1Y[8]={90,1000,440,226,167,126,90,90};
    double n2Y[8]={88,1000,308,183,126,100,88,88};
    double n3Y[8]={80,1000,593,326,217,142,80,80};
    double n4Y[8]={100,1000,627,300,217,117,90,90};
    double n5Y[8]={100,1000,650,320,251,126,100,100};
    double n6Y[8]={130,1000,635,326,234,134,130,130};
    double n7Y[8]={100,1000,650,320,251,126,100,100};
    double n8Y[8]={100,1000,320,190,129,101,100,100};
    double n9Y[8]={88,1000,460,234,192,100,88,88};
    
    if(!n0_evt)n0_evt=new TCutG("n0_evt",8,nX,n0Y);
    if(!n1_evt)n1_evt=new TCutG("n1_evt",8,nX,n1Y);
    if(!n2_evt)n2_evt=new TCutG("n2_evt",8,nX,n2Y);
    if(!n3_evt)n3_evt=new TCutG("n3_evt",8,nX,n3Y);
    if(!n4_evt)n4_evt=new TCutG("n4_evt",8,nX,n4Y);
    if(!n5_evt)n5_evt=new TCutG("n5_evt",8,nX,n5Y);
    if(!n6_evt)n6_evt=new TCutG("n6_evt",8,nX,n6Y);
    if(!n7_evt)n7_evt=new TCutG("n7_evt",8,nX,n7Y);
    if(!n8_evt)n8_evt=new TCutG("n8_evt",8,nX,n8Y);
    if(!n9_evt)n9_evt=new TCutG("n9_evt",8,nX,n9Y);


    


}
  
}



