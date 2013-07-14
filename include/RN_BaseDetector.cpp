#include "RN_BaseDetector.hpp"


void RN_BaseDetector::Reset(){
  for(unsigned int i=0;i<fMult;i++){
    fE[i]=0;
    fT[i]=0;
    fChlist[i]=0;
 }
  fMult=0;
}


void RN_BaseDetector::Init(const double& num){
  fNumOfCh=num;
  fChlist.resize(fNumOfCh,double(0));
  fE.resize(fNumOfCh,double(0));
  fT.resize(fNumOfCh,double(0));

}


void RN_BaseDetector::InsertHit(const double& e,const double& t,const double& ch){

  if(e < lowlimit || e > highlimit) 
    return;
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

  
}
