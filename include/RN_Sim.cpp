#ifndef RNSIM_CXX
#define RNSIM_CXX

#include "RN_Sim.hpp"
#include "RN_Root.hpp"
using namespace global;
namespace sim{

  RN_ParticleGun *particlegun;

  void RN_Sim::Init(){
    
  }
  
  void RN_Sim::SetAngularDistribution(std::string filename){
    nDWBA = RN_AngularDistribution(filename);
  }
  
  int RN_Sim::GenerateSingleParticleEvent(Long64_t evnum){
    Reset();
    particlegun->Shoot(particle[0].LV);
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if((*it).inDet(particle[0].LV.Vect())){
	(*it).NeutIn(particle[0].LV,n_tof,E_deposited);  
	return 1;
      }
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      if((*it).inDet(particle[0].LV.Vect())){
	double e=particle[0].LV.E()-particle[0].LV.M();
	double t=0;
	(*it).front.InsertHit(e,t,0);
	return 1;
      }
    }

    return 0;
  }
  

int RN_Sim::GenerateEvents(Long64_t evnum,std::string options=""){
  Reset();

  double beam_energy = beam_e - myRnd.Rndm() * beam_eloss;
  
  RN_SimEvent evt1(beam_energy,particle[0].mass,particle[1].mass,particle[2].mass,particle[3].mass);
  RN_SimEvent evt2(particle[3].mass,particle[4].mass,particle[5].mass);
  
  double theta=M_PI*myRnd.Rndm();
  n_cm=theta;
  double phi = 2.*M_PI*myRnd.Rndm();
  if (myRnd.Rndm()>nDWBA.GetAD(180.- (theta / M_PI * 180.)))
    return 0;
  TVector3 nv; nv.SetMagThetaPhi(1.,theta,phi);//neutron 
  
  if(!evt1.radiate_in_CM(nv,hi_ex_set))
    return 0;
  
  
  theta = acos(-1. + 2.*myRnd.Rndm());
  phi = 2.*M_PI*myRnd.Rndm();
  TVector3 pv; pv.SetMagThetaPhi(1.,theta,phi);
  //q1set=-5.518,q2set=3.293,q3set=cancelsoutq1q2
  if(!evt2.radiate_in_CM(evt1.getLVhi(),pv,d_ex_set)){
    if(options=="proton")
      return 0;
  }
  particle[1].LV.SetPxPyPzE(0.0,0.0,0.0,particle[1].mass);
  particle[0].LV = evt1.getLVin() - particle[1].LV;
  particle[2].LV = evt1.getLVrad();
  particle[3].LV = evt1.getLVhi();
  particle[4].LV = evt2.getLVrad();
  particle[5].LV = evt2.getLVhi();
   
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if((*it).inDet(particle[2].LV.Vect()))
      (*it).NeutIn(particle[2].LV,n_tof,E_deposited);  
  }

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    if((*it).inDet(particle[4].LV.Vect())){
      double e=particle[4].LV.E()-particle[4].LV.M();
      double t=0;
      (*it).front.InsertHit(e,t,0);
    }
  }
  
  
  return 1;
}   

double RN_Sim::QValue(const double deltaz/*mm*/, const double tof /*ns*/,double& NKE, double& hiKE){
  double M_N=particle[2].mass;
  double M1=particle[0].mass;
  double M2=particle[3].mass;
  NKE=.5*M_N*(deltaz*deltaz/(tof*tof*90000.0));
  hiKE=(M1/M2)*beam_est+((M_N/M2)*NKE)+((2/M2)*sqrt((beam_est*NKE*M_N*M1)));
  
  return NKE+hiKE-beam_est;

}

void RN_Sim::Reset(){
  E_deposited=0;
  n_tof=0;
   
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
    (*it).Reset();  
  }

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).Reset(); 
  }
  for(RN_ParticleCollectionRef it=particle.begin();it!=particle.end();it++){
    (*it).Reset();
  }
  
}
}


#endif
