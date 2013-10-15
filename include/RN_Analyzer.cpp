////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
///RN_Analyzer functions like a basic MakeClass for the DataTree TTree
////////////////////////////////////////////////////////////////////////

#ifndef _RN_ANALYZER_CXX
#define _RN_ANALYZER_CXX

//RN_Analyzer headers.
#include "RN_Analyzer.hpp"
#include "RN_Root.hpp"
#include "RN_Unpack2Root.hpp"

using namespace std;
using namespace unpacker;            

RN_Analyzer::RN_Analyzer()
{

}

void RN_Analyzer::Init(TString rootfilename)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed)
  if(!RN_RootSet)
    RN_RootInit();
  
  
  fChain=new TChain("DataTree");
  fChain->Add(rootfilename);
  
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

   if(fChain->GetBranch("QDC3"))   
     fChain->SetBranchAddress("QDC3", &QDC3, &b_QDC3);
   else
     std::cout<<"no QDC3 present, do not extract in channel mapping"<<std::endl;
}




void RN_Analyzer::Loop(Long64_t start,Long64_t evnum){
  Long64_t totentries= TotEntries();    
  if (start!=0&&evnum!=0)
    if(start+evnum<totentries)
      totentries=start+evnum;
  
  Begin();
  TIter next(analyzers);
  while(RN_Analyzer * obj =  (RN_Analyzer*)next()){
    obj->Begin();
  }
  next.Reset();
  
  for (Long64_t i=start;i<totentries;i++){
    ResetGlobals();
    while(RN_Analyzer * obj = (RN_Analyzer*)next()){
      obj->ResetGlobals();
    }
    next.Reset();

    GetDetectorEntry(i);

    Process();
    while(RN_Analyzer * obj = (RN_Analyzer*)next()){
      obj->Process();
    }
    next.Reset();


  }
  
  if(RN_Analyzer * obj =  (RN_Analyzer*)analyzers->Last())
    obj->Terminate();
  else
    Terminate();

}


RN_Analyzer::~RN_Analyzer(){

}


int RN_Analyzer::GetDetectorEntry(Long64_t entry, Int_t getall){


  //Reset all detectors
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
    if(ADC4[k+16]>0)nai[j].fE[0]=ADC4[k+16];
    if(ADC4[k+17]>0)nai[j].fE[1]=ADC4[k+17];
    if(TDC3[k+16]>0)nai[j+8].fT[0]=TDC3[k+16];
    if(TDC3[k+17]>0)nai[j+8].fT[1]=TDC3[k+17];
    
    //if(TDC3[k]>0)nai[j].fT[0]=TDC3[k];  //first 16 channels don't look good
    //if(TDC3[k+1]>0)nai[j].fT[1]=TDC3[k+1];//moving to TDC4 back 16

   
    if(TDC4[k+16]>0)nai[j].fT[0]=TDC4[k+16];
    if(TDC4[k+17]>0)nai[j].fT[1]=TDC4[k+17];

    if(j<4){
      if(TDC4[k]>0)nai[j+16].fT[0]=TDC4[k];
      if(TDC4[k+1]>0)nai[j+16].fT[1]=TDC4[k+1];
    }
    
    k+=2;
  }

  k=0;
  for(int j=0;j<12;j++){
    if(ADC7[k]>0)nai[j+8].fE[0]=ADC7[k];
    if(ADC7[k+1]>0)nai[j+8].fE[1]=ADC7[k+1];
    k+=2;
  }

  for(int k=0;k<32;k++){
    if(ADC5[k]>0){
      ic.xgrid.InsertHit(ADC5[k],0,k);
    }
    if(ADC6[k]>0){
      ic.ygrid.InsertHit(ADC6[k],0,k);
    }
  }


  if(ADC4[14]>0)ic.fE=ADC4[14];
  if(ADC4[15]>0)ic.fdE=ADC4[15];
  if(TDC1[0]>0)rftime[0].InsertHit(TDC1[0]);
  if(TDC1[2]>0)rftime[1].InsertHit(TDC1[2]);
  if(TDC1[3]>0)triggerbit[0].fBit=TDC1[3];
  if(TDC1[4]>0)triggerbit[1].fBit=TDC1[4];
  
 
  Narray.ReconstructHits(neut);
  if(Narray.fMult>1)
    RNArray::ReconstructTREL(neut);
  
  
  int cref=0;
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    if(cref<si_cluster_.size())
      si_cluster_[cref].ReconstructClusters(*it);
    cref++;
    
  }
  
 
}


#endif
