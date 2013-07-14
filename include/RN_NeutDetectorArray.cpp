#include "RN_NeutDetectorArray.hpp"

ClassImp(RN_NeutDetector);

void RN_NeutDetector::SetCalibrations(double elin, double eshift, double tlin, double tshift,double zero_off){
  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;
  this->zero_off=zero_off;
}

void RN_NeutDetector::SetCalibrations(RNVariableMap& detvar){
  double temp;
  detvar.GetParam(Form("%s.elin",Name().c_str()),elin);
  detvar.GetParam(Form("%s.eshift",Name().c_str()),eshift);
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);
  detvar.GetParam(Form("%s.z_off",Name().c_str()),zero_off);
  if(detvar.GetParam("neutarray.zpos",temp))
    fPos.SetZ(temp);
  if(detvar.GetParam(Form("%s.apos",Name().c_str()),temp)){
    apos=(int)temp;
    DeterminePosition(apos);
  }
}

void RN_NeutDetector::Reset(){
  RN_BaseDetector::Reset();
  fQ_long=0;
  fQ_short=0;
  fTrel=0;
  fPSD=0;
}


bool RN_NeutDetector::DeterminePosition(int apos){

  if(apos>=0 && apos < 4){
    fPos.SetX(0);
    fPos.SetY(150.622-(apos)*76.2);
    if(apos>1)
      fPos.SetY(fPos.Y()-72.644); //beamline through center
  }
  else if(apos >= 4 && apos < 8){
    fPos.SetX(-76.2);
    fPos.SetY(114.3-(apos-4)*76.2);
  }
  else if(apos >= 8 && apos < 10){
    fPos.SetX(-152.4);
    fPos.SetY(38.1-(apos-8)*76.2);
  }
  else if(apos >= 10 && apos < 12){
    fPos.SetX(152.4);
    fPos.SetY(38.1-(apos-10)*76.2);
  }
  else if(apos >= 12 && apos < 16){
    fPos.SetX(76.2);
    fPos.SetY(114.4-(apos-12)*76.2);
  }
  else return false;
  
  fPos.RotateZ(45*3.14/180);
  return true;
}

void RN_NeutDetector::ApplyCalibrations(){
  fQ_short = fQ_short - zero_off;
  fT[0]=fT[0]*tlin;//from TDC
  if(fQ_short>0 && fQ_long>0) fPSD = fQ_short/fQ_long;
  

}

void RN_NeutDetector::InsertHit(const double& q_long,const double& q_short){
  fQ_long=q_long;
  fQ_short=q_short;
}


Double_t RN_NeutDetector::PSD() const{if (fE[0]>0) return fE[1]/fE[0];}
Double_t RN_NeutDetector::Q() const{return fE[0];}


double RN_NeutDetector::CalculateTRel(const std::vector<RN_NeutDetector>& ndet, double &tfirst){
  if(tfirst>0){
    this->fTrel=this->fT[0]-tfirst;
  }
  else{
    double t=10000;
    for(unsigned int i=0;i<ndet.size();i++){
      if(ndet[i].fE[0]>0&&ndet[i].fT[0]>0&&ndet[i].fT[0]<t)
	t=ndet[i].fT[0];
    }
    if(t<10000){
      tfirst=t;
      this->fTrel=this->fT[0]-tfirst;

    }
  }
  
  return (this->fTrel);
  
}

