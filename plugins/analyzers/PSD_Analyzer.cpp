#include "PSD_Analyzer.hpp"


PSD_Analyzer::PSD_Analyzer()
{

}

void PSD_Analyzer::LoadGates(const char *a){
  sak::LoadCuts in(a);

  sak::Gate* neuts_n[10];
  
  for(int i=0;i<10;i++){
    // neuts_n[i]=new sak::Gate(&neut[i].fQ_long,&neut[i].fQ_short,*in.getCut(Form("n%d_neuts",i)));
    neuts_n[i]=new sak::Gate(&neut[i].fPSD,&neut[i].fQ_long,*in.getCut(Form("n%d_neuts",i)));
  }
  sak::Gate prots(&prot_E,&si_cluster_[1].fE[0],*in.getCut("prots"));

  
  hrftime_allneut->ApplyGate(*neuts_n[0]);
  hrftime_allneut->ApplyGate(*neuts_n[1]);
  hrftime_allneut->ApplyGate(*neuts_n[2]);
  hrftime_allneut->ApplyGate(*neuts_n[3]);
  hrftime_allneut->ApplyGate(*neuts_n[4]);
  hrftime_allneut->ApplyGate(*neuts_n[5]);
  hrftime_allneut->ApplyGate(*neuts_n[6]);
  hrftime_allneut->ApplyGate(*neuts_n[7]);
  hrftime_allneut->ApplyGate(*neuts_n[8]);
  hrftime_allneut->ApplyGate(*neuts_n[9]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[0]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[1]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[2]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[3]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[4]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[5]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[6]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[7]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[8]);
  hrftime_allneut_cal->ApplyGate(*neuts_n[9]);
  hrftime_allneut_cal_p->ApplyGate(prots);
  pEde_ngated->ApplyGate(*neuts_n[0]);
  pEde_ngated->ApplyGate(*neuts_n[1]);
  pEde_ngated->ApplyGate(*neuts_n[2]);
  pEde_ngated->ApplyGate(*neuts_n[3]);
  pEde_ngated->ApplyGate(*neuts_n[4]);
  pEde_ngated->ApplyGate(*neuts_n[5]);
  pEde_ngated->ApplyGate(*neuts_n[6]);
  pEde_ngated->ApplyGate(*neuts_n[7]);
  pEde_ngated->ApplyGate(*neuts_n[8]);
  pEde_ngated->ApplyGate(*neuts_n[9]);
  
  for(int i=0;i<10;i++){
    hTrel_n[i]->ApplyGate(*neuts_n[i]);
  }


}

void PSD_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}

void PSD_Analyzer::initHists(std::string output){
  int idx=0;
  rootfile=new TFile(output.c_str(),"RECREATE");
  rootfile->mkdir("rftime");
  rootfile->mkdir("trel");
  rootfile->mkdir("neut_PSD");
  rootfile->mkdir("ede");



  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[ns]",512,2050,2650);
  hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[ns]",512,2050,2650);
  hrftime_cal=new sak::Histogram1D("hrftime_cal","rftime[ns]",512,640,820);
  hrftime_allneut_cal=new sak::Histogram1D("hrftime_allneut_cal","rftime[ns]",512,640,820);
  hrftime_allneut_cal_p=new sak::Histogram1D("hrftime_allneut_cal_p","rftime[ns]",512,640,820);

  for(int i=0;i<10;i++){  
    rootfile->cd("neut_PSD");
    hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",512,50,2500,512,50,2500);
    hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,512,50,2500);
    rootfile->cd("trel");
    hTrel_n[i]=new sak::Histogram2D(Form("hTrel_n%d",i),"Trel","E",512,2,2500,512,50,2500);
  }
  

  rootfile->cd("ede");
  pEde=new sak::Histogram2D("pede","e[arb. units]","de[arb. units]",512,0,4096,512,0,4096);
  pEde_ngated=new sak::Histogram2D("pede_ngated","e[arb. units]","de[arb. units]",512,0,4096,512,0,4096);

  rootfile->cd();
  
  
}

void PSD_Analyzer::Process(){
 
  hrftime->Fill(rftime[0].fT);
  if(hrftime_allneut->OrCheck())hrftime_allneut->Fill(rftime[0].fT);





  for (int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    if(hTrel_n[i]->Check())hTrel_n[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
    hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
  }


  //fill histograms
  ApplyCalibrations();

  
  prot_E=si_cluster_[1].fE[0]+si_[0].front.fE[0];
  if(si_cluster_[1].fMult>0 && si_[0].front.fMult>0){
    pEde->Fill(prot_E,si_cluster_[1].fE[0]);
    if(pEde_ngated->OrCheck())pEde_ngated->Fill(si_cluster_[1].fE[0]+si_[0].front.fE[0],si_cluster_[1].fE[0]);
    
  }
  hrftime_cal->Fill(rftime[0].fT);
  if(hrftime_allneut_cal->OrCheck()){
    hrftime_allneut_cal->Fill(rftime[0].fT);
    if(hrftime_allneut_cal_p->Check()){
      hrftime_allneut_cal_p->Fill(rftime[0].fT);
    }
  }

  
  for(int i=0;i<10;i++){
    if(i>=neut.size())
      break;
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long);

  }


}


void PSD_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
