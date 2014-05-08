#ifndef SIMPHYSICS_CXX
#define SIMPHYSICS_CXX
#include "RN_SimPhysics.hpp"
#include "RN_Root.hpp"
#define M_N 939.565404 
#define M_P 938.2720137 
#define M_C 11177.92852

namespace sim{
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

}


RN_ParticleGun::RN_ParticleGun(std::string name,
			    double minTheta,
			    double maxTheta,
			    double minKE,
			    double maxKE):thetaMin(minTheta),
					  thetaMax(maxTheta),
					  keMin(minKE),
					  keMax(maxKE)
					 
{
  MassTable.GetParam(name,mass);

}

int RN_ParticleGun::Shoot(TLorentzVector & pLV){
  double theta=(M_PI/180)*(global::myRnd.Rndm()*(thetaMax-thetaMin)+thetaMin);
  double phi=2*M_PI*global::myRnd.Rndm();
  double E=global::myRnd.Rndm()*(keMax-keMin)+keMin;
  double p=sqrt(2*mass*E);
  TVector3 pv;
  pv.SetMagThetaPhi(p,theta,phi);
  pLV.SetPxPyPzE(pv.Px(),pv.Py(),pv.Pz(),mass+E);
  return 1;

}




#endif
