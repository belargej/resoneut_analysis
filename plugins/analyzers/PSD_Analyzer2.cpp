#include "PSD_Analyzer2.hpp"
namespace psd{


NeutAnalyzer2::NeutAnalyzer2()
{

}

void NeutAnalyzer2::LoadGates(const std::string& input){
  
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




}


void NeutAnalyzer2::Begin(){
  int idx=0;

  rootfile=new TFile("psd_analysis2.root","RECREATE");
  rootfile->mkdir("rftime");
  rootfile->mkdir("neut_timing");
  rootfile->mkdir("neut_PSD");
  rootfile->mkdir("protons");

  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[arb. units]",256,2050,2650);
  hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[arb. units]",4096,0,4095);
  hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",128,630,830);
  hrftime_prots=new sak::Histogram1D("hrftime_prots","rftime[ns]",128,630,830);
  hrftime_allneut_cal=new sak::Histogram1D("hrftime_allneut_cal","rftime[ns]",128,630,830); 
  hrftime_allneut_cal_p=new sak::Histogram1D("hrftime_allneut_cal_p","rftime[ns]",128,630,830); 
  hrftime_allneut_cal_proton=new sak::Histogram1D("hrftime_allneut_cal_protons","rftime[ns]",128,630,830); 
  hrftime_allneut_cal_alphas=new sak::Histogram1D("hrftime_allneut_cal_alphas","rftime[ns]",128,630,830); 
  hrftime_n=new sak::Histogram2D("hrftime_n","Detector","rftime[ns]",17,0,16,128,630,830);
  hrftime_gated_n=new sak::Histogram2D("hrftime_gated_n","Detector","rftime[ns]",17,0,16,128,630,830);
  hrftime_raw_n =new sak::Histogram2D("hrftime_raw_n","Detector","rftime[ns]",17,0,16,4096,0,4095);
  rootfile->cd("protons");
  hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
  hpede_2=new sak::Hist2D("hpEdE_2","E [MeV]","dE [MeV]",64,0,30,64,0,30);
  hPSD_n1_prots=new sak::Hist2D("hPSD_n1_prots","fPSD","fQ_long",256,0.,1.,1024,50,4096);
  hpede_ngated=new sak::Hist2D("hpEdE_ngated","E [MeV]","dE [MeV]",256,0,32,256,0,32);
 
  s2_tvrf_neut_gated=new sak::Hist2D("s2_tvrf_neut_gated","rftime[ns]","TDC2",128,640,820,256,1.,4096.);
  s2_tvrf=new sak::Hist2D("s2_tvrf","rftime[ns]","TDC2",128,640,820,256,1.,4096.);
  s2_e_v_theta_ngated=new sak::Hist2D("s2_e_v_theta_neut_gated","Theta [deg]","E[MeV]",32,10,50,32,0.,15.);
  s2_e_v_theta =new sak::Hist2D("s2_e_v_theta","Theta","E ]MeV]",32,10,50,32,0.,15.);
  s2_e_v_theta_protons =new sak::Hist2D("s2_e_v_theta_protons","Theta","E ]MeV]",32,10,50,32,0.,15.);

  for(int i=0;i<10;i++){  
    rootfile->cd("neut_PSD");
    hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",1024,50,4096,1024,50,4096);
    hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,1024,50,4096);
    rootfile->cd("neut_timing");
    hTrel_n[i]=new sak::Histogram2D(Form("hTrel_n%d",i),"Trel","E",512,2,2500,512,50,2500);
    hEvT_n[i]=new sak::Histogram2D(Form("hEvT_n%d",i),"rftime[ns]","E[arb.units]",128,630,830,2048,50,1000);
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
    hTrel_n[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
    hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
    
    if(neut[i].fQ_long>0){
      hrftime_raw_n->Fill(i,rftime[0].fT);
    }
  }


  ApplyCalibrations();
  //Fill calpar.Histograms below this line
  /*************************************************************/
  h_ndetMult->Fill(Narray.fMult);

  int orcheck=(n0_neuts->IsInside(neut[0].fPSD,neut[0].fQ_long) || 
	       n1_neuts->IsInside(neut[1].fPSD,neut[1].fQ_long) ||
	       n2_neuts->IsInside(neut[2].fPSD,neut[2].fQ_long) ||
	       n3_neuts->IsInside(neut[3].fPSD,neut[3].fQ_long) ||
	       n4_neuts->IsInside(neut[4].fPSD,neut[4].fQ_long) ||
	       n5_neuts->IsInside(neut[5].fPSD,neut[5].fQ_long) ||
	       n6_neuts->IsInside(neut[6].fPSD,neut[6].fQ_long) ||
	       n7_neuts->IsInside(neut[7].fPSD,neut[7].fQ_long) ||
	       n8_neuts->IsInside(neut[8].fPSD,neut[8].fQ_long) ||
	       n9_neuts->IsInside(neut[9].fPSD,neut[9].fQ_long)
	       );
  int protcheck=0;
  int alphacheck=0;

  if(si_[1].front.fMult>0&&si_[0].front.fE[0]){
      prot_dE=si_[1].front.fE[0];
      prot_E=si_[0].front.fE[0]+prot_dE;
      hpede->Fill(prot_E,prot_dE);
      hpede_2->Fill(prot_E,prot_dE);
      prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
      s2_e_v_theta->Fill(prot_theta,prot_E);
  }

  if(prots1)
    protcheck=prots1->IsInside(prot_E,prot_dE);
  if(alphas)
    alphacheck=alphas->IsInside(prot_E,prot_dE);
  
  
  hrftime_cal->Fill(rftime[0].fT);
  s2_tvrf->Fill(rftime[0].fT,si_cluster_[1].fT[0]);
  if(orcheck){
    hrftime_allneut_cal->Fill(rftime[0].fT);
    s2_tvrf_neut_gated->Fill(rftime[0].fT,si_cluster_[1].fT[0]);
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
      hrftime_prots->Fill(rftime[0].fT);
      hPSD_n1_prots->Fill(neut[1].fPSD,neut[1].fQ_long);
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
      if(orcheck){
	hrftime_gated_n->Fill(i,rftime[0].fT);
	hEvT_n[i]->Fill(rftime[0].fT,neut[i].fQ_long);
      }
    }
  }
}

  
void NeutAnalyzer2::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}




  void  analysis2(const char * infile, const char * config, const char * cuts){
    NeutAnalyzer2 a;
    a.Init(infile);
    a.LoadVariableFile(config);
    a.SetCalibrations();
    a.LoadGates(cuts);
    a.Loop();

    
    
  }

}
