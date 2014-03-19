/***************************************************************
//
//  DetectorConstruction class for the simple case of a single
//  plastic detector placed at the origin
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/

#ifndef RNDetectorConstruction_H
#define RNDetectorConstruction_H

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Tubs;
class G4VPhysicalVolume;
class G4UniformMagField;
class G4GenericMessenger;

/// Detector construction class to define materials and geometry.
///
/// Four parameters define the geometry of the NeutronDetectorArray :
/// - the thickness of the crystals
/// - the radius of the crystals,
/// - the number of crystals,
/// - transverse positioning of crystals,
/// 



class RNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    RNDetectorConstruction();
    virtual ~RNDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

    // set methods
    //

     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    G4GenericMessenger*  fMessenger; // messenger 

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};



#endif

