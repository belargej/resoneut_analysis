#include "NewTree_Analyzer.hpp"


NewTree_Analyzer::NewTree_Analyzer()
{

}

void NewTree_Analyzer::InitTree(std::string filename,std::string treename){
  rootfile=new TFile(filename.c_str(),"RECREATE");
  tree=new TTree(treename.c_str(),treename.c_str());
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_NeutDetector",&(*it),16000,0);
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Detector",&(*it),16000,0);
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Cluster",&(*it),16000,0);
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_RFTime",&(*it),16000,0);
  
}

void NewTree_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}

void NewTree_Analyzer::Process(){
  ApplyCalibrations();
  tree->Fill();
  
}

  

void NewTree_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();
  
}
