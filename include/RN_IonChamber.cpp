#ifndef __RNIONC_CXX
#define __RNIONC_CXX

#include "RN_IonChamber.hpp"


ClassImp(RN_IonChamber);


void RN_IonChamber::Reset(){
  fE=0;
  fdE=0;
  fT=0;
  xgrid.Reset();
  ygrid.Reset();

}

Double32_t RN_IonChamber::SumE_X(){
  Double32_t e = 0;
  for(unsigned int i=0;i<xgrid.fMult;i++){
    e += xgrid.E(i);
  }
  return e; 
}


Double32_t RN_IonChamber::SumE_Y(){
  Double32_t e = 0;
  for(unsigned int i=0;i<ygrid.fMult;i++){
    e += ygrid.E(i);
  }
  return e; 
}


Double32_t RN_IonChamber::Pos_X(){          
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  for(unsigned int i=0;i<xgrid.fMult;i++){
    chA += xgrid.E(i)*xgrid.Ch(i);
    chB += xgrid.E(i);
  }
  if(chB>0){
    ch = chA/chB;
  }
  return ch; 
}


Double32_t RN_IonChamber::Pos_Y(){
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  for(unsigned int i=0;i<ygrid.fMult;i++){
    chA += ygrid.E(i)*ygrid.Ch(i);
    chB += ygrid.E(i);
  }
  if(chB>0){
    ch = chA/chB;
  }
  return ch; 
}                                               


void RN_IonChamber::SetCalibrations(float elin, float eshift, float tlin, float tshift){
  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;
  
}


void RN_IonChamber::SetCalibrations(RN_VariableMap& detvar){
  xgrid.SetCalibrations(detvar);
  ygrid.SetCalibrations(detvar);

  detvar.GetParam(Form("%s.elin",Name().c_str()),elin);
  detvar.GetParam(Form("%s.eshift",Name().c_str()),eshift);
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);

}

#endif
