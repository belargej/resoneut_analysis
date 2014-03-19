/**************************************************************
//
//Event action class for the simple example of a single 
//plastic detector
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/


#include "RNEventAction.h"
#include "RNAnalysis.h"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4GenericMessenger.hh"
#include "G4UnitsTable.hh"
#include <cstdlib>
#include <iostream>
#include "Randomize.hh"
#include <iomanip>



RNEventAction::RNEventAction()
 : G4UserEventAction(),
   fMessenger(0),
   fPrintModulo(1)
{
  // Define /RN/event commands using generic messenger class
  fMessenger = new G4GenericMessenger(this, "/RN/event/", "Event control");

  std::cout<<"here"<<std::endl;

  // Define /RN/event/setPrintModulo command
  G4GenericMessenger::Command& setPrintModulo 
    = fMessenger->DeclareProperty("setPrintModulo", 
                                  fPrintModulo, 
                                 "Print events modulo n");
  setPrintModulo.SetRange("value>0");                                
}



RNEventAction::~RNEventAction()
{
  delete fMessenger;
}



G4THitsMap<G4double>* 
RNEventAction::GetHitsCollection(const G4String& hcName,
                                  const G4Event* event) const
{
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  G4THitsMap<G4double>* hitsCollection 
    = static_cast<G4THitsMap<G4double>*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4cerr << "Cannot access hitsCollection " << hcName << G4endl;
    exit(1);
  }         

  return hitsCollection;
}    

G4double RNEventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0;
  std::map<G4int, G4double*>::iterator it;
  for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
    sumValue += *(it->second);
  }
  return sumValue;  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RNEventAction::PrintEventStatistics(
					 G4double Edet, G4double Ldet)const
{
  // Print event statistics
  //
  G4cout
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(Edet, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(Ldet, "Length")
     << G4endl;





}



void RNEventAction::BeginOfEventAction(const G4Event* event)
{  

  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0 )  { 
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }
}



void RNEventAction::EndOfEventAction(const G4Event* event)
{  
  // Get sum value from hits collections
  //

   std::cout<<"EndOfEventAction\n";

  G4double Edet
    = GetSum(GetHitsCollection("neutDetector/Edep", event));

  G4double Ldet 
    = GetSum(GetHitsCollection("neutDetector/TrackLength", event));


  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  //  
  analysisManager->FillH1(1, Edet);
  analysisManager->FillH1(2, Ldet);
 
  
  // fill ntuple
  //
  analysisManager->FillNtupleDColumn(0, Edet);
  analysisManager->FillNtupleDColumn(1, Ldet);
 
 
  analysisManager->AddNtupleRow();  
  
  //print per event (modulo n)
  //
  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(Edet, Ldet);
  }

  
}  


