#include "PSD_Analyzer.hpp"


PSD_Analyzer::PSD_Analyzer()
{

}

void PSD_Analyzer::LoadGates(const char *a){
  sak::LoadCuts in(a);

  sak::Gate n0_neuts(&neut[0].fQ_long,&neut[0].fQ_short,*in.getCut("n0_neuts"));
  sak::Gate n1_neuts(&neut[1].fQ_long,&neut[1].fQ_short,*in.getCut("n1_neuts"));
  sak::Gate n2_neuts(&neut[2].fQ_long,&neut[2].fQ_short,*in.getCut("n2_neuts"));
  sak::Gate n3_neuts(&neut[3].fQ_long,&neut[3].fQ_short,*in.getCut("n3_neuts"));
  sak::Gate n4_neuts(&neut[4].fQ_long,&neut[4].fQ_short,*in.getCut("n4_neuts"));
  sak::Gate n5_neuts(&neut[5].fQ_long,&neut[5].fQ_short,*in.getCut("n5_neuts"));
  sak::Gate n6_neuts(&neut[6].fQ_long,&neut[6].fQ_short,*in.getCut("n6_neuts"));
  sak::Gate n7_neuts(&neut[7].fQ_long,&neut[7].fQ_short,*in.getCut("n7_neuts"));
  sak::Gate n8_neuts(&neut[8].fQ_long,&neut[8].fQ_short,*in.getCut("n8_neuts"));
  sak::Gate n9_neuts(&neut[9].fQ_long,&neut[9].fQ_short,*in.getCut("n9_neuts"));

  hrftime_allneut->ApplyGate(n0_neuts);
  hrftime_allneut->ApplyGate(n1_neuts);
  hrftime_allneut->ApplyGate(n2_neuts);
  hrftime_allneut->ApplyGate(n3_neuts);
  hrftime_allneut->ApplyGate(n4_neuts);
  hrftime_allneut->ApplyGate(n5_neuts);
  hrftime_allneut->ApplyGate(n6_neuts);
  hrftime_allneut->ApplyGate(n7_neuts);
  hrftime_allneut->ApplyGate(n8_neuts);
  hrftime_allneut->ApplyGate(n9_neuts);
  hTrel_n0->ApplyGate(n0_neuts);
  hTrel_n1->ApplyGate(n1_neuts);
  hTrel_n2->ApplyGate(n2_neuts);
  hTrel_n3->ApplyGate(n3_neuts);
  hTrel_n4->ApplyGate(n4_neuts);
  hTrel_n5->ApplyGate(n5_neuts);
  hTrel_n6->ApplyGate(n6_neuts);
  hTrel_n7->ApplyGate(n7_neuts);
  hTrel_n8->ApplyGate(n8_neuts);
  hTrel_n9->ApplyGate(n9_neuts);
 


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
    
  rootfile->mkdir("trel");
  rootfile->cd("trel");
  hTrel_n0=new sak::Histogram2D("hTrel_n0","Trel","E",512,2,2500,512,50,2500);
  hTrel_n1=new sak::Histogram2D("hTrel_n1","Trel","E",512,2,2500,512,50,2500);
  hTrel_n2=new sak::Histogram2D("hTrel_n2","Trel","E",512,2,2500,512,50,2500);
  hTrel_n3=new sak::Histogram2D("hTrel_n3","Trel","E",512,2,2500,512,50,2500);
  hTrel_n4=new sak::Histogram2D("hTrel_n4","Trel","E",512,2,2500,512,50,2500);
  hTrel_n5=new sak::Histogram2D("hTrel_n5","Trel","E",512,2,2500,512,50,2500);
  hTrel_n6=new sak::Histogram2D("hTrel_n6","Trel","E",512,2,2500,512,50,2500);
  hTrel_n7=new sak::Histogram2D("hTrel_n7","Trel","E",512,2,2500,512,50,2500);
  hTrel_n8=new sak::Histogram2D("hTrel_n8","Trel","E",512,2,2500,512,50,2500);
  hTrel_n9=new sak::Histogram2D("hTrel_n9","Trel","E",512,2,2500,512,50,2500);

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

  if(hTrel_n0->Check())hTrel_n0->Fill(neut[0].fTrel,neut[0].fQ_long);
  if(hTrel_n1->Check())hTrel_n1->Fill(neut[1].fTrel,neut[1].fQ_long);
  if(hTrel_n2->Check())hTrel_n2->Fill(neut[2].fTrel,neut[2].fQ_long);
  if(hTrel_n3->Check())hTrel_n3->Fill(neut[3].fTrel,neut[3].fQ_long);
  if(hTrel_n4->Check())hTrel_n4->Fill(neut[4].fTrel,neut[4].fQ_long);
  if(hTrel_n5->Check())hTrel_n5->Fill(neut[5].fTrel,neut[5].fQ_long);
  if(hTrel_n6->Check())hTrel_n6->Fill(neut[6].fTrel,neut[6].fQ_long);
  if(hTrel_n7->Check())hTrel_n7->Fill(neut[7].fTrel,neut[7].fQ_long);
  if(hTrel_n8->Check())hTrel_n8->Fill(neut[8].fTrel,neut[8].fQ_long);
  if(hTrel_n9->Check())hTrel_n9->Fill(neut[9].fTrel,neut[9].fQ_long);

  hrftime->Fill(rftime[0].fT);
  if(hrftime_allneut->OrCheck())hrftime_allneut->Fill(rftime[0].fT);
}
void PSD_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
