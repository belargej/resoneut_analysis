/************************************************************************
Class: RN_Base Detector
Author: Sean Kuvin                             


 RN_BaseDetector class: Inserting a hit (energy, time, channel) 
 is sorted by energy(default) and mult is counted. 
 Adapted from SpecTcl DetectorParameters.cpp (Diffenderfer, Wiedenhoever)

***********************************************************************/


#ifndef __BaseDetector__CXX
#define __BaseDetector__CXX

#include "RN_BaseDetector.hpp"

RN_BaseDetector::RN_BaseDetector(std::string name, int num):RN_BaseClass(name,name),
							    fNumOfCh(num),
							    sorted_by_channel(0),
							    lowlimit(0),
							    highlimit(4096),		
							    fCh_cal(num,int(0)),
							    a0(num,double(0)),
							    a1(num,double(1)),
							    t0(num,double(0)),
							    t1(num,double(1)),
							    q_offset(num,double(0)),
							    t_offset(num,double(0)),

							    fMult(0),
							    fChlist(num,double(0)),
							    fE(num,double(0)),
							    fT(num,double(0))
							    
							    
				       
				       
{
  for(int i=0;i<num;i++){
    fCh_cal[i]=i;
  }


}

void RN_BaseDetector::Reset(){
  for(unsigned int i=0;i<fMult;i++){
    fE[i]=0;
    fT[i]=0;
    fChlist[i]=-1;
 }
  fMult=0;
}


void RN_BaseDetector::Init(const double& num){
  fNumOfCh=num;
  //  fChlist.resize(fNumOfCh,double(0));
  //fE.resize(fNumOfCh,double(0));
  //fT.resize(fNumOfCh,double(0));

}

void RN_BaseDetector::SetELimits(const double& elow,const double& ehigh){
  lowlimit=elow;
  highlimit=ehigh;
}

void RN_BaseDetector::SetCalibrations(RN_VariableMap& detvar){
  for(int i=0;i<NumOfCh();i++){
    detvar.GetParam(Form("%s.ch[%d]",GetName(),i),fCh_cal[i]);
    detvar.GetParam(Form("%s.a0[%d]",GetName(),i),a0[i]);  
    detvar.GetParam(Form("%s.a1[%d]",GetName(),i),a1[i]);
    detvar.GetParam(Form("%s.t0[%d]",GetName(),i),t0[i]);  
    detvar.GetParam(Form("%s.t1[%d]",GetName(),i),t1[i]);
    detvar.GetParam(Form("%s.q_offset[%d]",GetName(),i),q_offset[i]); 
    detvar.GetParam(Form("%s.t_offset[%d]",GetName(),i),t_offset[i]);  
  } 
  detvar.GetParam(Form("%s.elowlimit",GetName()),lowlimit);
  detvar.GetParam(Form("%s.ehighlimit",GetName()),highlimit);
  detvar.GetParam(Form("%s.sorted_by_channel",GetName()),sorted_by_channel);

}

void RN_BaseDetector::Print(){
  TObject::Print();
  std::cout<<"number of channels: "<<NumOfCh()<<"\n";
  std::cout<<"sorted by channel: "<<sorted_by_channel<<"\n";
  std::cout<<"elowlimit: "<<lowlimit<<"\n";
  std::cout<<"ehighlimit: "<<highlimit<<"\n";

  return;
}

int RN_BaseDetector::InsertHit(const double& e,const double& t,const double& ch){

  if(e <= lowlimit || e > highlimit) 
    return -1;
  int i,j;
  if (sorted_by_channel){ 
    /* insert into list sorted by channel */
    for (i=(int)fMult-1;i>=0;i--){
      if (ch>fChlist[i])
	break;
    }
  }
  else{ /* sorted by energy, largest e first */
    for (i=(int)fMult-1;i>=0;i--){
      if (e<fE[i])
	break;
    }
  }   
  // element i+1 is at the position for ch 
  // so make room for it
  for (j=(int)fMult-1;j>i;j--){
    fChlist[j+1]=(double)fChlist[j];
    fE[j+1]=(double)fE[j];
    fT[j+1]=(double)fT[j];
  }
  // and shove it in
  fChlist[i+1]=ch;
  fE[i+1]=e;
  fT[i+1]=t;
  fMult += 1.;

  return (i+1);
}

#endif
