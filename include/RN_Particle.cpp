#include "RN_Particle.hpp"

///////////////////////////////////////////////////////////////////////
//Create Particle class (to be manipulated by other "Physics" classes//
///////////////////////////////////////////////////////////////////////

RN_Particle::RN_Particle(const TLorentzVector& lv):LV(lv){
}

RN_Particle::RN_Particle(std::string n):fName(n),  
					ex_energy(0){
					
  mtable.GetParam(n,mass);
  Reset();
}
    
void RN_Particle::Reset(){
  LV.SetPxPyPzE(0.0,0.0,0.0,mass);
}
