#ifndef __RNTRIGGERBIT__CXX
#define __RNTRIGGERBIT__CXX
#include "RN_TriggerBit.hpp"



ClassImp(RN_TriggerBit);




void RN_TriggerBit::Reset(){
  fBit=-1;
}

void RN_TriggerBit::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.bitmin",Name().c_str()),bitmin);
  detvar.GetParam(Form("%s.bitmax",Name().c_str()),bitmax);
}

int RN_TriggerBit::Check(){

  //std::cout << " Bit Min : " << bitmin << std::endl;
  //std::cout << " Bit Max : " << bitmax << std::endl;
  //std::cout << " fBit    : " << fBit << std::endl;

  if(fBit > bitmin && fBit < bitmax)
    return 1;
  else
    return 0;
}





#endif
