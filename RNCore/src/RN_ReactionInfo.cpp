#ifndef RN_REACTIONINFO_CXX
#define RN_REACTIONINFO_CXX

#include <assert.h>
#include "RN_ReactionInfo.hpp"
#include "RN_Root.hpp"
using namespace global;


RN_ReactionInfo::RN_ReactionInfo(const std::string & name):RN_BaseClass(name),
							   fBeamEnergy(0),
							   fBeamELoss(0),
							   fBeamSpread(0),
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
    //theta=M_PI*myRnd.Rndm();
    theta = acos(-1+2.0*myRnd.Rndm());
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
  detvar.GetParam(Form("%s.beam_spread",GetName()),fBeamSpread);
  
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
  return (d_ke* (1.0 + (M_Decay_Product() / M_Heavy_Decay())) 
	  - (fE_fragment_est * ( 1.0 - (M_Fragment() / M_Heavy_Decay())))
	  - ((2.0 / M_Heavy_Decay()) * TMath::Sqrt(d_ke * M_Decay_Product() * fE_fragment_est * M_Fragment()) * cos(d_theta)));
}

Double32_t RN_ReactionInfo::DecayQValueEstimate(const double& decay_ke,const double& decay_theta /*inradians*/){
  double a = (1.0 + (M_Decay_Product() / M_Heavy_Decay()));
  double b = (1.0 - (M_Fragment() / M_Heavy_Decay()));
  double c = (2.0 / M_Heavy_Decay());
  double d = TMath::Sqrt(decay_ke * M_Decay_Product() * fE_fragment_est * M_Fragment());
  double q = (decay_ke * a) - (fE_fragment_est * b ) - (c * d * TMath::Cos(decay_theta));
  
 return q;

}

Double32_t RN_ReactionInfo::DecayQValueIterations(const double& decay_ke, const double & decay_theta /*inradians*/,const UInt_t& iterations){
  Double32_t q = DecayQValueEstimate(decay_ke,decay_theta);
  if(iterations==0){
    return q;
  }
  //std::cout << " Beam Energy : " << fBeamEnergy << std::endl;
  //std::cout << "Name : " << GetName() << std::endl;
 
  /*
  std::cout << " Decay Iteration Checker -------------------------" << std::endl;
  std::cout << "> Proton Energy In Si : " << decay_ke << std::endl;
  std::cout << "> Proton Angle  In Si : " << decay_theta << std::endl;
  std::cout << "> Number of its       : " << iterations << std::endl;
  std::cout << "> Beam Est Energy     : " << BeamEnergy_Est()  << std::endl;
  std::cout << "> Mass Beam           : " << M_Beam()  << std::endl;
  std::cout << "> Mass Target         : " << M_Target()  << std::endl;
  std::cout << "> Mass recoil         : " << M_Recoil()  << std::endl;
  std::cout << "> Mass Fragment       : " << M_Fragment()  << std::endl;
  std::cout << "> Mass product        : " << M_Decay_Product()  << std::endl;
  std::cout << "> Mass heavy          : " << M_Heavy_Decay()  << std::endl;
  */

  //std::cout << "Proton Energy In Iterations : " << decay_ke << std::endl;

  double p_beam,alpha,beta;
  double p_rec,e_rec;
  // Used proton mass
  //alpha = ((1./M_Heavy_Decay()) - (1./M_Decay_Product()));
  //beta = ((1./M_Fragment()) + (1./M_Decay_Product()));

  // Using neutron mass
  alpha = ((1./M_Heavy_Decay()) - (1./M_Recoil()));
  beta = ((1./M_Fragment()) + (1./M_Recoil()));
	   

  p_beam = sqrt(2*BeamEnergy_Est()*M_Beam());
  
  //std::cout << " Beam Energy Estimate : " << BeamEnergy_Est() << std::endl;
  /*
  std::cout << "> Alpha               : " << alpha  << std::endl;
  std::cout << "> Beta                : " << beta  << std::endl;
  std::cout << "> p_beam              : " << p_beam  << std::endl;
  */
  //  std::cout<<fE_fragment_est<<" "<<q<<"\n";

  for(unsigned int i=0;i<iterations;i++){
    
    // Used Proton Mass
    /*p_rec = p_beam / (beta*M_Decay_Product()) + sqrt(pow((p_beam) / (beta*M_Decay_Product()),2.) + 2.0*(-2.2-q)/beta + pow(p_beam,2.)* alpha/ beta );
    e_rec = pow(p_rec,2.)/(2.0*M_Fragment());
    
    q = (decay_ke* (1.0 + (M_Decay_Product() / M_Heavy_Decay())) 
      - (e_rec * ( 1.0 - (M_Fragment() / M_Heavy_Decay())))
	 - ((2.0 / M_Heavy_Decay()) * TMath::Sqrt(decay_ke * M_Decay_Product() * e_rec * M_Fragment()) * cos(decay_theta)));

    */

    // Using Neutron Mass 
    p_rec = p_beam / (beta*M_Recoil()) + sqrt(pow((p_beam) / (beta*M_Recoil()),2.) + 2.0*(-2.2-q)/beta + pow(p_beam,2.)* alpha/ beta );
    e_rec = pow(p_rec,2.)/(2.0*M_Fragment());
    
    q = (decay_ke* (1.0 + (M_Recoil() / M_Heavy_Decay())) 
      - (e_rec * ( 1.0 - (M_Fragment() / M_Heavy_Decay())))
	 - ((2.0 / M_Heavy_Decay()) * TMath::Sqrt(decay_ke * M_Recoil() * e_rec * M_Fragment()) * cos(decay_theta)));

    //    std::cout<<e_rec<<" "<<q<<"\n";
    /*
   std::cout << ">   Trip " << i << " around loop " << std::endl;
    std::cout <<" >    p_rec           : " << p_rec << std::endl;
    std::cout <<" >    e_rec           : " << e_rec << std::endl;
    std::cout <<" >    q               : " << q << std::endl;
    */

  }
  return q;





}Double32_t RN_ReactionInfo::DecayQValueIterations_RecEn(const double& decay_ke, const double & decay_theta /*inradians*/,const UInt_t& iterations){
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
  return e_rec;





}


//JAB
Double32_t RN_ReactionInfo::IntHeavyQVal(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t ExcitationEnergy = 0;

  if(DEBUG){
    std::cout << "---- 4Vec QVal Recon Check ------------------" << std::endl;
    std::cout << "> Light KE : " << LightKE << std::endl;
    std::cout << "> Light Th : " << LightTheta << std::endl;
    std::cout << "> Light Phi: " << LightPhi << std::endl;
    std::cout << "> Heavy KE : " << HeavyKE << std::endl;
    std::cout << "> Heavy Th : " << HeavyTheta << std::endl;
    std::cout << "> Heavy Phi: " << HeavyPhi << std::endl;

  }


  //std::cout << "Proton Energy In Invariant : " << LightKE << std::endl;


  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;

  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh,Ml,Mh;
  Ml = M_Decay_Product();
  Mh = M_Heavy_Decay();
  
  El = LightKE+Ml;
  Eh = HeavyKE+Mh;
  Pl = TMath::Sqrt(El*El-Ml*Ml);
  Ph = TMath::Sqrt(Eh*Eh-Mh*Mh);
  
  //El = LightKE + M_Decay_Product();
  //Eh = HeavyKE + M_Heavy_Decay();
  //Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  //Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  if(DEBUG){
    std::cout << "> P_Light  : " << Pl << std::endl;
    std::cout << "> E_Light  : " << El << std::endl;
    std::cout << "> P_Heavy  : " << Ph << std::endl;
    std::cout << "> E_Heavy  : " << Eh << std::endl;
  }
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  
  // Now get the Excitation:
  ExcitationEnergy = Before.M()-M_Fragment();
  
  if(DEBUG){
    std::cout << "> Inv Mass : " << Before.M() << std::endl;
    std::cout << "> Frag Mass: " << M_Fragment() << std::endl;
    std::cout << "> Exc En   : " << ExcitationEnergy << std::endl;
  }


  return ExcitationEnergy;

}
Double32_t RN_ReactionInfo::IntHeavyKinEn(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t KineticEnergy = 0;

  if(DEBUG){
    std::cout << "---- 4Vec QVal Recon Check ------------------" << std::endl;
    std::cout << "> Light KE : " << LightKE << std::endl;
    std::cout << "> Light Th : " << LightTheta << std::endl;
    std::cout << "> Light Phi: " << LightPhi << std::endl;
    std::cout << "> Heavy KE : " << HeavyKE << std::endl;
    std::cout << "> Heavy Th : " << HeavyTheta << std::endl;
    std::cout << "> Heavy Phi: " << HeavyPhi << std::endl;

  }
  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;

  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  if(DEBUG){
    std::cout << "> P_Light  : " << Pl << std::endl;
    std::cout << "> E_Light  : " << El << std::endl;
    std::cout << "> P_Heavy  : " << Ph << std::endl;
    std::cout << "> E_Heavy  : " << Eh << std::endl;
  }
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  
  // Now get the Excitation:
  KineticEnergy = Before.E()-Before.M();//M_Fragment();
  
  if(DEBUG){
    std::cout << "> Inv Mass : " << Before.M() << std::endl;
    std::cout << "> Frag Mass: " << M_Fragment() << std::endl;
    std::cout << "> Kin En   : " << KineticEnergy << std::endl;
  }


  return KineticEnergy;

}
Double32_t RN_ReactionInfo::IntHeavyTheta(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t Theta = 0;

  if(DEBUG){
    std::cout << "---- 4Vec QVal Recon Check ------------------" << std::endl;
    std::cout << "> Light KE : " << LightKE << std::endl;
    std::cout << "> Light Th : " << LightTheta << std::endl;
    std::cout << "> Light Phi: " << LightPhi << std::endl;
    std::cout << "> Heavy KE : " << HeavyKE << std::endl;
    std::cout << "> Heavy Th : " << HeavyTheta << std::endl;
    std::cout << "> Heavy Phi: " << HeavyPhi << std::endl;

  }



  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;

  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  if(DEBUG){
    std::cout << "> P_Light  : " << Pl << std::endl;
    std::cout << "> E_Light  : " << El << std::endl;
    std::cout << "> G_Light  : " << 1.0/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product())) << std::endl;
    std::cout << "> P_Heavy  : " << Ph << std::endl;
    std::cout << "> E_Heavy  : " << Eh << std::endl;
    std::cout << "> G_Heavy  : " << 1.0/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay())) << std::endl;
  }
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  
  /*std::cout << "Light Theta Calc  : " << Light.Theta() << std::endl;
  std::cout << "Light Theta input : " << LightTheta << std::endl;
  std::cout << "Heavy Theta Calc  : " << Heavy.Theta() << std::endl;
  std::cout << "Heavy Theta input : " << HeavyTheta << std::endl;
  */
  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  
  // Now get the Excitation:
  Theta = Before.Theta();
  
  if(DEBUG){
    std::cout << "> Inv Mass : " << Before.M() << std::endl;
    std::cout << "> Frag Mass: " << M_Fragment() << std::endl;
    std::cout << "> Exc En   : " << Theta << std::endl;
  }


  return Theta;

}

Double32_t RN_ReactionInfo::NeutronEn(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t KinEnergy = 0;



  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;
  TLorentzVector Beam;
  TLorentzVector Target;
  TLorentzVector Neutron;
  
  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  Double32_t Pb,Eb;
  
  Eb = fBeamEnergy+M_Beam();
  Pb = TMath::Sqrt(Eb*Eb - M_Beam()*M_Beam());

  Beam.SetPxPyPzE(0,0,Pb,Eb);
  Target.SetPxPyPzE(0,0,0,M_Target());

  Neutron = Beam+Target-Before;
  // Now get the Excitation:
  KinEnergy = Neutron.E() - M_Recoil();
  
  if(DEBUG){
    std::cout << "> Beam En  : " << Beam.E()-M_Beam() << std::endl;
    std::cout << "> Neut Mass: " << M_Recoil() << std::endl;
    std::cout << "> Neut En  : " << KinEnergy << std::endl;
  }


  return KinEnergy;

}
Double32_t RN_ReactionInfo::NeutronAngle(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t Theta = 0;



  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;
  TLorentzVector Beam;
  TLorentzVector Target;
  TLorentzVector BT;
  TLorentzVector Neutron;
  
  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  Double32_t Pb,Eb;
  
  Eb = fBeamEnergy+M_Beam();
  Pb = TMath::Sqrt(Eb*Eb - M_Beam()*M_Beam());

  Beam.SetPxPyPzE(0,0,Pb,Eb);
  Target.SetPxPyPzE(0,0,0,M_Target());
  BT = Beam+Target;
  Neutron = Beam+Target-Before;
  // Now get the Excitation:
  Theta = Neutron.Theta();

  
  if(DEBUG){
    std::cout << "=================================" << std::endl;
    std::cout << "> Neut Theta : " << Theta << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> Prot Px    : " << Light.Px() << std::endl;
    std::cout << "> Prot Py    : " << Light.Py() << std::endl;
    std::cout << "> Prot Pz    : " << Light.Pz() << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> Heavy Px   : " << Heavy.Px() << std::endl;
    std::cout << "> Heavy Py   : " << Heavy.Py() << std::endl;
    std::cout << "> Heavy Pz   : " << Heavy.Pz() << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> Recoil Px  : " << Before.Px() << std::endl;
    std::cout << "> Recoil Py  : " << Before.Py() << std::endl;
    std::cout << "> Recoil Pz  : " << Before.Pz() << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> Neut Px    : " << Neutron.Px() << std::endl;
    std::cout << "> Neut Py    : " << Neutron.Py() << std::endl;
    std::cout << "> Neut Pz    : " << Neutron.Pz() << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> BT Px      : " << Beam.Px() << std::endl;
    std::cout << "> BT Py      : " << Beam.Py() << std::endl;
    std::cout << "> BT Pz      : " << Beam.Pz() << std::endl;
    std::cout << "- - - - - - - - - - - - - -" << std::endl;
    std::cout << "> Sum Px      : " << -Beam.Px()+Before.Px()+Neutron.Px() << std::endl;
    std::cout << "> Sum Py      : " << -Beam.Py()+Before.Py()+Neutron.Py() << std::endl;
    std::cout << "> Sum Pz      : " << -Beam.Pz()+Before.Pz()+Neutron.Pz() << std::endl;
  }


  return Theta;

}

Double32_t RN_ReactionInfo::NeutronAngleCM(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  
  bool DEBUG = false;
  Double32_t Theta = 0;



  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;
  TLorentzVector Beam;
  TLorentzVector Target;
  TLorentzVector BT;
  TLorentzVector Neutron;
  TVector3 BoostVector;
  
  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());
  /*
  Pl = TMath::Sqrt(2.0*LightKE*M_Decay_Product())/(TMath::Sqrt(1.0-2.0*LightKE/M_Decay_Product()));
  Ph = TMath::Sqrt(2.0*HeavyKE*M_Heavy_Decay())/(TMath::Sqrt(1.0-2.0*HeavyKE/M_Heavy_Decay()));
  El = TMath::Sqrt(M_Decay_Product()*M_Decay_Product()+Pl*Pl);
  Eh = TMath::Sqrt(M_Heavy_Decay()*M_Heavy_Decay()+Ph*Ph);
  */
  

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  // Add up the light and heavy to get the previous particles deets:
  Before = Light + Heavy;
  Double32_t Pb,Eb;
  
  Eb = fBeamEnergy+M_Beam();
  Pb = TMath::Sqrt(Eb*Eb - M_Beam()*M_Beam());

  Beam.SetPxPyPzE(0,0,Pb,Eb);
  Target.SetPxPyPzE(0,0,0,M_Target());
  BT = Beam+Target;
  BoostVector = BT.BoostVector(); 

  Neutron = Beam+Target-Before;
  Neutron.Boost(-BoostVector);
  // Now get the Excitation:
  Theta = Neutron.Theta();
  
  if(DEBUG){
    std::cout << "> Neut Theta : " << Theta << std::endl;
  }


  return Theta;

}

Double32_t RN_ReactionInfo::ProtonAngleCM(const double& LightKE, const double& HeavyKE, const double& LightTheta, const double& LightPhi, const double& HeavyTheta, const double& HeavyPhi){
  bool DEBUG = false;
  Double32_t ThetaCM = 0.0;
  
  // First we want to reconstruct the four vectors for the 
  // proton, and the heavy.
  TLorentzVector Light;
  TLorentzVector Heavy;
  TLorentzVector Before;
  TLorentzVector Beam;
  TLorentzVector Target;
  TLorentzVector BT;
  TLorentzVector Neutron;
  TVector3 BoostVector;
  
  // To set these lorentz vectors we need to calculate the relativistic energy
  // and momentum of these things:
  Double32_t Pl,El,Ph,Eh;
  El = LightKE + M_Decay_Product();
  Eh = HeavyKE + M_Heavy_Decay();
  Pl = TMath::Sqrt(El*El-M_Decay_Product()*M_Decay_Product());
  Ph = TMath::Sqrt(Eh*Eh-M_Heavy_Decay()*M_Heavy_Decay());

  Light.SetPxPyPzE(Pl*TMath::Cos(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Sin(LightPhi)*TMath::Sin(LightTheta),Pl*TMath::Cos(LightTheta),El);
  Heavy.SetPxPyPzE(Ph*TMath::Cos(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Sin(HeavyPhi)*TMath::Sin(HeavyTheta),Ph*TMath::Cos(HeavyTheta),Eh);

  Double32_t Pb,Eb;
  
  Eb = fBeamEnergy+M_Beam();
  Pb = TMath::Sqrt(Eb*Eb - M_Beam()*M_Beam());

  Beam.SetPxPyPzE(0,0,Pb,Eb);
  Target.SetPxPyPzE(0,0,0,M_Target());
  BT = Beam+Target;
  BoostVector = BT.BoostVector(); 

  Light.Boost(-BoostVector);

  ThetaCM = Light.Theta();

  return ThetaCM;

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

Double32_t RN_ReactionInfo::RecoilQValue(const double& nKE_R, const double & theta /*rad*/, double & hiKE  ){/*
  double M_N=M_Recoil();
  double M1=M_Beam();
  double M2=M_Fragment();
  hiKE=(M1/M2)*BeamEnergy_Est()+((M_N/M2)*nKE_R)-((2/M2)*sqrt((BeamEnergy_Est()*nKE_R*M_N*M1))*TMath::Cos(theta));
  
  return (nKE_R+hiKE-BeamEnergy_Est()); */
  // Define energy and momentum for beam
  double be,bp;
  
  // Define lorentz vectors for beam+target, recoil+target, target and beam
  TLorentzVector BT, RT,TargetLV,BeamLV,HeavyLV;

  // Define boost vector
  TVector3 boostv;
  
  // define q value
  double q;
  
  // define heavy ion kin en in cm, neturon ke in cm, and total energy in cm.
  double hiKE_CM,nKE_CM,ECM;

  //-------------------------------------------------
  // the beam energy and momentum are given by these:
  be = BeamEnergy_Est() + mArray[0];
  bp = sqrt(be*be - mArray[0]*mArray[0]);
  
  // Set the beam and target lorentz vectors in the lab frame:
  BeamLV.SetPxPyPzE(0,0,bp,be);
  TargetLV.SetPxPyPzE(0,0,0,mArray[1]);

  // the beam plus target lv, and teh boost vector:
  BT = BeamLV + TargetLV;
  boostv = BT.BoostVector();

  // Now get the energy and momentum of the neutron in the lab:
  be = nKE_R + mArray[2];
  bp = sqrt(be*be - mArray[2]*mArray[2]);
  
  // This is the recoil (neutron) LV.  We are assuming phi=0, which doesnt matter if we have cyl symmetry.
  RT.SetPxPyPzE(bp*sin(theta),0,bp*cos(theta),be);
  
  // Boost to the cm frame.
  RT.Boost(-boostv);
  BeamLV.Boost(-boostv);
  TargetLV.Boost(-boostv);
  
  // Get the neutron kinetic energy:
  nKE_CM = RT.E()-RT.M();
  
  // Calculate the heavy ion kinetic energy from the neutron ke.
  hiKE_CM = (mArray[2]/mArray[3])*nKE_CM;

  // Calculate the energy in the CM of the beam target system.
  ECM = BeamLV.E()-BeamLV.M() + TargetLV.E()-TargetLV.M();

  // Get the QValue
  q = -nKE_CM - hiKE_CM + ECM;
  //std::cout << "---------------------------" << std::endl;
  //std::cout << " Q First Way : " << q << std::endl;
  HeavyLV=BeamLV+TargetLV-RT;
  q = HeavyLV.M()-mArray[3];
  //std::cout << " Q Second Way : " << q << std::endl;
  
  return (q); 


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

TGraph RN_ReactionInfo::GetSecondaryHeavyDecayCurve(int Points,const double & hi_ex_set,const double& decay_ex_set){
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

    curve.SetPoint(p, evt2.getLVhi().Theta()*180/3.14,(double)(evt2.getLVhi().E() - evt2.getLVhi().M()));
    p++;
  }
  // end for(p)    

  return curve;
}

TGraph RN_ReactionInfo::GetSecondaryThetaTheta(int Points,const double & hi_ex_set,const double& decay_ex_set){
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

    curve.SetPoint(p, evt2.getLVhi().Theta()*180/3.14,evt2.getLVrad().Theta()*180/3.14);
    p++;
  }
  // end for(p)    

  return curve;
}






#endif
