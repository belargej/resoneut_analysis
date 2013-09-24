#ifndef __NEUTARRAY_CXX
#define __NEUTARRAY_CXX
#include "RN_NeutDetectorArray.hpp"
#include "RN_Root.hpp"
#define M_N 939.565404 
#define M_P 938.2720137 
#define M_C 11177.92852

ClassImp(RN_NeutDetector);

double z_pos(0);


RN_NeutDetector::RN_NeutDetector(std::string name,int num,int ap):RN_BaseDetector(name,num),
								 
								  elin(1),
								  eshift(0),
								  tlin(1),
								  tshift(0),
								  zero_off(0),
								  apos(ap),
								  fRadius(34),
								  fThickness(25.4),
								  fThreshold(0.05),
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
  detvar.GetParam("all.radius",fRadius);
  detvar.GetParam("all.thickness",fThickness);
  detvar.GetParam("all.threshold",fThreshold);
  detvar.GetParam(Form("%s.radius",Name().c_str()),fRadius);
  detvar.GetParam(Form("%s.thickness",Name().c_str()),fThickness);
  detvar.GetParam(Form("%s.threshold",Name().c_str()),fThreshold);
  


}

void RN_NeutDetector::Reset(){
  RN_BaseDetector::Reset();
  fQ_long=0;
  fQ_short=0;
  fTrel=0;
  fT_Q=0;
  fPSD=0;
  fDt=0;
  fEsum=0;
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
    fTrel=T()-tfirst;
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

  return 1;
}


int RN_NeutDetectorArray::Reset(){
  for(int i=0;i<fMult;i++){
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





bool RN_NeutDetector::inDet(const TVector3& v){
  //first see if it intersects the plane of the detector
  double udotnormv = (v.Unit()).Dot(fPos.Unit());
  if(udotnormv <= 0.)
    return false;

  //Find distance from target to interesection point of detector plane 
  //use line-plane intersection formula
  TVector3 posvect = GetPosVect();
  double vdist = (fPos.Unit()).Dot(posvect)/udotnormv;
  if(vdist <= 0.)
    return false;
  
  //vector from target to interesection point of detector plane with
  //magnitude equal to distance
  TVector3 v_to_det(v);
  v_to_det.SetMag(vdist);

  //create vector from detector origin to interaction point on plane of detector
  TVector3 ch_vect = v_to_det - posvect;

  //see if it is within the detector area
  double ch_vect_x = fabs(ch_vect.X());
  double ch_vect_y = fabs(ch_vect.Y());
  if(sqrt(ch_vect_x*ch_vect_x+ch_vect_y*ch_vect_y) > fRadius){
    return false;
  }
  return true;
}


int RN_NeutDetector::NeutIn(TLorentzVector nLV,double& t,double& e){
  TLorentzVector inLV = nLV;
  double pz=nLV.Pz(), px=nLV.Px(),py=nLV.Py();
  double psqr=px*px+py*py+pz*pz;
  double tof=(fPos.Z())*M_N/(pz*3*100);//tof to front of detector
  fDt=0;//time to first reaction
  double x_pos=(px*tof*300/(M_N))-fPos.X();
  double y_pos=(py*tof*300/(M_N))-fPos.Y();
  HitPos.SetXYZ(x_pos+fPos.X(),y_pos+fPos.Y(),fPos.Z());
  double radial_pos=sqrt(x_pos*x_pos+y_pos*y_pos);  
 
  z_pos=fThickness;

  while (radial_pos <= fRadius && z_pos >= 0 && z_pos <= fThickness){
    double nKE=inLV.E()-inLV.M();
    if (nKE<0.010)
      break;
    //step is fThickness/100 (mm)
    double dx=(fThickness/100)*px/(sqrt(psqr));
    double dy=(fThickness/100)*py/(sqrt(psqr));
    double dz=(fThickness/100)*pz/(sqrt(psqr));
    
    if(!H_hit(inLV))
      C_hit(inLV);
    
    px = inLV.Px();
    py = inLV.Py();
    pz = inLV.Pz();
    
    double dr=sqrt(dx*dx+dy*dy);
    z_pos += dz;//move to start of next sector.
    radial_pos += dr;
    
  }
  
  if(fEsum>fThreshold){
    e = fEsum;
    t = tof+ fDt;
    fT_Sim=t;
    return true;
  }
  else{
    e=0;
    t=0;
    return false;
  }

}	  

int RN_NeutDetector::H_hit(TLorentzVector& inLV){
  double KE = inLV.E()-inLV.M();
  TLorentzVector neut_LVcopy(inLV);
  TLorentzVector Target(0.,0.,0.,M_P);
  TLorentzVector Before(Target + neut_LVcopy);
  TVector3 boostv = (-1)*Before.BoostVector();
  Before.Boost(boostv);
  neut_LVcopy.Boost(boostv);  //this is for getting neutron KE in CM
  double nKE=neut_LVcopy.E()-neut_LVcopy.M();
  
  if(nKE>.010 && nKE<.400){
    if(global::myRnd.Rndm()>(0.004508869*exp(2.91109-3.12513*nKE)*fThickness/100))
      return 0;
  }
  else if(nKE>=.400&& nKE<1.4){
    if(global::myRnd.Rndm()>(0.004508869*exp(2.1646-.67*nKE)*fThickness/100))
      return 0;
  }
  else if(nKE<0.010 || nKE>1.4){
    return 0;
  }
  
  fCounter++;
  if(fCounter==1){fDt=(z_pos-fThickness)*M_N/(neut_LVcopy.Pz()*3*100);}
  double theta = 3.14 * global::myRnd.Rndm(); 
  double phi = 2.* 3.14 * global::myRnd.Rndm(); //isotropic CM
  double psqr=2*M_P*M_N*(Before.E()-M_N-M_P)/(M_P+M_N);
  double pz=-1*sqrt(psqr)*cos(theta);
  double px=-1*sqrt(psqr)*sin(theta)*cos(phi);
  double py=-1*sqrt(psqr)*sin(theta)*sin(phi);
  double e_neut=(psqr/(2*M_N))+M_N;
  inLV.SetPxPyPzE(px,py,pz,e_neut);
  inLV.Boost(-1*boostv);//boost back to lab frame
  fEsum = fEsum + KE - (inLV.E()-inLV.M());
  return 1;
  
}

int RN_NeutDetector::C_hit(TLorentzVector& inLV){
  double KE = inLV.E()-inLV.M();
  TLorentzVector neut_LVcopy(inLV);
  TLorentzVector Target(0.,0.,0.,M_C);
  TLorentzVector Before(Target + neut_LVcopy);
  TVector3 boostv = (-1)*Before.BoostVector();
  Before.Boost(boostv);
  neut_LVcopy.Boost(boostv);
  double nKE=neut_LVcopy.E()-neut_LVcopy.M();
  if(nKE>=.010&& nKE<1.4){
    if(global::myRnd.Rndm()>(0.005798*(4.6126-1.77378*nKE)*fThickness/100))
      return 0;
  }
  else if(nKE<0.010 || nKE>1.4){
    return 0;
  }  
  
  fCounter++;
  if(fCounter==1){fDt=(z_pos-fThickness)*939.56404/(neut_LVcopy.Pz()*3*100);}
  double theta = 3.14 * global::myRnd.Rndm(); 
  double phi = 2.* 3.14 * global::myRnd.Rndm(); //isotropic CM
  double psqr=2*M_C*M_N*(Before.E()-M_N-M_C)/(M_C+M_N);
  double pz=-1*sqrt(psqr)*cos(theta);
  double px=-1*sqrt(psqr)*sin(theta)*cos(phi);
  double py=-1*sqrt(psqr)*sin(theta)*sin(phi);
  double e_neut=(psqr/(2*M_N))+M_N;
  inLV.SetPxPyPzE(px,py,pz,e_neut);
  inLV.Boost(-1*boostv);//boost back to lab frame
  fEsum = fEsum + KE - (inLV.E()-inLV.M());//add what neut loses
  return 1;  
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
