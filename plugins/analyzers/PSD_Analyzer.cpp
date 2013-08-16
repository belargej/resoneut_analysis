#include "PSD_Analyzer.hpp"


PSD_Analyzer::PSD_Analyzer()
{

}

void PSD_Analyzer::LoadGates(const std::string& input){
  
  sak::LoadCuts in(input.c_str());
  
  if(in.getCut("n0_neuts") && !neuts_n0){
    neuts_n0=new sak::Gate(&neut[0].fPSD,&neut[0].fQ_long,*in.getCut("n0_neuts"));
    allneuts.push_back(*neuts_n0);
    }
  if(in.getCut("n1_neuts") && !neuts_n1){
    neuts_n1=new sak::Gate(&neut[1].fPSD,&neut[1].fQ_long,*in.getCut("n1_neuts"));
    allneuts.push_back(*neuts_n1);
  }
  if(in.getCut("n2_neuts") && !neuts_n2){
    neuts_n2=new sak::Gate(&neut[2].fPSD,&neut[2].fQ_long,*in.getCut("n2_neuts"));
    allneuts.push_back(*neuts_n2);
  }
  if(in.getCut("n3_neuts") && !neuts_n3){
    neuts_n3=new sak::Gate(&neut[3].fPSD,&neut[3].fQ_long,*in.getCut("n3_neuts"));
    allneuts.push_back(*neuts_n3);
    
  }
  if(in.getCut("n4_neuts") && !neuts_n4){
    neuts_n4=new sak::Gate(&neut[4].fPSD,&neut[4].fQ_long,*in.getCut("n4_neuts"));
    allneuts.push_back(*neuts_n4);
  }
  if(in.getCut("n5_neuts") && !neuts_n5){
    neuts_n5=new sak::Gate(&neut[5].fPSD,&neut[5].fQ_long,*in.getCut("n5_neuts"));
    allneuts.push_back(*neuts_n5);
  }
  if(in.getCut("n6_neuts") && !neuts_n6){
    neuts_n6=new sak::Gate(&neut[6].fPSD,&neut[6].fQ_long,*in.getCut("n6_neuts"));
    
    allneuts.push_back(*neuts_n6);
  }
  if(in.getCut("n7_neuts") && !neuts_n7){
    neuts_n7=new sak::Gate(&neut[7].fPSD,&neut[7].fQ_long,*in.getCut("n7_neuts"));
    allneuts.push_back(*neuts_n7);
  }
  if(in.getCut("n8_neuts") && !neuts_n8){
    neuts_n8=new sak::Gate(&neut[8].fPSD,&neut[8].fQ_long,*in.getCut("n8_neuts"));
    allneuts.push_back(*neuts_n8);
  }
  if(in.getCut("n9_neuts") && !neuts_n9){
    neuts_n9=new sak::Gate(&neut[9].fPSD,&neut[9].fQ_long,*in.getCut("n9_neuts"));
    allneuts.push_back(*neuts_n9);    
  }
  if(in.getCut("prots1") && !prots1){
    prots1=new sak::Gate(&prot_E,&prot_dE,*in.getCut("prots1"));
    }
  if(in.getCut("alphas") && !alphas){
    alphas=new sak::Gate(&prot_E,&prot_dE,*in.getCut("alphas"));
    }


}





//override Loop to sort data in a fashion different from the method
//in RN_Analyzer

/*
void PSD_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}
*/
void PSD_Analyzer::Begin(){
  int idx=0;

  rootfile=new TFile("psd_analysis.root","RECREATE");
  rootfile->mkdir("rftime");
  rootfile->mkdir("trel");
  rootfile->mkdir("neut_PSD");
  rootfile->mkdir("protons");

  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[arb. units]",256,2050,2650);
  hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[arb. units]",256,2050,2650);
  hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",128,1,90);
  hrftime_prots=new sak::Histogram1D("hrftime_prots","rftime[ns]",128,1,90);
  hrftime_allneut_cal=new sak::Histogram1D("hrftime_allneut_cal","rftime[ns]",128,1,90); 
  hrftime_allneut_cal_p=new sak::Histogram1D("hrftime_allneut_cal_p","rftime[ns]",128,1,90); 
  hrftime_allneut_cal_proton=new sak::Histogram1D("hrftime_allneut_cal_protons","rftime[ns]",128,1,90); 
  hrftime_allneut_cal_alphas=new sak::Histogram1D("hrftime_allneut_cal_alphas","rftime[ns]",128,1,90); 
  hrftime_n=new sak::Histogram2D("hrftime_n","Detector","rftime[ns]",17,0,16,128,1,90);
  hrftime_gated_n=new sak::Histogram2D("hrftime_gated_n","Detector","rftime[ns]",17,0,16,512,1,90);
  hrftime_raw_n =new sak::Histogram2D("hrftime_raw_n","Detector","rftime[ns]",17,0,16,256,2050,2650);
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
    rootfile->cd("trel");
    hTrel_n[i]=new sak::Histogram2D(Form("hTrel_n%d",i),"Trel","E",512,2,2500,512,50,2500);
  }
  
  rootfile->cd();
  
}

void PSD_Analyzer::Process(){
 

  prot_E=0;
  prot_dE=0;
  prot_theta=0;

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

  if(si_[1].front.fMult>0&&si_[0].front.fE[0]){
      prot_dE=si_[1].front.fE[0];
      prot_E=si_[0].front.fE[0]+prot_dE;
      hpede->Fill(prot_E,prot_dE);
      hpede_2->Fill(prot_E,prot_dE);
      prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
      s2_e_v_theta->Fill(prot_theta,prot_E);
  }
 
  

  hrftime_cal->Fill(rftime[0].fT);
  if(sak::OrCheck(allneuts)){
    hrftime_allneut_cal->Fill(rftime[0].fT);
    s2_tvrf_neut_gated->Fill(rftime[0].fT,si_cluster_[1].fT[0]);
    s2_e_v_theta_ngated->Fill(prot_theta,prot_E);
    if(si_[1].front.fMult>0 && si_[0].front.fMult>0){
      hpede_ngated->Fill(prot_E,prot_dE);
      hrftime_allneut_cal_p->Fill(rftime[0].fT);
    }

    if(prots1)
      if(prots1->Check())
	hrftime_allneut_cal_proton->Fill(rftime[0].fT);
  
  
    if(alphas)
      if(alphas->Check())
	hrftime_allneut_cal_alphas->Fill(rftime[0].fT);
  }

  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);
    if(neut[i].fQ_long>0){
      hrftime_n->Fill(i,rftime[0].fT);
      if(sak::OrCheck(allneuts)){
	hrftime_gated_n->Fill(i,rftime[0].fT);
      }


    }
  }

 



  if(prots1)
    if(prots1->Check()){
      s2_e_v_theta_protons->Fill(prot_theta,prot_E);
      hrftime_prots->Fill(rftime[0].fT);
      hPSD_n1_prots->Fill(neut[1].fPSD,neut[1].fQ_long);
    }

  //temp analysis--since we didnt have the backchannels plugged in
  // for(int i=0;i<16;i++){
  //}

  s2_tvrf->Fill(rftime[0].fT,si_cluster_[1].fT[0]);




}

  
void PSD_Analyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}
