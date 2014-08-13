#ifndef __BaseDetector__CXX
#define __BaseDetector__CXX

#include "RN_BaseDetector.hpp"
#include <cstdlib>
#include <iostream>
ClassImp(RN_BaseDetector);

RN_BaseDetector::RN_BaseDetector(std::string name, int num):RN_BaseClass(name,name),
							    fNumOfCh(num),
							    fSortedByChannel(0),
							    fLowLimit(0),
							    fHighLimit(4096),		
							    fChCal(num,int(0)),
							    fA0(num,double(0)),
							    fA1(num,double(1)),
							    fT0(num,double(0)),
							    fT1(num,double(1)),
							    fQOffset(num,double(0)),
							    fTOffset(num,double(0)),
							    fELin(1),
							    fEShift(0),
							    fTLin(1),
							    fTShift(0),
							    fMult(0),
							    fChlist(num,double(0)),
							    fE(num,double(-1)),
							    fT(num,double(-1))
							    
							    
				       
				       
{
  for(int i=0;i<num;i++){
    fChCal[i]=i;
  }


}

void RN_BaseDetector::Reset(){
  for(unsigned int i=0;i<fMult;i++){
    fE[i]=-1;
    fT[i]=-1;
    fChlist[i]=-1;
 }
  fMult=0;
}


void RN_BaseDetector::Init(const int& num){
  fNumOfCh=num;
  //  fChlist.resize(fNumOfCh,double(0));
  //fE.resize(fNumOfCh,double(0));
  //fT.resize(fNumOfCh,double(0));

}

void RN_BaseDetector::SetELimits(const double& elow,const double& ehigh){
  fLowLimit=elow;
  fHighLimit=ehigh;
}

void RN_BaseDetector::SetCalibrations(RN_VariableMap& detvar){
  for(unsigned int i=0;i<NumOfCh();i++){
    detvar.GetParam(Form("%s.ch[%d]",GetName(),i),fChCal[i]);
    detvar.GetParam(Form("%s.a0[%d]",GetName(),i),fA0[i]);  
    detvar.GetParam(Form("%s.a1[%d]",GetName(),i),fA1[i]);
    detvar.GetParam(Form("%s.t0[%d]",GetName(),i),fT0[i]);  
    detvar.GetParam(Form("%s.t1[%d]",GetName(),i),fT1[i]);
    detvar.GetParam(Form("%s.q_offset[%d]",GetName(),i),fQOffset[i]); 
    detvar.GetParam(Form("%s.t_offset[%d]",GetName(),i),fTOffset[i]);  
  } 
  detvar.GetParam(Form("%s.elowlimit",GetName()),fLowLimit);
  detvar.GetParam(Form("%s.ehighlimit",GetName()),fHighLimit);
  detvar.GetParam(Form("%s.sorted_by_channel",GetName()),fSortedByChannel);
  detvar.GetParam(Form("%s.elin",GetName()),fELin);
  detvar.GetParam(Form("%s.eshift",GetName()),fEShift);
  detvar.GetParam(Form("%s.tlin",GetName()),fTLin);
  detvar.GetParam(Form("%s.tshift",GetName()),fTShift);

}

void RN_BaseDetector::Print(){
  TObject::Print();
  std::cout<<"number of channels: "<<NumOfCh()<<"\n";
  std::cout<<"sorted by channel: "<<fSortedByChannel<<"\n";
  std::cout<<"elowlimit: "<<fLowLimit<<"\n";
  std::cout<<"ehighlimit: "<<fHighLimit<<"\n";

  return;
}

int RN_BaseDetector::InsertHit(const double& e,const double& t,const double& ch){

  //if there is a time we should continue with the sorting process even if there is no energy...if there is no time, check whether the energy is within the limit. if its not, exit.

  if(!(t>0)){
    if(e <= fLowLimit || e > fHighLimit){ 
      return -1;
    }
  }
  if(fMult>=NumOfCh()){
    std::cout<<"Attempting to insert a hit into an already fully allocated detector:\n fMult = "<<fMult<<" fNumOfCh = "<<fNumOfCh<<"\n Double check your channel mapping\n Enter 1 to EXIT with failure\n";
    int query = 0;
    std::cin>>query;
    if (query == 1){ 
      exit(EXIT_FAILURE);
    }
    return -1;
  }
    

  int i,j;
  if (fSortedByChannel){ 
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
  fMult += 1;

  return (i+1);
}

#endif
