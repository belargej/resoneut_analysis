////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
///RN_Analyzer functions like a basic MakeClass for the DataTree TTree
///which has the NTUPLE form: hits,GeoAddress[hits],Channel[hits],Value[hits]
////////////////////////////////////////////////////////////////////////


//RN_Analyzer headers.
#include "RN_Analyzer.hpp"

using namespace std;


RN_Analyzer::RN_Analyzer():calibrated(0),
			   ic("ic")
			   //si_a("si_a",16,16),
			   // si_b("si_b",16,16),
			   //si_cluster_b("si_cluster_b",16),
			   //rftime("rftime")

{

}

void RN_Analyzer::SetCalibrations(){


  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).SetCalibrations(DetVar);
  }

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
    (*it).SetCalibrations(DetVar);
  }
 
  ic.SetCalibrations(DetVar);


}

void RN_Analyzer::ApplyCalibrations(){


  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).ApplyCalibrations();
  }

  int cref=0;
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).ApplyCalibrations();
    if(cref<si_cluster_.size())
      si_cluster_[cref].ReconstructClusters(*it);
    cref++;
  }
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
    (*it).ApplyCalibrations();
  }
  
  ic.ApplyCalibrations();

}

void RN_Analyzer::Init(TString rootfile)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
  si_.push_back(RN_S2Detector("si_a",16,16));
  si_.push_back(RN_S2Detector("si_b",16,16));
  si_cluster_.push_back(RN_S2Cluster("si_cluster_a",16));
  si_cluster_.push_back(RN_S2Cluster("si_cluster_b",16));
  rftime.push_back(RN_RFTime("rftime"));
  neut.push_back(RN_NeutDetector("neut0",4,1));
  neut.push_back(RN_NeutDetector("neut1",4,4));
  neut.push_back(RN_NeutDetector("neut2",4,5));
  neut.push_back(RN_NeutDetector("neut3",4,6));
  neut.push_back(RN_NeutDetector("neut4",4,7));
  neut.push_back(RN_NeutDetector("neut5",4,11));
  neut.push_back(RN_NeutDetector("neut6",4,14));
  neut.push_back(RN_NeutDetector("neut7",4,10));
  neut.push_back(RN_NeutDetector("neut8",4,12));
  neut.push_back(RN_NeutDetector("neut9",4,15));


   // Set object pointer
   ADC1 = 0;
   ADC2 = 0;
   ADC3 = 0;
   ADC4 = 0;
   ADC5 = 0;
   ADC6 = 0;
   TDC1 = 0;
   TDC2 = 0;
   TDC3 = 0;
   QDC1 = 0;
   QDC2 = 0;

   fChain=new TChain("DataTree");
   fChain->Add(rootfile);

   if(fChain->GetBranch("ADC1"))
     fChain->SetBranchAddress("ADC1", &ADC1, &b_ADC1);
   else
     std::cout<<"no ADC1 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("ADC2"))    
     fChain->SetBranchAddress("ADC2", &ADC2, &b_ADC2);
   else
     std::cout<<"no ADC2 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("ADC3"))     
     fChain->SetBranchAddress("ADC3", &ADC3, &b_ADC3);
   else
     std::cout<<"no ADC3 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("ADC4"))   
     fChain->SetBranchAddress("ADC4", &ADC4, &b_ADC4);
   else
     std::cout<<"no ADC4 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("ADC5"))   
     fChain->SetBranchAddress("ADC5", &ADC5, &b_ADC5);
   else
     std::cout<<"no ADC5 present, do not extract in channel mapping"<<std::endl;   if(fChain->GetBranch("ADC6"))   
     fChain->SetBranchAddress("ADC6", &ADC6, &b_ADC6);
   else
     std::cout<<"no ADC6 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("TDC1"))  
     fChain->SetBranchAddress("TDC1", &TDC1, &b_TDC1);
   else
     std::cout<<"no TDC1 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("TDC2"))   
     fChain->SetBranchAddress("TDC2", &TDC2, &b_TDC2);
   else
     std::cout<<"no TDC2 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("TDC3"))   
     fChain->SetBranchAddress("TDC3", &TDC3, &b_TDC3);
   else
     std::cout<<"no TDC3 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("QDC1"))   
     fChain->SetBranchAddress("QDC1", &QDC1, &b_QDC1);
   else
     std::cout<<"no QDC1 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("QDC2"))   
     fChain->SetBranchAddress("QDC2", &QDC2, &b_QDC2);
   else
     std::cout<<"no QDC2 present, do not extract in channel mapping"<<std::endl;
}




void RN_Analyzer::Loop(){
  Begin();
  Long64_t totentries= TotEntries();
  for (Long64_t i=0;i<totentries;i++){
    GetDetectorEntry(i);

    Process();
  }

  Terminate();

}




RN_Analyzer::~RN_Analyzer(){

}


int RN_Analyzer::GetDetectorEntry(Long64_t entry, Int_t getall){

  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).Reset();
  }

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).Reset();
  }
  
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
    (*it).Reset();
  }
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
    (*it).Reset();
  }

  ic.Reset();
  
  if(!GetEntry(entry,getall)){
    
    return 0;

  }
  if(entry%30000==0)std::cout<<entry<<std::endl;
  //ChanneltoDetector
  
  int idx=1; //neut detectors start from channel 1 in QDC
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if(QDC1->fCh[idx]>0){
      (*it).InsertPSDHit(QDC1->fCh[idx],QDC1->fCh[idx+16]);
      if(TDC1->fCh[idx]>0)
	(*it).InsertHit(QDC1->fCh[idx],TDC1->fCh[idx+16],0.);
    }
    idx++;
    
    

  }
  
  for(int j=0;j<16;j++){
    if(ADC2->fCh[j+16]>0)si_[0].front.InsertHit(ADC2->fCh[j+16],0,j);
    if(ADC3->fCh[j+16]>0)si_[1].front.InsertHit(ADC3->fCh[j+16],0,j);
    if(ADC2->fCh[j]>0)si_[0].back.InsertHit(ADC2->fCh[j],TDC2->fCh[j],j);
    if(ADC3->fCh[j]>0)si_[1].back.InsertHit(ADC3->fCh[j],TDC2->fCh[j+16],j);
  }
  if(TDC1->fCh[0]>0)rftime[0].InsertHit(TDC1->fCh[0]);
  
  for(int k=0;k<32;k++){
    if(ADC5->fCh[k]>0){
      ic.xgrid.InsertHit(ADC5->fCh[k],0,k);
    }
    if(ADC6->fCh[k]>0){
      ic.ygrid.InsertHit(ADC6->fCh[k],0,k);
    }
  }
  if(ADC4->fCh[13]>0){
    ic.fdE=ADC4->fCh[13];
  }

  
  RNArray::ReconstructTREL(neut);

  
}

