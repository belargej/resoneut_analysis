#ifndef __RNNAI__CXX
#define __RNNAI__CXX
#include "RN_NaIArray.hpp"


ClassImp(RN_NaIArray);
ClassImp(RN_NaIDetector);




void RN_NaIDetector::Reset(){

  for(int i=0;i<2;i++){
    fE[i]=0;
    fT[i]=0;
  }

}


Double32_t RN_NaIDetector::Position() const{
  if(E2()>0 && E1()>0)
    return ((1 / ( 2 * light_atten))*(log(E2()/E1())));
  
  else
    return 0xffff;
}


void RN_NaIDetector::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.elin[0]",Name().c_str()),elin[0]);
  detvar.GetParam(Form("%s.eshift[0]",Name().c_str()),eshift[0]);
  detvar.GetParam(Form("%s.tlin[0]",Name().c_str()),tlin[0]);
  detvar.GetParam(Form("%s.tshift[0]",Name().c_str()),tshift[0]);
  detvar.GetParam(Form("%s.elin[1]]",Name().c_str()),elin[1]);
  detvar.GetParam(Form("%s.eshift[1]",Name().c_str()),eshift[1]);
  detvar.GetParam(Form("%s.tlin[1]",Name().c_str()),tlin[1]);
  detvar.GetParam(Form("%s.tshift[1]",Name().c_str()),tshift[1]);
  detvar.GetParam(Form("%s.tzero[0]",Name().c_str()),tzero[0]);
  detvar.GetParam(Form("%s.tzero[1]",Name().c_str()),tzero[1]);
  detvar.GetParam(Form("%s.mutwo",Name().c_str()),MuTwo);

}




void RN_NaIArray::Reset(){
  //don't call BaseReset before using fMultx

  for(unsigned int i=0;i<fMult;i++){

    fPosition[i]=0;
  }

 

  RN_BaseDetector::Reset();
 
}


void RN_NaIArray::SetCalibrations(Double32_t elin, Double32_t eshift, Double32_t tlin, Double32_t tshift){
  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;
  
}


void RN_NaIArray::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.elin",Name().c_str()),elin);
  detvar.GetParam(Form("%s.eshift",Name().c_str()),eshift);
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);
  

}


void RN_NaIArray::ReconstructHits(const RN_NaICollection& in){

    unsigned int idx(0);
    for(RN_NaICollectionCRef it=in.begin();it!=in.end();it++){
      if((*it).E_Gamma()>0){
	InsertHit((*it).E_Gamma(),(*it).T(),(*it).Position(),idx);
      }
      idx++;
    }
    
  
}

int RN_NaIArray::InsertHit(const Double32_t&egamma,const Double32_t&tgamma,const Double32_t&pos_gamma,const Int_t&ch){
  if (!egamma>0)
    return -1;

  int i,j;

  /* sorted by energy */
  for (i=(int)fMult-1;i>=0;i--){
    if (egamma<fE[i])
      break;
  }
  
  // element i+1 is at the position for ch 
  // so make room for it
  for (j=(int)fMult-1;j>i;j--){
    fChlist[j+1]=fChlist[j];
    fT[j+1]=fT[j];
    fE[j+1]=fE[j];
    fPosition[j+1]=fPosition[j];
  }
  // and shove it in
  fChlist[i+1]=ch;
  fE[i+1]=egamma;
  fPosition[i+1]=pos_gamma;
  fT[i+1]=tgamma;
  fMult += 1;

  return (i+1);
}



#endif
