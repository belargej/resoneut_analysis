#ifndef RN_REACTIONINFO_H
#define RN_REACTIONINFO_H

#include "RN_ReactionInfo.hpp"

RN_PrimaryReaction::RN_PrimaryReaction(TString beam, TString target, TString recoil, TString fragment){
  
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);
  
}

RN_PrimaryReaction::SetReaction(TString beam, 
				TString target, 
				TString recoil, 
				TString fragment){
		       
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);

}

//Set Primary Reaction and automatically add one decay channel
RN_PrimaryReaction::SetReaction(TString beam, 
				TString target, 
				TString recoil, 
				TString fragment,
				TString product,
				TString heavy){
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);
  AddDecayChannel(product,heavy);

}

RN_PrimaryReaction::AddDecayChannel(TString decay, TString heavy){
  //create new decay channel
  RN_DecayChannel * decaychan = new RN_DecayChannel(this,decay,heavy);
  if(!fDecayChannels)
    fDecayChannels =new TList();
  
  fDecayChannels->Add(decaychan);
  
  
}

void RN_PrimaryReaction::SetAngularDistribution(const std::string & filename){
  fDWBA = RN_AngularDistribution(filename);
}

Double32_t RN_PrimaryReaction::M_Decay_Product(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->M_Decay_Product();
}


Double32_t RN_PrimaryReaction::M_Heavy_Decay(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->M_Heavy_Decay();
}

TLorentzVector& RN_PrimaryReaction::DecayProductLV(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->DecayProductLV();
}

TLorentzVector& RN_PrimaryReaction::HeavyDecayLV(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->HeavyDecayLV();
}

RN_Particle& RN_PrimaryReaction::DecayProduct(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->DecayProduct();
}

RN_Particle& RN_PrimaryReaction::HeavyDecay(int i){
  if(i < GetListOfDecayChannels().GetSize())
    return GetListOfDecayChannels()->At(i)->HeavyDecay();
}


bool RN_PrimaryReaction::GenerateSimEvent(){
  //get a beam energy that is dependent on how deep in the target the reaction took place
  do{
    double beam_energy = BeamEnergy()-myRnd.Rndm()*BeamELoss();
    sim::RN_SimEvent evt1(beam_energy,M_Beam(),M_Target(),M_Recoil(),M_Fragment());
    TVector3 nv;
    double theta;
    double phi;
    
    //find an angle in theta and phi that falls under the probability curve given by the angular distribution
    do{
      theta=M_PI*myRnd.Rndm();
      n_cm=theta;
    phi = 2.*M_PI*myRnd.Rndm();
    }while (myRnd.Rndm()>fDWBA.GetAD(180.- (theta / M_PI * 180.)));
  }while(!evt1.radiate_in_CM(nv,Ex_Fragment()));
  
  //set the particle LorentzVectors
  pBeam.LV = evt1.getLVin() - pTarget.LV;
  pRecoil.LV = evt1.getLVrad();
  pFragment.LV = evt1.getLVhi();
  
  return 1;
}

bool RN_PrimaryReaction::GenerateDecayEvents(){
  for(unsigned int i=0;i<GetListOfDecayChannels().GetSize();i++){
    GetListOfDecayChannels()->At(i)->GenerateDecayEvent();
  }
  return 1;
}


void RN_PrimaryReaction::Reset(){
  for(unsigned int i = 0; i<GetListOfDecayChannels();i++){
    GetListOfDecayChannels()->At(i)->Reset();
  }
  
  pBeam.Reset();
  pRecoil.Reset();
  pFragment.Reset();
}

void RN_PrimaryReaction::SetCalibrations(RN_VariableMap& detvar){
  double temp(0);
  detvar.GetParam(Form("%s.E_fragment",GetName()),E_fragment_est);
  detvar.GetParam(Form("%s.hi_ex_set",GetName()),temp);
  pFragment.SetExEnergy(temp);
}

Double32_t RN_PrimaryReaction::DecayQValueExact(){
  double d_ke=DecayProduct().KE();
  double d_theta=DecayProduct().LV.Theta();
  double hi_ke=Fragment().KE();
  (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
   - (hi_ke * ( 1 - (M_Fragment() / M_Heavy_Decay())))
   - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * hi_ke * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_PrimaryReaction::DecayQValueEstimate(){
  double d_ke=DecayProduct().KE();
  double d_theta=DecayProduct().LV.Theta();
  double hi_ke=Fragment().KE();
  (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
   - (E_fragment_est * ( 1 - (M_Fragment() / M_Heavy_Decay())))
   - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * E_fragment_est * M_Fragment()) * cos(d_theta)));
}

//Calculate the Q_value given the time of flight of the recoil. 
Double32_t RN_PrimaryReaction::RecoilQValue(const double& deltaz/*mm*/, const double& tof /*ns*/,const double& NKE, const double& hiKE){
  double M_N=M_Recoil();
  double M1=M_Beam();
  double M2=M_Fragment();
  NKE=.5*M_N*(deltaz*deltaz/(tof*tof*90000.0));
  hiKE=(M1/M2)*BeamEnergy_Est()+((M_N/M2)*NKE)+((2/M2)*sqrt((BeamEnergy_Est()*NKE*M_N*M1)));
  
  return NKE+hiKE-beam_est;
  
}



RN_DecayChannel::RN_DecayChannel(RN_PrimaryReaction* parent, TString decay, TString heavy){
  Init(parent,decay,heavy); 
}

void RN_DecayChannel::Init(RN_PrimaryReaction* parent, TString decay, TString heavy){
  fParentReaction = parent;
  pProduct.Init(decay);
  pHIDecay.Init(heavy);
  
}

bool RN_DecayChannel::GenerateDecayEvent(){
  //set reaction kinematics
  sim::RN_SimEvent evt2(fMotherReaction->M_Fragment(),pProduct.M(),pHIDecay.M());

  theta = acos(-1. + 2.*myRnd.Rndm());
  phi = 2.*M_PI*myRnd.Rndm();
  TVector3 pv; pv.SetMagThetaPhi(1.,theta,phi);

  //check if the primary reaction fragment is capable of decaying to this state and calculate the decay particle kinematics
  if(!evt2.radiate_in_CM(fMotherReaction->FragmentLV(),pv,fExEnergy)){
    return 0;
  }

  pProduct = evt2.getLVrad();
  pHIDecay = evt2.getLVhi();
  return 1;
}

void RN_DecayChannel::Reset(){
  pProduct.Reset();
  pHIDecay.Reset();

}


#endif
