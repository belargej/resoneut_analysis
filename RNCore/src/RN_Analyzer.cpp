/*************************************************************************
Class: RN_Analyzer, RN_Analyzer_Stack
Author: Sean A. Kuvin -2013
*******************************************************************/

#ifndef _RN_ANALYZER_CXX
#define _RN_ANALYZER_CXX

//RN_Analyzer headers.
#include "RN_Analyzer.hpp"
#include "RN_Root.hpp"

ClassImp(RN_Analyzer);
ClassImp(RN_Analyzer_Stack);

using namespace RNROOT;


RN_Analyzer::RN_Analyzer(const std::string&a,const std::string &b):RN_EventProcessor(a.c_str(),b.c_str())
{

}

RN_Analyzer::~RN_Analyzer(){
}


int RN_Analyzer::GetDetectorEntry(){
  RNROOT::ResetRNROOTDetectors();
  
  //ChanneltoDetector
  
  //Neutron Detector Long and Short gate Hits
    int idx=1; //neut detectors start from channel 1 in QDC
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if(QDC1[idx]>0){
	(*it).InsertPSDHit(QDC1[idx],QDC1[idx+16]);
      }
      if(TDC1[idx+16]>0){  
      (*it).fT_Q=TDC1[idx+16];
      }  
      idx++;
    }
    //Neutron Detector position channel hits
    //insert here
    
    //Silicon Hits
    for(int j=0;j<16;j++){
    //si_a de front
      if(ADC3[j+16]>0)si_[0].front.InsertHit(ADC3[j+16],0,j);
      //si_b e front
      if(ADC2[j+16]>0)si_[1].front.InsertHit(ADC2[j+16],0,j);
      //si_a de back
      if(ADC3[j]>0)si_[0].back.InsertHit(ADC3[j],TDC2[j+16],j);
      //si_b e back
      if(ADC2[j]>0)si_[1].back.InsertHit(ADC2[j],TDC2[j],j);

    }
    
    //NaI E and T hits
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
    
    

    //IonChamber Position grid hits
    for(int k=0;k<32;k++){
      if(ADC5[k]>0){
	ic.xgrid.InsertHit(ADC5[k],0,k);
      }
    if(ADC6[k]>0){
      ic.ygrid.InsertHit(ADC6[k],0,k);
    }
    }
    
    //IonChamber E+dE hits
    if(ADC4[13]>0)ic.fE=ADC4[13];
    if(ADC4[12]>0)ic.fdE=ADC4[12];
    //if(ADC4[13]>0)ic.fE=ADC4[13];
    //if(ADC4[14]>0)ic.fdE=ADC4[14];  
    if(TDC1[1]>0) ic.fT = TDC1[1];
    
    //rftime
    if(TDC1[0]>0)rftime.InsertHit(TDC1[0]);
    
    //mcptime
    //   if(TDC1[2]>0)rftime[1].InsertHit(TDC1[2]);
    
    
    if(TDC1[3]>0)triggerbit[0].fBit=TDC1[3];
    if(TDC1[4]>0)triggerbit[1].fBit=TDC1[4];
    

    //recontruct clusters for all silicon detectors in S2Collection
    unsigned int cref=0;
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      if(cref<si_cluster_.size())
      si_cluster_[cref].ReconstructClusters(*it);
      cref++;
      
    }
    
    //reconstruct neutron detector hits for all neutron detectors 
    //in NeutCollection
    Narray.ReconstructHits(neut);
    RNArray::ReconstructTREL(neut,Narray.fT_mult,Narray.fT_first,Narray.fDetfirst);
    
    nai_array.ReconstructHits(nai);
    si_array.ReconstructHits(si_cluster_);
    
    return 1;
}



void RN_Analyzer::Reset(){
}

bool RN_Analyzer::Begin(){
  return true;
}

bool RN_Analyzer::Process(){
  return true;
}

bool RN_Analyzer::ProcessFill(){
  return true;
}

bool RN_Analyzer::Terminate(){
  return true;
}

bool RN_Analyzer_Stack::Begin(){
  TIter next(fRNStack);
  while( RN_Analyzer*obj = (RN_Analyzer*)next()){
    obj->Begin();
  }
  return 1;
}


bool RN_Analyzer_Stack::Process(){
  TIter next(fRNStack);
  while( RN_Analyzer*obj = (RN_Analyzer*)next()){
    if(!obj->Process()){
      return 0;

    }
  }
  return 1;
}

bool RN_Analyzer_Stack::ProcessFill(){
  TIter next(fRNStack);
  while( RN_Analyzer*obj = (RN_Analyzer*)next()){
    if(!obj->ProcessFill()){
      return 0;
    }
  }
  return 1;
}

bool RN_Analyzer_Stack::Terminate(){
  TIter next(fRNStack);
  while( RN_Analyzer*obj = (RN_Analyzer*)next()){
    obj->Terminate(); 
  }
  return 1;
}



UInt_t RN_Analyzer_Stack::Add(RN_BaseClass *base){
  if(!base->InheritsFrom("RN_Analyzer")){
    return 0;
  }
  if(!fRNStack){
    Init();
  }
  fRNStack->Add(base);
  return 1;
}

#endif
