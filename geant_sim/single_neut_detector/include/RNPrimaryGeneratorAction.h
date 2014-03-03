/**************************************************************
//
//Primary Generator class for the simple example of a single 
//plastic detector
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/

#ifndef RNPrimaryGeneratorAction_h
#define RNPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the pterphenyl crystal 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class RNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  RNPrimaryGeneratorAction();    
  virtual ~RNPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4ParticleGun*  fParticleGun; // G4 particle gun
};



#endif


