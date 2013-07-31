#include "PSD_Analyzer.hpp"


PSD_Analyzer::PSD_Analyzer()
{

}

void PSD_Analyzer::LoadGates(const char *a){
  sak::LoadCuts in(a);
  if(hrftime_allneut){
  hrftime_allneut->ApplyGate(sak::Gate(&neut[0].fQ_long,&neut[0].fQ_short,*in.getCut("n0_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[1].fQ_long,&neut[1].fQ_short,*in.getCut("n1_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[2].fQ_long,&neut[2].fQ_short,*in.getCut("n2_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[3].fQ_long,&neut[3].fQ_short,*in.getCut("n3_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[4].fQ_long,&neut[4].fQ_short,*in.getCut("n4_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[5].fQ_long,&neut[5].fQ_short,*in.getCut("n5_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[6].fQ_long,&neut[6].fQ_short,*in.getCut("n6_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[7].fQ_long,&neut[7].fQ_short,*in.getCut("n7_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[8].fQ_long,&neut[8].fQ_short,*in.getCut("n8_neuts")));
  hrftime_allneut->ApplyGate(sak::Gate(&neut[9].fQ_long,&neut[9].fQ_short,*in.getCut("n9_neuts")));
  }	     
}

void PSD_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}

void PSD_Analyzer::initHists(){
  int idx=0;
  rootfile=new TFile("Analyze.root","RECREATE");
  rootfile->mkdir("rftime");
  rootfile->cd("rftime");
  hrftime=new sak::Histogram1D("hrftime","rftime[ns]",512,2000,3000);
  hrftime_allneut=new sak::Histogram1D("hrftime_allneut","rftime[ns]",512,2000,3000);
  rootfile->mkdir("neut_PSD");
  rootfile->cd("neut_PSD");
  hPSD_n0=new sak::Histogram2D("hPSD_n0","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n1=new sak::Histogram2D("hPSD_n1","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n2=new sak::Histogram2D("hPSD_n2","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n3=new sak::Histogram2D("hPSD_n3","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n4=new sak::Histogram2D("hPSD_n4","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n5=new sak::Histogram2D("hPSD_n5","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n6=new sak::Histogram2D("hPSD_n6","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n7=new sak::Histogram2D("hPSD_n7","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n8=new sak::Histogram2D("hPSD_n8","fQ_long","fQ_short",512,50,2500,512,50,2500);
  hPSD_n9=new sak::Histogram2D("hPSD_n9","fQ_long","fQ_short",512,50,2500,512,50,2500);
    
  rootfile->cd();
  
  
}

void PSD_Analyzer::Process(){
  //fill histograms
  hPSD_n0->Fill(neut[0].fQ_long,neut[0].fQ_short);
  hPSD_n1->Fill(neut[1].fQ_long,neut[1].fQ_short);
  hPSD_n2->Fill(neut[2].fQ_long,neut[2].fQ_short);
  hPSD_n3->Fill(neut[3].fQ_long,neut[3].fQ_short);
  hPSD_n4->Fill(neut[4].fQ_long,neut[4].fQ_short);
  hPSD_n5->Fill(neut[5].fQ_long,neut[5].fQ_short);
  hPSD_n6->Fill(neut[6].fQ_long,neut[6].fQ_short);
  hPSD_n7->Fill(neut[7].fQ_long,neut[7].fQ_short);
  hPSD_n8->Fill(neut[8].fQ_long,neut[8].fQ_short);
  hPSD_n9->Fill(neut[9].fQ_long,neut[9].fQ_short);

  hrftime->Fill(rftime.fT);
  if(hrftime_allneut->OrCheck())hrftime_allneut->Fill(rftime.fT);
}
void PSD_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
