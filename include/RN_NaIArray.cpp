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


Double32_t RN_NaIDetector::T(int id){
  if(!fT[id]>0)
    return 0;
  else
    return (fT[id] * tlin[id] + tshift[id]);
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

}




void RN_NaIArray::Reset(){

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

#endif
