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
			   //nai("nai")
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
  
  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    (*it).SetCalibrations(DetVar);
  }


}

void RN_Analyzer::ApplyCalibrations(){

  int nref;
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).ApplyCalibrations();
  }
  
  Narray.ReconstructHits(neut);
  if(Narray.fMult>1)
    RNArray::ReconstructTREL(neut);


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

  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    (*it).ApplyCalibrations();
  }


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
  neut.reserve(16);
  rftime.reserve(2);
  si_.reserve(2);
  si_cluster_.reserve(2);
  nai.reserve(20);
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
  nai.push_back(RN_NaIDetector("nai_l1"));
  nai.push_back(RN_NaIDetector("nai_l2"));
  nai.push_back(RN_NaIDetector("nai_l3"));
  nai.push_back(RN_NaIDetector("nai_l4"));
  nai.push_back(RN_NaIDetector("nai_l5"));
  nai.push_back(RN_NaIDetector("nai_l6"));
  nai.push_back(RN_NaIDetector("nai_l7"));
  nai.push_back(RN_NaIDetector("nai_l8"));
  nai.push_back(RN_NaIDetector("nai_l9"));
  nai.push_back(RN_NaIDetector("nai_l10"));
  nai.push_back(RN_NaIDetector("nai_r1"));
  nai.push_back(RN_NaIDetector("nai_r2"));
  nai.push_back(RN_NaIDetector("nai_r3"));
  nai.push_back(RN_NaIDetector("nai_r4"));
  nai.push_back(RN_NaIDetector("nai_r5"));
  nai.push_back(RN_NaIDetector("nai_r6"));
  nai.push_back(RN_NaIDetector("nai_r7"));
  nai.push_back(RN_NaIDetector("nai_r8"));
  nai.push_back(RN_NaIDetector("nai_r9"));
  nai.push_back(RN_NaIDetector("nai_r10"));



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
   if(fChain->GetBranch("ADC7"))   
     fChain->SetBranchAddress("ADC7", &ADC7, &b_ADC7);
   else
     std::cout<<"no ADC7 present, do not extract in channel mapping"<<std::endl; 
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
   if(fChain->GetBranch("TDC4"))   
     fChain->SetBranchAddress("TDC4", &TDC4, &b_TDC4);
   else
     std::cout<<"no TDC4 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("QDC1"))   
     fChain->SetBranchAddress("QDC1", &QDC1, &b_QDC1);
   else
     std::cout<<"no QDC1 present, do not extract in channel mapping"<<std::endl;
   if(fChain->GetBranch("QDC2"))   
     fChain->SetBranchAddress("QDC2", &QDC2, &b_QDC2);
   else
     std::cout<<"no QDC2 present, do not extract in channel mapping"<<std::endl;
}




void RN_Analyzer::Loop(Long64_t start,Long64_t evnum){
  Long64_t totentries= TotEntries();    
  if (start!=0&&evnum!=0)
    if(start+evnum<totentries)
      totentries=start+evnum;
  
  Begin();
  for (Long64_t i=start;i<totentries;i++){
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

  Narray.Reset();

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

  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    (*it).Reset();
  }
  
  if(!GetEntry(entry,getall)){
    
    return 0;

  }
  if(entry%30000==0)std::cout<<entry<<std::endl;
  //ChanneltoDetector
  
  int idx=1; //neut detectors start from channel 1 in QDC
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if(QDC1[idx]>0){
      if(TDC1[idx+16]>0)
	(*it).InsertPSDHit(QDC1[idx],QDC1[idx+16],TDC1[idx+16]);
      else
	(*it).InsertPSDHit(QDC1[idx],QDC1[idx+16]);
    }
    idx++;
    
  }

  for(int j=0;j<16;j++){
    if(ADC2[j+16]>0)si_[0].front.InsertHit(ADC2[j+16],0,j);
    if(ADC3[j+16]>0)si_[1].front.InsertHit(ADC3[j+16],0,j);
    if(ADC2[j]>0)si_[0].back.InsertHit(ADC2[j],TDC2[j],j);
    if(ADC3[j]>0)si_[1].back.InsertHit(ADC3[j],TDC2[j+16],j);

  }

  int k=0;
  for(int j=0;j<8;j++){
    if(ADC4[k+16]>0)nai[j].fE[0]=ADC4[j+16];
    if(ADC4[k+17]>0)nai[j].fE[1]=ADC4[j+17];
    k+=2;
  }

  k=0;
  for(int j=0;j<12;j++){
    if(ADC7[k]>0)nai[j+8].fE[0]=ADC5[j];
    if(ADC7[k+1]>0)nai[j+8].fE[0]=ADC5[j];
    k+=2;
  }



  if(TDC1[0]>0)rftime[0].InsertHit(TDC1[0]);

  
  for(int k=0;k<32;k++){
    if(ADC5 && ADC5[k]>0){
      ic.xgrid.InsertHit(ADC5[k],0,k);
    }
    if(ADC6 && ADC6[k]>0){
      ic.ygrid.InsertHit(ADC6[k],0,k);
    }
  }
  if(ADC4 && ADC4[13]>0){
    ic.fdE=ADC4[13];
  }

  
}

