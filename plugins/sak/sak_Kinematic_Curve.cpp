/*******************************************************************
Code: sak_KinematicCurve.cpp

Description: This class gives the kinematic curves as a TGraph
  object. The masses for the reaction and the energy of excitation
  must be set before getting the curve.

Adapted from Kinematic Curve class by Daniel Santiago-Gonzalez
2013-March
*******************************************************************/

#include <iostream>
#include <cmath>
#include <TMath.h>
#include <TGraph.h>
#include "sak_Kinematic_Curve.hpp"
#include "RN_Root.hpp"
#include "RN_Sim.hpp"
#include "RN_SimPhysics.hpp"

using namespace RNROOT;

namespace sak{

TGraph GetCurve(int Points,const double & hi_ex_set)
{
  TGraph curve;
  if(!gPrimaryReaction.IsSet()){
    std::cout<<"Reaction Masses have not been set"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(!gPrimaryReaction.BeamEnergy()){
    std::cout<<"Beam Energy has not been set"<<std::endl;
    exit(EXIT_FAILURE);
  }

  sim::RN_SimEvent evt1(gPrimaryReaction.BeamEnergy(),gPrimaryReaction.M_Beam(),gPrimaryReaction.M_Target(),gPrimaryReaction.M_Recoil(),gPrimaryReaction.M_Fragment()); 
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


TGraph GetSecondaryDecayCurve(int Points,const double & hi_ex_set,const double& decay_ex_set)
{
  TGraph curve;
  if(!gPrimaryReaction.IsSet()){
    std::cout<<"Reaction Masses have not been set"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(!gPrimaryReaction.BeamEnergy()){
    std::cout<<"Beam Energy has not been set"<<std::endl;
    exit(EXIT_FAILURE);
  }
 
  sim::RN_SimEvent evt1(gPrimaryReaction.BeamEnergy(),gPrimaryReaction.M_Beam(),gPrimaryReaction.M_Target(),gPrimaryReaction.M_Recoil(),gPrimaryReaction.M_Fragment());
  sim::RN_SimEvent evt2(gPrimaryReaction.M_Fragment(),gPrimaryReaction.M_Decay_Product(),gPrimaryReaction.M_Heavy_Decay());
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
    if(!evt2.radiate_in_CM(evt1.getLVhi(),pv,decay_ex_set))
      continue;

    curve.SetPoint(p, evt2.getLVrad().Theta()*180/3.14,(double)(evt2.getLVrad().E() - evt2.getLVrad().M()));
    p++;
  }
  // end for(p)    

  return curve;
}
}
