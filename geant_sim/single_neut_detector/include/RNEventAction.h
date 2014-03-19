/**************************************************************
//
//Event action class for the simple example of a single 
//plastic detector
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/


/// \file RNdEventAction.hh
/// \brief Definition of the RNdEventAction class

#ifndef RNEventAction_h
#define RNEventAction_h 

#include "G4UserEventAction.hh"

#include "G4THitsMap.hh"
#include "globals.hh"

class G4GenericMessenger;

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.
///
/// The data member fPrintModulo defines the frequency of printing
/// the accumulated quantities. Its value can be changed via a command
/// defined using G4GenericMessenger class:
/// - /RN/event/setPrintModulo value

class RNEventAction : public G4UserEventAction
{
public:
  RNEventAction();
  virtual ~RNEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
                     
  // set methods
  void SetPrintModulo(G4int value);
    
private:
  // methods
  G4THitsMap<G4double>* GetHitsCollection(const G4String& hcName,
                                          const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
  void PrintEventStatistics(G4double , G4double) const;
  
  // data members                   
  G4GenericMessenger*  fMessenger;
  G4int  fPrintModulo;
};

// inline functions

inline void RNEventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
}
                     


#endif

    
