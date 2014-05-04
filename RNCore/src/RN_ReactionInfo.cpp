#ifndef RN_REACTIONINFO_CXX
#define RN_REACTIONINFO_CXX

#include "RN_ReactionInfo.hpp"
#include "RN_Root.hpp"
using namespace global;


RN_PrimaryReaction::RN_PrimaryReaction(const std::string & beam, const std::string & target, const std::string & recoil, const std::string & fragment){
  
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);
  fIsSet = 1;
}

void RN_PrimaryReaction::SetReaction(const std::string & beam, 
				const std::string & target, 
				const std::string & recoil, 
				const std::string & fragment){
		       
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);
  fIsSet =1;
}

//Set Primary Reaction and automatically add one decay channel
void RN_PrimaryReaction::SetReaction(const std::string & beam, 
				const std::string & target, 
				const std::string & recoil, 
				const std::string & fragment,
				const std::string & product,
				const std::string & heavy){
  pBeam.Init(beam);
  pTarget.Init(target);
  pRecoil.Init(recoil);
  pFragment.Init(fragment);
  AddDecayChannel(product,heavy);
  fIsSet = 1;
}
void RN_PrimaryReaction::Clear(){
  pBeam.Clear();
  pTarget.Clear();
  pRecoil.Clear();
  pFragment.Clear();
  
  //need to add delete the decay channels
  if(fDecayChannels)fDecayChannels->Clear();

  fIsSet = 0;
}


RN_DecayChannel* RN_PrimaryReaction::AddDecayChannel(const std::string & decay, const std::string & heavy){
  //create new decay channel
  RN_DecayChannel * decaychan = new RN_DecayChannel(this,decay,heavy);
  if(!fDecayChannels)
    fDecayChannels =new TList();
  
  fDecayChannels->Add(decaychan);
  
  return decaychan;
}

void RN_PrimaryReaction::SetAngularDistribution(const std::string & filename){
  fDWBA = sim::RN_AngularDistribution(filename);
}

Double32_t RN_PrimaryReaction::M_Decay_Product(int i){
  if(GetListOfDecayChannels() && i < GetListOfDecayChannels()->GetSize())
    return ((RN_DecayChannel*)GetListOfDecayChannels()->At(i))->M_Decay_Product();
  else 
    return 0;
}


Double32_t RN_PrimaryReaction::M_Heavy_Decay(int i){
  if(GetListOfDecayChannels() && i < GetListOfDecayChannels()->GetSize())
    return ((RN_DecayChannel *)GetListOfDecayChannels()->At(i))->M_Heavy_Decay();
  else 
    return 0;
}

TLorentzVector& RN_PrimaryReaction::DecayProductLV(int i){
  
  return ((RN_DecayChannel *)GetListOfDecayChannels()->At(i))->DecayProductLV();
}

TLorentzVector& RN_PrimaryReaction::HeavyDecayLV(int i){
  
  return ((RN_DecayChannel *)GetListOfDecayChannels()->At(i))->HeavyDecayLV();
  
}

RN_Particle& RN_PrimaryReaction::DecayProduct(int i){

  return ((RN_DecayChannel *)GetListOfDecayChannels()->At(i))->DecayProduct();
}

RN_Particle& RN_PrimaryReaction::HeavyDecay(int i){
  
  return ((RN_DecayChannel *)GetListOfDecayChannels()->At(i))->HeavyDecay();
 
}


Double32_t RN_PrimaryReaction::GenerateSimEvent(){
  //get a beam energy that is dependent on how deep in the target the reaction took place
  double theta(0);
  TVector3 nv;
  double phi(0);
  sim::RN_SimEvent evt1;
  do{
    double beam_energy = BeamEnergy()-myRnd.Rndm()*BeamELoss();
    evt1.Init(beam_energy,M_Beam(),M_Target(),M_Recoil(),M_Fragment());
    //find an angle in theta and phi that falls under the probability curve given by the angular distribution
    do{
      theta=M_PI*myRnd.Rndm();
      phi = 2.*M_PI*myRnd.Rndm();
      nv.SetMagThetaPhi(1.,theta,phi);
    }while (myRnd.Rndm()>fDWBA.GetAD(180.- (theta / M_PI * 180.)));
  }while(!evt1.radiate_in_CM(nv,Ex_Fragment()));
  
  //set the particle LorentzVectors
  pBeam.LV = evt1.getLVin() - pTarget.LV;
  pRecoil.LV = evt1.getLVrad();
  pFragment.LV = evt1.getLVhi();
  
  return theta;
}

bool RN_PrimaryReaction::GenerateDecayEvents(){
  for(int i=0;i<GetListOfDecayChannels()->GetSize();i++){
    ((RN_DecayChannel*)GetListOfDecayChannels()->At(i))->GenerateDecayEvent();
  }
  return 1;
}


void RN_PrimaryReaction::Reset(){
  for(int i = 0; i<GetListOfDecayChannels()->GetSize();i++){
    ((RN_DecayChannel*)GetListOfDecayChannels()->At(i))->Reset();
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
  return (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	  - (hi_ke * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	  - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * hi_ke * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_PrimaryReaction::DecayQValueEstimate(){
  double d_ke=DecayProduct().KE();
  double d_theta=DecayProduct().LV.Theta();
  return (d_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	  - (E_fragment_est * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	  - ((2 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * E_fragment_est * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_PrimaryReaction::DecayQValueEstimate(const double& decay_ke,const double& decay_theta /*inradians*/){
  return(decay_ke* (1 + (M_Decay_Product() / M_Heavy_Decay())) 
	 - (E_fragment_est * ( 1 - (M_Fragment() / M_Heavy_Decay())))
	 - ((2 / M_Heavy_Decay()) * TMath::Sqrt(decay_ke * M_Decay_Product() * E_fragment_est * M_Fragment()) * cos(decay_theta)));
}


//Calculate the Q_value given the time of flight of the recoil. 
Double32_t RN_PrimaryReaction::RecoilQValue(const double& deltaz/*mm*/, const double& tof /*ns*/,double& NKE,double& hiKE){
  double M_N=M_Recoil();
  double M1=M_Beam();
  double M2=M_Fragment();
  NKE=.5*M_N*(deltaz*deltaz/(tof*tof*90000.0));
  hiKE=(M1/M2)*BeamEnergy_Est()+((M_N/M2)*NKE)+((2/M2)*sqrt((BeamEnergy_Est()*NKE*M_N*M1)));
  
  return (NKE+hiKE-BeamEnergy_Est()); 
}



RN_DecayChannel::RN_DecayChannel(RN_PrimaryReaction* parent, const std::string & decay, const std::string & heavy){
  Init(parent,decay,heavy); 
}

void RN_DecayChannel::Init(RN_PrimaryReaction* parent, const std::string & decay, const std::string & heavy){
  fParentReaction = parent;
  pProduct.Init(decay);
  pHIDecay.Init(heavy);
  
}

bool RN_DecayChannel::GenerateDecayEvent(){
  //set reaction kinematics
  sim::RN_SimEvent evt2(fParentReaction->M_Fragment(),pProduct.M(),pHIDecay.M());

  double theta = acos(-1. + 2.*myRnd.Rndm());
  double phi = 2.*M_PI*myRnd.Rndm();
  TVector3 pv; pv.SetMagThetaPhi(1.,theta,phi);

  //check if the primary reaction fragment is capable of decaying to this state and calculate the decay particle kinematics
  if(!evt2.radiate_in_CM(fParentReaction->FragmentLV(),pv,fExEnergy)){
    return 0;
  }

  pProduct.LV = evt2.getLVrad();
  pHIDecay.LV = evt2.getLVhi();
  return 1;
}

void RN_DecayChannel::Reset(){
  pProduct.Reset();
  pHIDecay.Reset();

}


#endif
