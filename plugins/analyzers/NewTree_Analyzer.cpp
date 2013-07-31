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
    rawtree->Branch(Form("%s.",si_a.Name().c_str()),"RN_S2Detector",&si_a);
    rawtree->Branch(Form("%s.",si_b.Name().c_str()),"RN_S2Detector",&si_b);
    rawtree->Branch(Form("%s.",rftime.Name().c_str()),"RN_RFTime",&rftime);
    if(calibrated){
      caltree->Branch(Form("%s.",si_a.Name().c_str()),"RN_S2Detector",&si_a);
      caltree->Branch(Form("%s.",si_b.Name().c_str()),"RN_S2Detector",&si_b);
      caltree->Branch(Form("%s.",rftime.Name().c_str()),"RN_RFTime",&rftime);
    }
  }
  else{
    rawtree->Branch(Form("%s",si_a.Name().c_str()),"RN_S2Detector",&si_a,16000,0);
    rawtree->Branch(Form("%s",si_b.Name().c_str()),"RN_S2Detector",&si_b,16000,0);
    rawtree->Branch(Form("%s",rftime.Name().c_str()),"RN_RFTime",&rftime,16000,0);
    if(calibrated){
      caltree->Branch(Form("%s",si_a.Name().c_str()),"RN_S2Detector",&si_a,16000,0);
      caltree->Branch(Form("%s",si_b.Name().c_str()),"RN_S2Detector",&si_b,16000,0);
      caltree->Branch(Form("%s",rftime.Name().c_str()),"RN_RFTime",&rftime,16000,0);

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
    si_a.ApplyCalibrations();
    si_b.ApplyCalibrations();
    rftime.ApplyCalibrations();
    
    caltree->Fill();
  }
  
  
}
void NewTree_Analyzer::WriteOut(){
  rootfile->Write();
  rootfile->Close();

}
