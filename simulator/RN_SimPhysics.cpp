#include "RN_SimPhysics.hpp"
#define M_N 939.565404 
#define M_P 938.2720137 
#define M_C 11177.92852

////////////////////////////////////////////////////////////////////////
///To store multiple angular distributions for particle interactions///
///////////////////////////////////////////////////////////////////////

RN_AngularDistribution::RN_AngularDistribution(){
}
RN_AngularDistribution::~RN_AngularDistribution(){
}

RN_AngularDistribution::RN_AngularDistribution(std::string _file){

  std::ifstream in(_file.c_str()); 
  char line[80];
  int i;
  double test,value;

  for (i=0;i<180;i++){
    in.getline(line,80);
    sscanf(line,"%lf%lf",&test,&value);
    if ((int)test == i){
      this->ad[i]=value;
    }
    //    else{
    //  cout << "could not read line "<<line<<" in angle " << i;
    // }
  }
  in.close();
  value=this->ad[0];
  for (i=0;i<180;i++){
    this->ad[i] /= value;
  }  


}

double RN_AngularDistribution::GetAD(double angle)
{
  int index=(int)floor(angle);
  return((this->ad)[index]);
}


RN_SimEvent::RN_SimEvent(){}
RN_SimEvent::~RN_SimEvent(){}
RN_SimEvent::RN_SimEvent(double beam_energy,double beam_mass,
		   double target_mass,double rad_mass,
		   double hi_mass){

 //fill in the ground state rest mass array
  Marray.push_back(beam_mass+target_mass);
  Marray.push_back(rad_mass);
  Marray.push_back(hi_mass);
  thebeam_mass=beam_mass;
  thetarget_mass=target_mass;
  //calulate beam-target LorentzVector
  this->beam_energy=beam_energy;
  double be = beam_energy + beam_mass;
  double bp = sqrt(be*be - beam_mass*beam_mass);
  TLorentzVector BT(0.,0.,bp,be+target_mass);
  
  //fill up the LVarray
  LVarray.push_back(BT);
  LVarray.push_back(TLorentzVector(0.,0.,0.,rad_mass));
  LVarray.push_back(TLorentzVector(0.,0.,0.,hi_mass));

  //get the CM to LAB boost vector
  boostv = LVarray[0].BoostVector();
}

RN_SimEvent::RN_SimEvent(double parent_mass,double rad_mass,double hi_mass){
  //fill in the ground state rest mass array
  Marray.push_back(parent_mass);
  Marray.push_back(rad_mass);
  Marray.push_back(hi_mass);

  //fill up the LVarray
  LVarray.push_back(TLorentzVector(0.,0.,0.,parent_mass));
  LVarray.push_back(TLorentzVector(0.,0.,0.,rad_mass));
  LVarray.push_back(TLorentzVector(0.,0.,0.,hi_mass));

  //boost vector is zero for now
}





////////////////////////////////////////////////////////////////////////////
///2 body CM scattering using rapidity//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int RN_SimEvent::radiate_in_CM(const TVector3& rad_V,
			      const double& hi_ex){
  TVector3 radP(rad_V);
  double hi_rmass = Marray[2]+hi_ex;
  double dke = LVarray[0].M() - hi_rmass - Marray[1];
  if(dke <= 0.){
    return false;
  }

  TLorentzVector parent(LVarray[0]);
  parent.Boost(-boostv);

  double E_CM=parent.E();

  double En=(Marray[1]*Marray[1]-hi_rmass*hi_rmass+E_CM*E_CM)/(2*E_CM);
  double Pn_sqr=En*En-Marray[1]*Marray[1];
  
  radP.SetMag(sqrt(Pn_sqr));
  LVarray[1].SetPxPyPzE(radP.X(),radP.Y(),radP.Z(),En);
  LVarray[1].Boost(boostv);
   
  LVarray[2] = LVarray[0] - LVarray[1];
  
  return true;
}

////////////////////////////////////////////////////////////////////////////////////Decay/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int RN_SimEvent::radiate_in_CM(const TLorentzVector& in_LV,
		const TVector3& rad_V,const double& hi_ex){
  //copy the parent nucleus LorentzVector
  LVarray[0] = in_LV;
  TLorentzVector Parent(in_LV);

  //make a copy of the radiated particle direction unit vector
  TVector3 radP(rad_V);
  
  //the kinetic energy available for the reaction is the difference
  //of the parent CM energy and the daughter plus radiated particle rest mass
  
  double hi_rmass = Marray[2] + hi_ex;
  double dke = LVarray[0].M() - hi_rmass - Marray[1];
  if(dke <= 0.){
    return false;
  }

   boostv = Parent.BoostVector();
   Parent.Boost(-boostv);

   double E_CM=Parent.E();
   double Ep=(Marray[1]*Marray[1]-hi_rmass*hi_rmass+E_CM*E_CM)/(2*E_CM);
   double Pp_sqr=Ep*Ep-Marray[1]*Marray[1];
   
   radP.SetMag(sqrt(Pp_sqr));
   LVarray[1].SetPxPyPzE(radP.X(),radP.Y(),radP.Z(),Ep);
   LVarray[1].Boost(boostv);
   
   LVarray[2] = LVarray[0] - LVarray[1];

   
   return true;
}

////////////////////////////////////////////////////////////////////////////
///What happens when the particle enters a P_terphenyl crystal/////////////
///////////////////////////////////////////////////////////////////////////
RN_PTerph::RN_PTerph(){

}
RN_PTerph::RN_PTerph(std::string name,int apos):fName(name),
						fRadius(34),
						fThickness(25.4),
						fThreshold(0.05),
						rnd(0)
{
  
  RNArray::PositionMap(apos,fPos);
  Reset();
}


void RN_PTerph::Reset(){
  fDt=0;
  fT=0;
  fEsum=0;
  fCounter=0;
}

void RN_PTerph::LoadVariables(RN_VariableMap& detvar){
  double temp;  
  detvar.GetParam("all.radius",fRadius);
  detvar.GetParam("all.thickness",fThickness);
  detvar.GetParam("all.threshold",fThreshold);
  if(detvar.GetParam("all.zpos",temp))
    fPos.SetZ(temp);
  detvar.GetParam(Form("%s.radius",fName.c_str()),fRadius);
  detvar.GetParam(Form("%s.thickness",fName.c_str()),fThickness);
  detvar.GetParam(Form("%s.threshold",fName.c_str()),fThreshold);

}

bool RN_PTerph::inDet(const TVector3& v){
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
  if((ch_vect_x+ch_vect_y) > fRadius){
    return false;
  }
  return true;
}

int RN_PTerph::NeutIn(TLorentzVector nLV,double& t,double& e){
  TLorentzVector inLV = nLV;
  double pz= nLV.Pz(); 
  double px=nLV.Px(); 
  double py=nLV.Py();
  double psqr=px*px+py*py+pz*pz;
  double tof=(fPos.Z())*M_N/(pz*3*100);//tof to front of detector
  fDt=0;//time to first reaction
  double x_pos=(px*tof*300/(M_N))-fPos.X();
  double y_pos=(py*tof*300/(M_N))-fPos.Y();
  double radial_pos=sqrt(x_pos*x_pos+y_pos*y_pos);
 
  
  z_pos=fThickness;
  while (radial_pos <= fRadius && z_pos >= 0 && z_pos <= fThickness){
    double nKE=inLV.E()-inLV.M();
    if (nKE<fThreshold/5)
      break;
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
    fT=t;
    return true;
  }
  else{
    e=0;
    t=0;
    return false;
  }

}	  
int RN_PTerph::H_hit(TLorentzVector& inLV){
  double KE = inLV.E()-inLV.M();
  TLorentzVector neut_LVcopy(inLV);
  TLorentzVector Target(0.,0.,0.,M_P);
  TLorentzVector Before(Target + neut_LVcopy);
  TVector3 boostv = (-1)*Before.BoostVector();
  Before.Boost(boostv);
  neut_LVcopy.Boost(boostv);  //this is for getting neutron KE in CM
  double nKE=neut_LVcopy.E()-neut_LVcopy.M();
  double Hprob=0.04508869*(3.76-3.69*log(nKE)*(fThickness/1000)); 
			   
  if(rnd.Rndm()<= Hprob){  //scatters off a proton
    fCounter++;
    if(fCounter==1){fDt=(z_pos-fThickness)*M_N/(neut_LVcopy.Pz()*3*100);}
    double theta = 3.14 * rnd.Rndm(); 
    double phi = 2.* 3.14 * rnd.Rndm(); //isotropic CM
    double psqr=2*M_P*M_N*(Before.E()-M_N-M_P)/(M_P+M_N);
    double pz=-1*sqrt(psqr)*cos(theta);
    double px=-1*sqrt(psqr)*sin(theta)*cos(phi);
    double py=-1*sqrt(psqr)*sin(theta)*sin(phi);
    double e_neut=(psqr/(2*M_N))+M_N;
    inLV.SetPxPyPzE(px,py,pz,e_neut);
    inLV.Boost(-1*boostv);//boost back to lab frame
    fEsum = fEsum + KE - (inLV.E()-inLV.M());
    return true;
  }
  
  return false;

   
}

int RN_PTerph::C_hit(TLorentzVector& inLV){
  double KE = inLV.E()-inLV.M();
  TLorentzVector neut_LVcopy(inLV);
  TLorentzVector Target(0.,0.,0.,M_C);
  TLorentzVector Before(Target + neut_LVcopy);
  TVector3 boostv = (-1)*Before.BoostVector();
  Before.Boost(boostv);
  neut_LVcopy.Boost(boostv);
  double nKE=neut_LVcopy.E()-neut_LVcopy.M();
  double Cprob=0.057971178*(5.01-4.31*nKE)*(fThickness/1000);  
  
  if(rnd.Rndm() <= Cprob){  //scatters off a carbon
      fCounter++;
      if(fCounter==1){fDt=(z_pos-fThickness)*939.56404/(neut_LVcopy.Pz()*3*100);}
      double theta = 3.14 * rnd.Rndm(); 
      double phi = 2.* 3.14 * rnd.Rndm(); //isotropic CM
      double psqr=2*M_C*M_N*(Before.E()-M_N-M_C)/(M_C+M_N);
      double pz=-1*sqrt(psqr)*cos(theta);
      double px=-1*sqrt(psqr)*sin(theta)*cos(phi);
      double py=-1*sqrt(psqr)*sin(theta)*sin(phi);
      double e_neut=(psqr/(2*M_N))+M_N;
      inLV.SetPxPyPzE(px,py,pz,e_neut);
      inLV.Boost(-1*boostv);//boost back to lab frame
      fEsum = fEsum + KE - (inLV.E()-inLV.M());//add what neut loses
      return true;  
  }
  
  return false;
 
}

