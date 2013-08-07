#include "RN_NeutDetectorArray.hpp"

ClassImp(RN_NeutDetector);

void RN_NeutDetector::SetCalibrations(double elin, double eshift, double tlin, double tshift,double zero_off){
  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;
  this->zero_off=zero_off;
}

void RN_NeutDetector::SetCalibrations(RN_VariableMap& detvar){
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

void RN_NeutDetector::InsertPSDHit(const double& q_long,const double& q_short){
  fQ_long=q_long;
  fQ_short=q_short;
 
}


Double_t RN_NeutDetector::PSD() const{if (fQ_long>0) return fQ_short/fQ_long;}
Double_t RN_NeutDetector::Q() const{return fQ_long;}


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

namespace RNArray{
  
  void ReconstructHits(RN_NeutCollection&in){
    
    double tfirst=-1;
    //calculate TRel for all detectors(only important for coincidence data(ie source)
    for(RN_NeutCollectionRef it = in.begin(); it != in.end();it++){
      (*it).CalculateTRel(in,tfirst);
    }
    
  }
  
  
  int PositionMap(int slot,TVector3 & pos){
    int z=pos.Z();
    
    if(slot==1)pos.SetXYZ(38.1,-152.4,z);
    else if(slot==2)pos.SetXYZ(-38.1,-152.4,z);
    else if(slot==3)pos.SetXYZ(114.3,-76.2,z);
    else if(slot==4)pos.SetXYZ(38.1,-76.2,z);
    else if(slot==5)pos.SetXYZ(-38.1,-76.2,z);
    else if(slot==6)pos.SetXYZ(-114.3,-76.2,z);
    else if(slot==7)pos.SetXYZ(150.622,0,z);
    else if(slot==8)pos.SetXYZ(74.422,0,z);
    else if(slot==9)pos.SetXYZ(-74.422,0,z);
    else if(slot==10)pos.SetXYZ(-150.622,0,z);
    else if(slot==11)pos.SetXYZ(114.3,76.2,z);
    else if(slot==12)pos.SetXYZ(38.1,76.2,z);
    else if(slot==13)pos.SetXYZ(-38.1,76.2,z);
    else if(slot==14)pos.SetXYZ(-114.3,76.2,z);
    else if(slot==15)pos.SetXYZ(38.1,152.4,z);
    else if(slot==16)pos.SetXYZ(-38.1,-152.4,z);
    else return 0;
    
    
    pos.RotateZ(45*3.14/180);
    return 1;
  
  }
}
