#ifndef __RNPARTICLE_CXX
#define __RNPARTICLE_CXX

#include "RN_Particle.hpp"
#include "RN_Root.hpp"
///////////////////////////////////////////////////////////////////////
//Create Particle class (to be manipulated by other "Physics" classes//
///////////////////////////////////////////////////////////////////////

RN_Particle::RN_Particle(const std::string& n,const double& ex):RN_BaseClass(n,n),  
								ex_energy(ex){
  Init(n);
  
								}

void RN_Particle::Reset(){
  LV.SetPxPyPzE(0.0,0.0,0.0,mass);
}

void RN_Particle::Init(const std::string & pname){
  TNamed::SetName(pname.c_str());
  TNamed::SetTitle(pname.c_str());
  RNROOT::gMassTable.GetParam(pname,mass);
  if (mass==0){
    std::cout<<"no entry found in mass table for this particle\n";
    std::cout<<"enter mass below or 0 to force quit program"<<std::endl;
    std::cin>>mass;
    if(!mass){
      std::cout<<"exiting..."<<std::endl;
      exit(EXIT_FAILURE);
    } 
  }
  Reset();
}


#endif
