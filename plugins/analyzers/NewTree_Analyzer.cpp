#include "NewTree_Analyzer.hpp"


NewTree_Analyzer::NewTree_Analyzer()
{

}

void NewTree_Analyzer::InitTree(std::string filename,Int_t calibrate){
  calibrated=calibrate;
  rootfile=new TFile(filename.c_str(),"RECREATE");
  rawtree=new TTree("rawpar","rawpar");
  if(calibrated)
    caltree=new TTree("calpar","calpar");
  std::string split="";
  std::cout<<"split branches?(y/n)"<<std::endl;
  std::cin>>split;
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if(split=="y"){
      rawtree->Branch(Form("%s.",(*it).Name().c_str()),"RN_NeutDetector",&(*it));
      if(calibrated)
	caltree->Branch(Form("%s.",(*it).Name().c_str()),"RN_NeutDetector",&(*it));
    }
    else{
      rawtree->Branch(Form("%s",(*it).Name().c_str()),"RN_NeutDetector",&(*it),16000,0);
      if(calibrated)
	caltree->Branch(Form("%s",(*it).Name().c_str()),"RN_NeutDetector",&(*it),16000,0);
    }
  }
  if(split=="y"){
    rawtree->Branch(Form("%s.",si_[0].Name().c_str()),"RN_S2Detector",&si_[0]);
    rawtree->Branch(Form("%s.",si_[1].Name().c_str()),"RN_S2Detector",&si_[1]);
    rawtree->Branch(Form("%s.",rftime[0].Name().c_str()),"RN_RFTime",&rftime[0]);
    if(calibrated){
      caltree->Branch(Form("%s.",si_[0].Name().c_str()),"RN_S2Detector",&si_[0]);
      caltree->Branch(Form("%s.",si_[1].Name().c_str()),"RN_S2Detector",&si_[1]);
      caltree->Branch(Form("%s.",rftime[0].Name().c_str()),"RN_RFTime",&rftime[0]);
    }
  }
  else{
    rawtree->Branch(Form("%s",si_[0].Name().c_str()),"RN_S2Detector",&si_[0],16000,0);
    rawtree->Branch(Form("%s",si_[1].Name().c_str()),"RN_S2Detector",&si_[1],16000,0);
    rawtree->Branch(Form("%s",rftime[0].Name().c_str()),"RN_RFTime",&rftime[0],16000,0);
    if(calibrated){
      caltree->Branch(Form("%s",si_[0].Name().c_str()),"RN_S2Detector",&si_[0],16000,0);
      caltree->Branch(Form("%s",si_[1].Name().c_str()),"RN_S2Detector",&si_[1],16000,0);
      caltree->Branch(Form("%s",rftime[0].Name().c_str()),"RN_RFTime",&rftime[0],16000,0);

    }
  }

  
}

void NewTree_Analyzer::Loop(){
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }
}

void NewTree_Analyzer::Process(){
  rawtree->Fill();
  if(calibrated){
    
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      (*it).ApplyCalibrations();
    }
    int cref=0;
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      (*it).ApplyCalibrations();
      if(cref<si_.size())
	si_cluster_[cref].ReconstructClusters(*it);
      cref++;
    }
    for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
      (*it).ApplyCalibrations();
    }
	
    caltree->Fill();
  }
  
  
}
void NewTree_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
