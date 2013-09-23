#include "NewTree_Analyzer.hpp"


NewTree_Analyzer::NewTree_Analyzer()
{

}

void NewTree_Analyzer::InitTree(std::string filename,std::string treename){
  rootfile=new TFile(filename.c_str(),"RECREATE");
  tree=new TTree(treename.c_str(),treename.c_str());
  //for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++)
  //tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_NeutDetector",&(*it));
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Detector",&(*it));
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Cluster",&(*it));
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++)
    tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_RFTime",&(*it));
  tree->Branch("Narray.","RN_NeutDetectorArray",&Narray);
  tree->Branch("ic.","RN_IonChamber",&ic);
  // tree->Branch("nai.","RN_NaIArray",&nai);
}


void NewTree_Analyzer::Process(){

  tree->Fill();
  
}

  

void NewTree_Analyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}
