#ifndef RN_REACTIONINFO_CXX
#define RN_REACTIONINFO_CXX

#include <assert.h>
#include "RN_ReactionInfo.hpp"
#include "RN_Root.hpp"
using namespace global;


RN_ReactionInfo::RN_ReactionInfo(const std::string & name):RN_BaseClass(name),
							      fBeamEnergy(0),
							      fBeamELoss(0),
							      fhi_ex_set(0),
							      fd_ex_set(0),
							      fE_fragment_est(0),
							      fIsSet(0)
{
  Clear(); // initalize mArrays
}

bool RN_ReactionInfo::IsSet(){
  if(fIsSet){
    return 1;
  }
  //check if the masses are set and atleast a beam energy
  //and set fIsSet for next time
  else{
   
    if(mArray[0] && 
       mArray[1] && 
       mArray[2] &&
       mArray[3] &&
       mArray[4] &&
       mArray[5] &&
       fBeamEnergy)
      {
	fIsSet = 1;
      }
  }
  return fIsSet;
}


//Set Primary Reaction and automatically add one decay channel
bool RN_ReactionInfo::SetReaction(const std::string & beam, 
				  const std::string & target, 
				  const std::string & recoil, 
				  const std::string & fragment,
				  const std::string & product,
				  const std::string & heavy)
{
  fNames[0] = beam;
  fNames[1] = target;
  fNames[2] = recoil;
  fNames[3] = fragment;
  fNames[4] = product;
  fNames[5] = heavy;

  RNROOT::gMassTable.GetParam(beam,mArray[0]);
  RNROOT::gMassTable.GetParam(target,mArray[1]);
  RNROOT::gMassTable.GetParam(recoil,mArray[2]);
  RNROOT::gMassTable.GetParam(fragment,mArray[3]);
  RNROOT::gMassTable.GetParam(product,mArray[4]);
  RNROOT::gMassTable.GetParam(heavy,mArray[5]);

  if(fBeamEnergy){
    fIsSet = 1;
  }
  return fIsSet;

}
void RN_ReactionInfo::Clear(){
  for (int i=0;i<6;i++){
    mArray[i]=0;
    fLVarray[i].SetPxPyPzE(0,0,0,0);
  }
  fIsSet = 0;
}

void RN_ReactionInfo::SetAngularDistribution(const std::string & filename){
  fDWBA.Init(filename);
}


Double32_t RN_ReactionInfo::GenerateSimEvent(){
  //get a beam energy that is dependent on how deep in the target the reaction took place
  double theta = 0, phi = 0;
  TVector3 nv;
  
  double beam_energy = BeamEnergy()-myRnd.Rndm()*BeamELoss();
  sim::RN_SimEvent evt1(beam_energy,mArray[0],mArray[1],mArray[2],mArray[3]);
  //find an angle in theta and phi that falls under the probability curve given by the angular distribution
  do{
    theta=M_PI*myRnd.Rndm();
    phi = 2.*M_PI*myRnd.Rndm();
    nv.SetMagThetaPhi(1.,theta,phi);
  }while (myRnd.Rndm()>fDWBA.GetAD(180.- (theta / M_PI * 180.)));
  
  //check if there was enough energy and calculate the lab frame LVs
  if(!evt1.radiate_in_CM(nv,fhi_ex_set))
    return 0;
  //set the LorentzVectors
  fLVarray[1].SetPxPyPzE(0,0,0,mArray[1]);
  fLVarray[0] = evt1.getLVin() - fLVarray[1];
  fLVarray[2] = evt1.getLVrad();
  fLVarray[3] = evt1.getLVhi();

  GenerateDecayEvent();

  //return the recoil cm theta
  return theta;
}

bool RN_ReactionInfo::GenerateDecayEvent(){
  //set reaction kinematics			 
  sim::RN_SimEvent evt2(mArray[3],mArray[4],mArray[5]);
  
  double theta = acos(-1. + 2.*myRnd.Rndm());
  double phi = 2.*M_PI*myRnd.Rndm();
  TVector3 pv; pv.SetMagThetaPhi(1.,theta,phi);
  
  //check if the primary reaction fragment is capable of decaying to this state and calculate the decay particle kinematics
  if(!evt2.radiate_in_CM(fLVarray[3],pv,fd_ex_set)){
    return 0;
  }

  fLVarray[4] = evt2.getLVrad();
  fLVarray[5] = evt2.getLVhi();
  
  return 1;
}


void RN_ReactionInfo::Reset(){
  for(int i=0;i<6;i++){
    fLVarray[i].SetPxPyPzE(0,0,0,0);
  }

}

void RN_ReactionInfo::SetCalibrations(RN_VariableMap& detvar){

  detvar.GetParam(Form("%s.E_fragment",GetName()),fE_fragment_est);
  detvar.GetParam(Form("%s.beam_e",GetName()),fBeamEnergy);
  detvar.GetParam(Form("%s.beam_eloss",GetName()),fBeamELoss);
  detvar.GetParam(Form("%s.hi_ex_set",GetName()),fhi_ex_set);
  detvar.GetParam(Form("%s.d_ex_set",GetName()),fd_ex_set);


}

Double32_t RN_ReactionInfo::DecayQValueExact(){
  double d_ke=DecayProductLV().E() - DecayProductLV().M();
  double d_theta=DecayProductLV().Theta();
  double hi_ke=FragmentLV().E()-FragmentLV().M();
  return (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	  - (hi_ke * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	  - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * hi_ke * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_ReactionInfo::DecayQValueEstimate(){
  double d_ke=DecayProductLV().E()-DecayProductLV().M();
  double d_theta=DecayProductLV().Theta();
  return (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	  - (fE_fragment_est * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	  - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * fE_fragment_est * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_ReactionInfo::DecayQValueEstimate(const double& decay_ke,const double& decay_theta /*inradians*/){
 Double32_t q = (decay_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	 - (fE_fragment_est * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	 - ((2 / M_Heavy_Decay()) * TMath::Sqrt(decay_ke * M_Decay_Product() * fE_fragment_est * M_Fragment()) * cos(decay_theta)));

 return q;

}

Double32_t RN_ReactionInfo::DecayQValueIterations(const double& decay_ke, const double & decay_theta /*inradians*/,const int& iterations){
  Double32_t q = DecayQValueEstimate(decay_ke,decay_theta);
  if(iterations==0){
    return q;
  }

  double p_beam,alpha,beta;
  double p_rec,e_rec;
  alpha = ((1./M_Heavy_Decay()) - (1./M_Decay_Product()));
  beta = ((1./M_Fragment()) + (1./M_Decay_Product()));
  p_beam = sqrt(2*BeamEnergy_Est()*M_Beam());
  
  //  std::cout<<fE_fragment_est<<" "<<q<<"\n";

  for(unsigned int i=0;i<iterations;i++){
    p_rec = p_beam / (beta*M_Decay_Product()) + sqrt(pow((p_beam) / (beta*M_Decay_Product()),2.) + 2*(-2.2-q)/beta + pow(p_beam,2.)* alpha/ beta );
    e_rec = pow(p_rec,2.)/(2*M_Fragment());
    
    q = (decay_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
      - (e_rec * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	 - ((2 / M_Heavy_Decay()) * TMath::Sqrt(decay_ke * M_Decay_Product() * e_rec * M_Fragment()) * cos(decay_theta)));
    //    std::cout<<e_rec<<" "<<q<<"\n";

  }
  return q;





}




//Calculate the Q_value given the time of flight of the recoil. 
Double32_t RN_ReactionInfo::RecoilQValue(const double& deltaz/*mm*/, const double& tof /*ns*/,double& NKE,double& hiKE){
  double M_N=M_Recoil();
  double M1=M_Beam();
  double M2=M_Fragment();
  NKE=.5*M_N*(deltaz*deltaz/(tof*tof*90000.0));
  hiKE=(M1/M2)*BeamEnergy_Est()+((M_N/M2)*NKE)+((2/M2)*sqrt((BeamEnergy_Est()*NKE*M_N*M1)));
  
  return (NKE+hiKE-BeamEnergy_Est()); 
}

TGraph RN_ReactionInfo::GetCurve(int Points){
 return GetCurve(Points,this->fhi_ex_set);
}
TGraph RN_ReactionInfo::GetCurve(int Points, const double& hi_ex_set){ 
  TGraph curve;
  if(!IsSet()){
    std::cout<<"Reaction Masses have not been set"<<std::endl;
    return curve;
  }
 
  sim::RN_SimEvent evt1(fBeamEnergy,mArray[0],mArray[1],mArray[2],mArray[3]); 
  // Fill the points of the kinematic curve
  int p=0;
  while(p<Points){
    double theta_deg = 180.0*p/Points;
    double phi=2.*M_PI*global::myRnd.Rndm();

    TVector3 nv; nv.SetMagThetaPhi(1.,theta_deg*M_PI/180.0,phi);
    if(!evt1.radiate_in_CM(nv,hi_ex_set))
      continue;
    else 
      curve.SetPoint(p, evt1.getLVrad().Theta()*180/3.14,(double)(evt1.getLVrad().E()-evt1.getLVrad().M()));      
    p++;
  }
  // end for(p)    

  return curve;

}
TGraph RN_ReactionInfo::GetSecondaryDecayCurve(int Points){
  return GetSecondaryDecayCurve(Points,this->fhi_ex_set,this->fd_ex_set);
}

TGraph RN_ReactionInfo::GetSecondaryDecayCurve(int Points,const double & hi_ex_set,const double& decay_ex_set){
  TGraph curve;
  if(!IsSet()){
    std::cout<<"Reaction Masses have not been set"<<std::endl;
    return curve;
  }
  
  sim::RN_SimEvent evt1(fBeamEnergy,mArray[0],mArray[1],mArray[2],mArray[3]);
  sim::RN_SimEvent evt2(mArray[3],mArray[4],mArray[5]);
  // Fill the points of the kinematic curve
  int p=0;
  while(p<Points){
    double theta_deg = 175; // assume backward angle from inverse kinematics
    double phi=2.*M_PI*global::myRnd.Rndm();
    
    TVector3 nv; nv.SetMagThetaPhi(1.,theta_deg*M_PI/180.0,phi);
    if(!evt1.radiate_in_CM(nv,hi_ex_set))
      continue;

    theta_deg = 180* p / Points;
    phi = 2.*M_PI*global::myRnd.Rndm();
    TVector3 pv; pv.SetMagThetaPhi(1.,theta_deg*M_PI/180.0,phi);
    if(!evt2.radiate_in_CM(evt1.getLVhi(),pv,fd_ex_set))
      continue;

    curve.SetPoint(p, evt2.getLVrad().Theta()*180/3.14,(double)(evt2.getLVrad().E() - evt2.getLVrad().M()));
    p++;
  }
  // end for(p)    

  return curve;
}



#endif
