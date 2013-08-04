#include "RN_Sim.hpp"

RN_Sim::RN_Sim():myRnd(0){}

void RN_Sim::Init(){
  
  plist.push_back(RN_Particle("12C"));
  plist.push_back(RN_Particle("d"));
  plist.push_back(RN_Particle("n"));
  plist.push_back(RN_Particle("13N"));
  plist.push_back(RN_Particle("p"));
  plist.push_back(RN_Particle("12C"));
  
  pterph.push_back(RN_PTerph("neut0",1));
  pterph.push_back(RN_PTerph("neut1",4));
  pterph.push_back(RN_PTerph("neut2",5));
  pterph.push_back(RN_PTerph("neut3",6));
  pterph.push_back(RN_PTerph("neut4",7));
  pterph.push_back(RN_PTerph("neut5",11));
  pterph.push_back(RN_PTerph("neut6",14));
  pterph.push_back(RN_PTerph("neut7",10));
  pterph.push_back(RN_PTerph("neut8",12));
  pterph.push_back(RN_PTerph("neut9",15));
  pterph.push_back(RN_PTerph("neut10",2));
  pterph.push_back(RN_PTerph("neut11",3));
  pterph.push_back(RN_PTerph("neut12",8));
  pterph.push_back(RN_PTerph("neut13",9));
  pterph.push_back(RN_PTerph("neut14",13));
  pterph.push_back(RN_PTerph("neut15",16));
  
  s2_det.push_back(RN_S2Detector("si_a",16,16));
  s2_det.push_back(RN_S2Detector("si_b",16,16));
}


void RN_Sim::SetAngularDistribution(std::string filename){
  nDWBA = RN_AngularDistribution(filename);
}

void RN_Sim::LoadVariableFile(std::string a){
  global.LoadParams(a);
}

void RN_Sim::SetVariables(){
  global.GetParam("sim.q1set",q1set);
  global.GetParam("sim.q2set",q2set);
  global.GetParam("sim.q3set",q3set);
  global.GetParam("sim.q4set",q4set);
  global.GetParam("sim.beam_energy",beam_energy);
  global.GetParam("sim.beam_eloss",beam_eloss);
  global.GetParam("sim.hi_ex_set",hi_ex_set);
  global.GetParam("sim.d_ex_set",d_ex_set);
  
  for(RN_PTerphCollectionRef it=pterph.begin();it!=pterph.end();it++){ 
    (*it).LoadVariables(global);  
  }
  
  for(RN_S2CollectionRef it=s2_det.begin();it!=s2_det.end();it++){
    (*it).SetCalibrations(global); 
  }


}

int RN_Sim::GenerateEvents(Long64_t evnum,std::string options=""){
  
  beam_e = beam_energy - myRnd.Rndm() * beam_eloss;
  
  RN_SimEvent evt1(beam_e,plist[0].mass,plist[1].mass,plist[2].mass,plist[3].mass);
  RN_SimEvent evt2(plist[3].mass,plist[4].mass,plist[5].mass);
  
  double theta=M_PI*myRnd.Rndm();
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
  double decay_ex = q1set + q2set - q3set + d_ex_set;
  if(!evt2.radiate_in_CM(evt1.getLVhi(),pv,decay_ex)){
    if(options=="proton")
      return 0;
  }
  plist[1].LV.SetPxPyPzE(0.0,0.0,0.0,plist[1].mass);
  plist[0].LV = evt1.getLVin() - plist[1].LV;
  plist[2].LV = evt1.getLVrad();
  plist[3].LV = evt1.getLVhi();
  plist[4].LV = evt2.getLVrad();
  plist[5].LV = evt2.getLVhi();
   
  for(RN_PTerphCollectionRef it=pterph.begin();it!=pterph.end();it++){
    if((*it).inDet(plist[2].LV.Vect()))
      (*it).NeutIn(plist[2].LV,n_tof,E_deposited);  
  }

  for(RN_S2CollectionRef it=s2_det.begin();it!=s2_det.end();it++){
    if((*it).inDet(plist[4].LV.Vect())){
      double e=plist[4].LV.E()-plist[4].LV.M();
      double t=0;
      (*it).front.InsertHit(e,t,0);
    }
  }
  
  
  return 1;
}   


void RN_Sim::Reset(){
  E_deposited=0;
  n_tof=0;
   
  for(RN_PTerphCollectionRef it=pterph.begin();it!=pterph.end();it++){ 
    (*it).Reset();  
  }

  for(RN_S2CollectionRef it=s2_det.begin();it!=s2_det.end();it++){
    (*it).Reset(); 
  }
  for(RN_ParticleCollectionRef it=plist.begin();it!=plist.end();it++){
    (*it).Reset();
  }
  
}
