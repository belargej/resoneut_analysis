#include "NewTree_Analyzer.hpp"
#include "../../include/RN_Root.hpp"


NewTree_Analyzer::NewTree_Analyzer()
{

}

bool NewTree_Analyzer::Begin(){

  if(!rootfile){
    std::cout<<"output file has not been created"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(!newtree){
    std::cout<<"new tree has not been created"<<std::endl;   
    exit(EXIT_FAILURE);
  }
 
  //for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++)
  //tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_NeutDetector",&(*it));
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++)
    newtree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Detector",&(*it));
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++)
    newtree->Branch(Form("%s.",(*it).Name().c_str()),"RN_S2Cluster",&(*it));
  //for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++)
  // tree->Branch(Form("%s.",(*it).Name().c_str()),"RN_RFTime",&(*it));
  //tree->Branch("Narray.","RN_NeutDetectorArray",&Narray);
  newtree->Branch("ic.","RN_IonChamber",&ic);
  // tree->Branch("nai.","RN_NaIArray",&nai);
  return 1;
}


bool NewTree_Analyzer::Process(){

  newtree->Fill();
  return 1;
}

void NewTree_Analyzer::ResetGlobals(){
}

bool NewTree_Analyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  return 1;
}
