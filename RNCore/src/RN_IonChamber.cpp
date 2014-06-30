#ifndef __RNIONC_CXX
#define __RNIONC_CXX

#include "RN_IonChamber.hpp"


ClassImp(RN_IonChamber);


void RN_IonChamber::Reset(){
  esegment.Reset();
  desegment.Reset();
  xgrid.Reset();
  ygrid.Reset();

}

Double32_t RN_IonChamber::SumE_X() const{
  Double32_t e = 0;
  for(unsigned int i=0;i<xgrid.Mult();i++){
    e += xgrid.E(i);
  }
  return e; 
}

Double32_t RN_IonChamber::SumE_Y() const{
  Double32_t e = 0;
  for(unsigned int i=0;i<ygrid.Mult();i++){
    e += ygrid.E(i);
  }
  return e; 
}


Double32_t RN_IonChamber::Pos_X(){          
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  for(unsigned int i=0;i<xgrid.Mult();i++){
    chA += xgrid.E(i)*xgrid.Ch(i);
    chB += xgrid.E(i);
  }
  if(chB>0){
    ch = chA/chB;
  }
  return (ch - 16.5); 
}


Double32_t RN_IonChamber::Pos_Y(){
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  for(unsigned int i=0;i<ygrid.Mult();i++){
    chA += ygrid.E(i)*ygrid.Ch(i);
    chB += ygrid.E(i);
  }
  if(chB>0){
    ch = chA/chB;
  }
  return (ch - 16.5); 
}                                               

void RN_IonChamber::ReconstructHitPos(){
  fHitPos.SetX(Pos_X() * fWireDist + fXPos);
  fHitPos.SetY(Pos_Y() * fWireDist + fYPos);
  fHitPos.SetZ(fZPos);
  return ;

}


void RN_IonChamber::SetCalibrations(RN_VariableMap& detvar){
  esegment.SetCalibrations(detvar);
  desegment.SetCalibrations(detvar);
  xgrid.SetCalibrations(detvar);
  ygrid.SetCalibrations(detvar);
  detvar.GetParam(Form("%s.xpos",Name().c_str()),fXPos);
  detvar.GetParam(Form("%s.ypos",Name().c_str()),fYPos);
  detvar.GetParam(Form("%s.zpos",Name().c_str()),fZPos);
  detvar.GetParam(Form("%s.wire_dist",Name().c_str()),fWireDist);

}

#endif
