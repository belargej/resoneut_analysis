#ifndef __NEUTARRAY_CXX
#define __NEUTARRAY_CXX
#include "RN_NeutDetectorArray.hpp"

ClassImp(RN_NeutDetector);


RN_NeutDetector::RN_NeutDetector(std::string name,int num,int ap):RN_BaseDetector(name,num),
								 
								  elin(1),
								  eshift(0),
								  tlin(1),
								  tshift(0),
								  zero_off(0),
								  apos(ap),
								  fQ_long(0),
								  fQ_short(0),
								  fPSD(0),
								  fT_Q(0),
								  fTrel(0)
  
{
  fPos.SetXYZ(0,0,-228.7);//set default zpos
  RNArray::PositionMap(apos,fPos);//set xpos and ypos
} 


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
    RNArray::PositionMap(apos,fPos);
  }
}

void RN_NeutDetector::Reset(){
  RN_BaseDetector::Reset();
  fQ_long=0;
  fQ_short=0;
  fTrel=0;
  fT_Q=0;
  fPSD=0;
}

Double_t RN_NeutDetector::E_est() const{
  return fQ_long*elin+eshift; 
}

Double_t RN_NeutDetector::Q_Long() const{
  return fQ_long*elin+eshift;
}

Double_t RN_NeutDetector::Q_Short_Off() const{
  return (fQ_short-zero_off);
}

Double_t RN_NeutDetector::T() const{
  return fT_Q * tlin + tshift;//from TDC
}

Double_t RN_NeutDetector::nKE(double tof) const{
  return ((.5*939.565*fPos.Z()*fPos.Z())/(90000*tof*tof));
}

Double_t RN_NeutDetector::Q_value_est(double tof,
				      double mass1,
				      double mass2,
				      double beam_e,
				      double& hi_KE,
				      double& Q_val
				      )
{
  double ne=nKE(tof);
  hi_KE=((mass1*beam_e/mass2)+(939.565*ne/mass2)+((2/mass2)*sqrt(beam_e*ne*mass1*939.565)));
  Q_val=ne+hi_KE-beam_e;
  
  return Q_val;

}

void RN_NeutDetector::InsertPSDHit(const double& q_long,const double& q_short,const double& t){
  fQ_long=q_long;
  fQ_short=q_short;
  fT_Q=t;

  if(fQ_short>0 && fQ_long>0) 
    fPSD = Q_Short_Off()/fQ_long;

}

Double_t RN_NeutDetector::PSD() const{if (fQ_long>0) return fQ_short/fQ_long;}


double RN_NeutDetector::CalculateTRel(const double &tfirst){
  if(fT_Q)
    fTrel=fT_Q-tfirst;
  return fTrel;
}


RN_NeutDetectorArray::RN_NeutDetectorArray():fMult(0),
					     fPos(16,0),
					     fQ_long(16,0),
					     fPSD(16,0),
					     fDetlist(16,-1){
  
}
					     


int RN_NeutDetectorArray::ReconstructHits(RN_NeutCollection& in){

  int cref=0;
  for(RN_NeutCollectionRef it=in.begin();it!=in.end();it++){
    if((*it).fQ_long>0){
      double ql=(*it).Q_Long();
      double qs=(*it).Q_Short_Off();
      TVector3 pos=(*it).GetPosVect();
      InsertHit(ql,qs,pos,cref);
    }
    
      cref++;
  }

}


int RN_NeutDetectorArray::Reset(){
  for(unsigned int i=0;i<fMult;i++){
    fQ_long[i]=0;
    fPSD[i]=0;
    fDetlist[i]=-1;
 }
  fMult=0;
}


int RN_NeutDetectorArray::InsertHit(const double& q_long,const double& q_short,const TVector3& pos,const int& index){
  if (!q_long>0)
    return -1;

  int i,j;
  double psd=(q_short/q_long);

  /* sorted by energy, smallest fPSD ratio first */
  for (i=(int)fMult-1;i>=0;i--){
    if (psd>fPSD[i])
      break;
  }
  
  // element i+1 is at the position for ch 
  // so make room for it
  for (j=(int)fMult-1;j>i;j--){
    fDetlist[j+1]=fDetlist[j];
    fQ_long[j+1]=fQ_long[j];
    fPSD[j+1]=fPSD[j];
    fPos[j+1]=fPos[j];
  }
  // and shove it in
  fDetlist[i+1]=index;
  fQ_long[i+1]=q_long;
  fPSD[i+1]=psd;
  fPos[i+1]=pos;
  fMult += 1;

  return (i+1);
}









namespace RNArray{
  
  void ReconstructTREL(RN_NeutCollection&in){
    double t=4096;
    for(unsigned int i=0;i<in.size();i++){
      if(in[i].fQ_long>0 &&in[i].T()>0 &&in[i].T()<t)
	t=in[i].T();
    }
    if(t<4096){    
      //calculate TRel for all detectors(only important for coincidence data(ie source)
      for(RN_NeutCollectionRef it = in.begin(); it != in.end();it++){
	(*it).CalculateTRel(t);
      }
    }
    
  }
  
  
  int PositionMap(int slot,TVector3 & pos){
    double z=pos.Z();
    
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
    else if(slot==16)pos.SetXYZ(-38.1,152.4,z);
    else return 0;
    
    
    pos.RotateZ(45*3.14/180);
    return 1;
  
  }
}


#endif
