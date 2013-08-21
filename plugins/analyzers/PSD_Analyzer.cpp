#include "PSD_Analyzer.hpp"

namespace psd{


 TFile* rootfile;
  
  //declare histograms here
  sak::Histogram1D *hrftime;
  sak::Hist2D *hrftime_n;
  sak::Histogram1D *hrftime_cal;
  sak::Hist1D *h_ndetMult;
  sak::Hist1D *h_ndetMult_ngated;
  sak::Hist1D *h_ndetMult_npgated;
  sak::Hist2D *s2_tvrf;
  sak::Hist2D *s2_e_v_theta;
  sak::Histogram2D *hPSDq_n[10];
  sak::Histogram2D *hPSD_n_[10];
  sak::Histogram2D *  hTrel_n[10];
  sak::Hist2D* hpede;
  sak::Hist2D* hpede_2;
    



NeutAnalyzer::NeutAnalyzer()
{

}



void NeutAnalyzer::Begin(){
  int idx=0;

  rootfile=new TFile("psd_analysis.root","RECREATE");
  rootfile->mkdir("rftime");
  rootfile->mkdir("trel");
  rootfile->mkdir("neut_PSD");
  rootfile->mkdir("protons");

  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[arb. units]",4096,0,4095);
  hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",1270,0,1269);
  hrftime_n=new sak::Histogram2D("hrftime_n","Detector","rftime[ns]",17,0,16,1270,0,1269);


  rootfile->cd("protons");
  hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
  hpede_2=new sak::Hist2D("hpEdE_2","E [MeV]","dE [MeV]",64,0,30,64,0,30);
  s2_tvrf=new sak::Hist2D("s2_tvrf","rftime[ns]","TDC2",128,640,820,256,1.,4096.);
  s2_e_v_theta =new sak::Hist2D("s2_e_v_theta","Theta","E ]MeV]",32,10,50,32,0.,15.);
  

  for(int i=0;i<10;i++){  
    rootfile->cd("neut_PSD");
    hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",1024,50,4096,1024,50,4096);
    hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,1024,50,4096);
    rootfile->cd("trel");
    hTrel_n[i]=new sak::Histogram2D(Form("hTrel_n%d",i),"Trel","E",512,2,2500,512,50,2500);
  }
  
  h_ndetMult=new sak::Hist1D("h_ndetmult","mult",11,0,10);
  h_ndetMult_ngated=new sak::Hist1D("h_ndetmult_ngated","mult",11,0,10);
  h_ndetMult_npgated=new sak::Hist1D("h_ndetmult_npgated","mult",11,0,10);


  rootfile->cd();
  
}

void NeutAnalyzer::Process(){ 
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
    

  }
  

  
  ApplyCalibrations();
  //Fill calpar.Histograms below this line
  /*************************************************************/
  h_ndetMult->Fill(Narray.fMult);


  if(si_[1].front.fMult>0&&si_[0].front.fE[0]){
    prot_dE=si_[1].front.fE[0];
    prot_E=si_[0].front.fE[0]+prot_dE;
    hpede->Fill(prot_E,prot_dE);
    hpede_2->Fill(prot_E,prot_dE);
    prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
    s2_e_v_theta->Fill(prot_theta,prot_E);
  }
 
  
  
  hrftime_cal->Fill(rftime[0].fT);
  
  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);
    if(neut[i].fQ_long>0){
      hrftime_n->Fill(i,rftime[0].fT);
    }


  }
  s2_tvrf->Fill(rftime[0].fT,si_cluster_[1].fT[0]);
  
}

  
void NeutAnalyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}


  void  analysis(const char * infile, const char * config){
    NeutAnalyzer a;
    a.Init(infile);
    a.LoadVariableFile(config);
    a.SetCalibrations();
    a.Loop();
    
    
    
  }
  
}



