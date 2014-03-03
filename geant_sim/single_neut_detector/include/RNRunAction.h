/***************************************************************
//
//  RunAction class for the simple case of a single
//  plastic detector placed at the origin
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/
#ifndef RNRunAction_h
#define RNRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// It accumulates statistic and computes the energy deposited in the 
/// crystal, also, track lengths 
/// The same values are also saved in the ntuple.
/// The histograms and ntuple are saved in the output file in a format
/// accoring to a selected technology in RNAnalysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.
///

class RNRunAction : public G4UserRunAction
{
  public:
    RNRunAction();
    virtual ~RNRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};



#endif

