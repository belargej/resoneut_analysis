#include "RN_NaIArray.hpp"


ClassImp(RN_NaIArray);


void RN_NaIArray::Reset(){

  RN_BaseDetector::Reset();

}


void RN_NaIArray::SetCalibrations(float elin, float eshift, float tlin, float tshift){
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
