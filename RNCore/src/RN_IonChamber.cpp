#ifndef __RNIONC_CXX
#define __RNIONC_CXX

#include "RN_IonChamber.hpp"
#include "RN_Root.hpp"

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

  //std::cout << " Calculating XGrid Position --------" << std::endl;
  for(unsigned int i=0;i<xgrid.Mult();i++){
    //std::cout << "  Channel    : " << xgrid.Ch(i) << std::endl;
    //std::cout << "  Energy     : " << xgrid.E(i) << std::endl;
    //std::cout << " Ch A Before : " << chA << std::endl;
    //std::cout << " Ch B Before : " << chB << std::endl;
    chA += xgrid.E(i)*xgrid.Ch(i);
    chB += xgrid.E(i);
    //std::cout << " Ch A After  : " << chA << std::endl;
    //std::cout << " Ch B After  : " << chB << std::endl;
  }
  if(chB>0){
    ch = chA/chB;
  }

  //std::cout << "  Final Channel Used : " << ch << std::endl;
  //std::cout << " -----------------------------------" << std::endl;
  return (ch - 15.5); 
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
  return (ch - 15.5); 
}                                               


Double32_t RN_IonChamber::PosXAdj(){
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  chA = xgrid.E(0)*xgrid.Ch(0);
  chB = xgrid.E(0);
  for(unsigned int i=1;i<xgrid.Mult();i++){
    if ((xgrid.Ch(i) == (xgrid.Ch(0) + 1)) || (xgrid.Ch(i) == (xgrid.Ch(0) - 1))){
      chA += xgrid.E(i) * xgrid.Ch(i);
      chB += xgrid.E(i);
    }
  }
  if(chB>0){
    ch = chA/chB;
  }

  /*std::cout << "> IC X Grid Summary --------" << std::endl;
  std::cout << "> Mult : " << xgrid.Mult() << std::endl;
  for(int i=0; i < xgrid.Mult();i++){
    std::cout << ">   Energy  : " << xgrid.E(i) << std::endl;
    std::cout << ">   Channel : " << xgrid.Ch(i) << std::endl;
    
  }

  std::cout << "> Final Channel : " << ch << std::endl;
  */
  return (ch - 15.5); 
}  


Double32_t RN_IonChamber::PosYAdj(){
  Double32_t ch = 0;
  Double32_t chA = 0;
  Double32_t chB = 0;
  chA = ygrid.E(0)*ygrid.Ch(0);
  chB = ygrid.E(0);
  for(unsigned int i=1;i<ygrid.Mult();i++){
    if ((ygrid.Ch(i) == (ygrid.Ch(0) + 1)) || (ygrid.Ch(i) == (ygrid.Ch(0) - 1))){
      chA += ygrid.E(i) * ygrid.Ch(i);
      chB += ygrid.E(i);
    }
  }
  if(chB>0){
    ch = chA/chB;
  }
  return (ch - 15.5); 
}                                               



void RN_IonChamber::ReconstructHitPos(){

  //fHitPos.SetX((PosXAdj() - 0.5 + global::myRnd.Rndm()) * fWireDist);
  //fHitPos.SetY((PosYAdj() - 0.5 + global::myRnd.Rndm()) * fWireDist);
  
  fHitPos.SetX((Pos_X()) * fWireDist);
  fHitPos.SetY((Pos_Y()) * fWireDist);

  fHitPos.SetZ(0);
  if(fRotv[2]){
    fHitPos.RotateZ(fRotv[2]*M_PI/180.0);
  }
  if(fRotv[0]){
    fHitPos.RotateX(fRotv[0]*M_PI/180.0);
  }
  if(fRotv[1]){
    fHitPos.RotateY(fRotv[1]*M_PI/180.0);
  }
  
  fHitPos = fHitPos + fShiftv;
  fHitPos = fHitPos + TVector3(fXPos,fYPos,fZPos);
    

  return ;

}

void RN_IonChamber::ReconstructTwoPosHitPos(){

  double Middle = (fZPosXgrid+fZPosYgrid)/2.0;
  // double X_slope = ((PosXAdj() - 0.5 + global::myRnd.Rndm()) * fWireDist)/fZPosXgrid;
  //double Y_slope = ((PosYAdj() - 0.5 + global::myRnd.Rndm()) * fWireDist)/fZPosYgrid;

  double X_slope = ((Pos_X()) * fWireDist)/fZPosXgrid;
  double Y_slope = ((Pos_Y()) * fWireDist)/fZPosYgrid;
  
  double XMidPos = X_slope*Middle;
  double YMidPos = Y_slope*Middle;

  /* std::cout << "> Middle : " << Middle << std::endl;
  std::cout << "> XPos   : " << fZPosXgrid << std::endl;
  std::cout << "> YPos   : " << fZPosYgrid << std::endl;
  std::cout << "> X slope: " << X_slope << std::endl;
  std::cout << "> Y slope: " << Y_slope << std::endl;
  std::cout << "> X mid  : " << XMidPos << std::endl;
  std::cout << "> Y mid  : " << YMidPos << std::endl;
  */
  
  fHitPos.SetX(XMidPos);
  fHitPos.SetY(YMidPos);
  fHitPos.SetZ(0);
  if(fRotv[2]){
    fHitPos.RotateZ(fRotv[2]*M_PI/180.0);
  }
  if(fRotv[0]){
    fHitPos.RotateX(fRotv[0]*M_PI/180.0);
  }
  if(fRotv[1]){
    fHitPos.RotateY(fRotv[1]*M_PI/180.0);
  }
  
  fHitPos = fHitPos + fShiftv;
  fHitPos = fHitPos + TVector3(fXPos,fYPos,Middle);

  
  
  return ;

}


void RN_IonChamber::SetCalibrations(RN_VariableMap& detvar){
  double tempx=0, tempy = 0, tempz=0;
  esegment.SetCalibrations(detvar);
  desegment.SetCalibrations(detvar);
  xgrid.SetCalibrations(detvar);
  ygrid.SetCalibrations(detvar);
  detvar.GetParam(Form("%s.xpos",Name().c_str()),fXPos);
  detvar.GetParam(Form("%s.ypos",Name().c_str()),fYPos);
  detvar.GetParam(Form("%s.zpos",Name().c_str()),fZPos);
  detvar.GetParam(Form("%s.zposXgrid",Name().c_str()),fZPosXgrid);
  detvar.GetParam(Form("%s.zposYgrid",Name().c_str()),fZPosYgrid);
  detvar.GetParam(Form("%s.wire_dist",Name().c_str()),fWireDist);
  tempx=0,tempy = 0, tempz = 0;
  detvar.GetParam(Form("%s.rotx",GetName()),tempx);
  detvar.GetParam(Form("%s.roty",GetName()),tempy);
  detvar.GetParam(Form("%s.rotz",GetName()),tempz);
  fRotv.SetXYZ(tempx,tempy,tempz);
  tempx=0,tempy = 0, tempz = 0;
  detvar.GetParam(Form("%s.shiftx",GetName()),tempx);
  detvar.GetParam(Form("%s.shifty",GetName()),tempy);
  detvar.GetParam(Form("%s.shiftz",GetName()),tempz);
  fShiftv.SetXYZ(tempx,tempy,tempz);


}

#endif
