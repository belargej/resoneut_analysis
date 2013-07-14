#include "RN_SiliconDetectors.hpp"


ClassImp(RN_S2Detector);



////////////////////////////////////////////////////////////////////
///RN_S2Detector
////////////////////////////////////////////////////////////////////

void RN_S2Detector::Reset(){
  front.Reset();
  back.Reset();

}
void RN_S2Detector::SetCalibrations(const std::vector<double>&a0,const std::vector<double>& a1,double elin,double eshift,double tlin, double tshift){
  for (int i=0;i<front.NumOfCh();i++){
    this->a0[i]=a0[i];
    this->a1[i]=a1[i];
  }

  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;

}


void RN_S2Detector::SetCalibrations(RNVariableMap& detvar){
  int numofch=front.NumOfCh()+back.NumOfCh();
  for (int i=0;i<numofch;i++){
    detvar.GetParam(Form("%s.a0[%d]",Name().c_str(),i),a0[i]);  
    detvar.GetParam(Form("%s.a1[%d]",Name().c_str(),i),a1[i]);
  }
  detvar.GetParam(Form("%s.elin",Name().c_str()),elin);
  detvar.GetParam(Form("%s.eshift",Name().c_str()),eshift);
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);
  double tempx,tempy,tempz;
  detvar.GetParam(Form("%s.xpos",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.ypos",Name().c_str()),tempy);
  detvar.GetParam(Form("%s.zpos",Name().c_str()),tempz);
  SetPosVect(TVector3(tempx,tempy,tempz));
  detvar.GetParam(Form("%s.shiftx",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.shifty",Name().c_str()),tempy);
  detvar.GetParam(Form("%s.shiftz",Name().c_str()),tempz);
  SetShiftVect(TVector3(tempx,tempy,tempz));
  detvar.GetParam(Form("%s.rot_theta",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.rot_phi",Name().c_str()),tempy);
  
  Calcnormv();

}


void RN_S2Detector::Calcnormv(){
  TVector3 resultv = (shiftv_ + posv_).Unit();
  if(rotv_[0]){
    resultv.RotateX(rotv_[0]*M_PI/180.);
  }
  if(rotv_[1]){
    resultv.RotateY(rotv_[1]*M_PI/180.);
  }
  if(rotv_[2]){
    resultv.RotateZ(rotv_[2]*M_PI/180.);
  }
  
  normv_ = resultv;
}


void RN_S2Detector::ApplyCalibrations(){
  for(int i=0;i<front.fMult;i++){
    front.fE[i]=((a1[i]*front.fE[i])+a0[i])*elin+eshift;   
    front.fT[i]=tlin*front.fT[i]+tshift;
  }
  for(int i=0;i<back.fMult;i++){
    back.fE[i]=((a1[i]*back.fE[i])+a0[i])*elin+eshift;   
    back.fT[i]=tlin*back.fT[i]+tshift;
  }
  
}

bool RN_S2Detector::inDet(const TVector3& v){
  //first see if it intersects the plane of the detector
  double udotnormv = (v.Unit()).Dot(normv_);
  if(udotnormv <= 0.)
    return false;

  //Find distance from target to interesection point of detector plane 
  //use line-plane intersection formula
  TVector3 posvect = GetPosVect();
  double vdist = normv_.Dot(posvect)/udotnormv;
  if(vdist <= 0.)
    return false;
  
  //vector from target to interesection point of detector plane with
  //magnitude equal to distance
  TVector3 v_to_det(v);
  v_to_det.SetMag(vdist);

  //create vector from detector origin to interaction point on plane of detector
  TVector3 ch_vect = v_to_det - posvect;

  //radial component from center of detector
  double ch_vect_mag = ch_vect.Mag();

  //see if it falls in the detector window
  if((ch_vect_mag > S2OUTERRAD)||(ch_vect_mag < S2INNERRAD)){
    return false;
  }
  return true;
}

bool RN_S2Detector::Vect_to_ch(const TVector3& v,double& cf,double& cb){
  //first see if it intersects the plane of the detector
  double udotnormv = (v.Unit()).Dot(normv_);
  if(udotnormv <= 0.)
    return false;

  //Find distance from target to interesection point of detector plane 
  //use line-plane intersection formula
  TVector3 posvect = GetPosVect();
  double vdist = normv_.Dot(posvect)/udotnormv;
  if(vdist <= 0.)
    return false;
  
  //vector from target to interesection point of detector plane with
  //magnitude equal to distance
  TVector3 v_to_det(v);
  v_to_det.SetMag(vdist);

  //create vector from detector origin to interaction point on plane of detector
  TVector3 ch_vect = v_to_det - posvect;

  //radial component from center of detector
  double ch_vect_mag = ch_vect.Mag();

  //see if it falls in the detector window
  if((ch_vect_mag > S2OUTERRAD)||(ch_vect_mag < S2INNERRAD)){
    return false;
  }
  
  //find the ring
  cf = (ch_vect_mag-S2INNERRAD)/ring_pitch_;
  
  double phi = ch_vect.Phi()*180./M_PI;
  //switch range of phi from (-180,180) to (0,360)
  phi += 360.;
  if(phi > 360.)
    phi = phi - 360.;
  
  //find the segment
  cb = static_cast<double>(back.NumOfCh()) - (phi/delta_phi_);
  
  return true;
}


TVector3 RN_S2Detector::chVect(const double& cf,const double& cb){
  //First make a vector to the channel for a detector at the origin
  TRandom3 myRnd;
  double s;
  double phi;
  if((cf < static_cast<double>(front.NumOfCh())) 
     && (cb < static_cast<double>(back.NumOfCh()))){
    s = S2INNERRAD + (cf + (double)myRnd.Rndm())*ring_pitch_;
    phi = (static_cast<double>(front.NumOfCh()) - cb 
	   - (double)myRnd.Rndm())*delta_phi_;
  }
  else{ 
    std::cerr << "InValid channels sent to S2DetectorPar::chVect" << std::endl;
    exit(EXIT_FAILURE);
  }

  TVector3 resultv;
  resultv.SetMagThetaPhi(s,M_PI/2.,phi*M_PI/180.);
  
  //then calibrate the vector 
  //add it to the detector position vector
  if(rotv_[0]){
    resultv.RotateX(rotv_[0]*M_PI/180.);
  }
  if(rotv_[1]){
    resultv.RotateY(rotv_[1]*M_PI/180.);
  }
  if(rotv_[2]){
    resultv.RotateZ(rotv_[2]*M_PI/180.);
  }
  resultv = resultv + shiftv_;
  resultv = resultv + posv_;
    
  return resultv;
}
