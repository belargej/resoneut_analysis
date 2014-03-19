/***************************************************************
//
//  DetectorConstruction class for the simple case of a single
//  plastic detector placed at the origin
//
//  Author: Sean Kuvin 3/2/2014
***************************************************************/

#ifndef RNDetectorConstruction_CXX
#define RNDetectorConstruction_CXX

#include "RNDetectorConstruction.h"

#include "G4Material.hh"
#include "G4NistManager.hh"


#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4GenericMessenger.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <stdio.h>


RNDetectorConstruction::RNDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fMessenger(0),
   fCheckOverlaps(true)
{
  // Define /RN/det commands using generic messenger class
  fMessenger 
    = new G4GenericMessenger(this, "/RN/det/", "Detector construction control");                            
}



RNDetectorConstruction::~RNDetectorConstruction()
{ 
  delete fMessenger;
}



G4VPhysicalVolume* RNDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();

}

void RNDetectorConstruction::DefineMaterials()
{ 
  G4NistManager* nistManager = G4NistManager::Instance();
  G4bool fromIsotopes = false;
  
  G4double z, a, density;
  G4String name, symbol;
  G4int ncomponents, natoms;

  //p-Terphenyl molecule constructed from constituents
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

  a = 12.00*g/mole;
  G4Element* elC  = new G4Element(name="Carbon"  ,symbol="O" , z= 6., a);

  density = 1.23*g/cm3;
  G4Material* C18H14 = new G4Material(name="p_terphenyl",density,ncomponents=2);
  C18H14->AddElement(elH, natoms=14);
  C18H14->AddElement(elC, natoms=18);  

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  //////////////////////////////////////////////////////////

  // Print materials
  G4cout << C18H14 << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}



G4VPhysicalVolume* RNDetectorConstruction::DefineVolumes()
{
  //cylinder properties
  G4double zpos = -228.7*cm;
  G4double xpos=0,ypos =0;
  G4double innerRadius = 0.*cm;
  G4double outerRadius = 10*cm; 
  G4double thickness= 2.54*cm;
  G4ThreeVector pos(xpos, ypos, zpos);
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;

  // Get materials
  G4Material* pterphenyl = G4Material::GetMaterial("p_terphenyl");
  G4Material* vac    = G4Material::GetMaterial("Galactic");

  if ( ! pterphenyl || ! vac ) {
    G4cerr << "Cannot retrieve materials already defined. " << G4endl;
    G4cerr << "Exiting application " << G4endl;
    exit(1);
  }  
   
  //     
  // World
  //
  double wx = fabs(zpos)*2,wy = fabs(zpos)*2,wz = fabs(zpos)*2;

  G4VSolid* worldS 
    = new G4Box("World",           // its name
		wx, wy, wz); // its size
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 vac,  // its material
                 "World");         // its name
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  //                               
  // plastic
  //  
  
  G4VSolid* Single_Plastic = new G4Tubs("single_plastic",
					innerRadius, 
					outerRadius,
					thickness,
					startAngle, 
					spanningAngle);
  G4LogicalVolume* l_single_plastic
    = new G4LogicalVolume(
                 Single_Plastic,    // its solid
                 pterphenyl, // its material
                 "single_plastic");  // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 pos,  //
                 l_single_plastic,          // its logical volume                         
                 "single_plastic",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  
  //
  // print parameters
  //
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The single_plastic is:" << thickness << "mm thick[ "
         << thickness/mm << "mm of " << pterphenyl->GetName()<<"\n";
  
  
  // 
  // Scorers
  //

  // declare Absorber as a MultiFunctionalDetector scorer
  //  
    G4MultiFunctionalDetector* neutDetector
    = new G4MultiFunctionalDetector("neutDetector");

  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  neutDetector->RegisterPrimitive(primitive);

  primitive = new G4PSTrackLength("TrackLength");
  neutDetector->RegisterPrimitive(primitive);  

  G4SDManager::GetSDMpointer()->AddNewDetector(neutDetector);
  l_single_plastic->SetSensitiveDetector(neutDetector);
  
 
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  l_single_plastic->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

#endif
