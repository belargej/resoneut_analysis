/***************************************************************
//
//  RunAction class for the simple case of a single
//  plastic detector placed at the origin
//
//Author: Sean Kuvin 3/2/2014
***************************************************************/

#include "RNRunAction.h"
#include "RNAnalysis.h"


#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RNRunAction::RNRunAction()
 : G4UserRunAction()
{ 
}



RNRunAction::~RNRunAction()
{
}



void RNRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Book histograms, ntuple
  //
  
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in RNAnalysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() 
         << " analysis manager" << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  
  // Open an output file
  //
  G4String fileName = "RN";
  analysisManager->OpenFile(fileName);
  analysisManager->SetFirstHistoId(1);

  // Creating histograms
  //
  analysisManager->CreateH1("1","Edep in detector", 100, 0., 10*MeV);
  analysisManager->CreateH1("2","trackL in detector", 100, 0., 1*m);

   // Creating ntuple
  //
  analysisManager->CreateNtuple("RN", "Edet and Ldet");
  analysisManager->CreateNtupleDColumn("Edet");
  analysisManager->CreateNtupleDColumn("Ldet");

  analysisManager->FinishNtuple();
}


void RNRunAction::EndOfRunAction(const G4Run* aRun)
{

  std::cout<<"EndOfRunAction\n";
  G4int nofEvents = aRun->GetNumberOfEvent();
  if ( nofEvents == 0 ) return;
  
  // print histogram statistics
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
    
    G4cout 
       << " Edet : mean = " << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
               << " rms = " << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") 
               << G4endl;
    G4cout                
       << " Ldet : mean = " << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length") 
               << " rms = " << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Length") 
               << G4endl;

  }
  
  // save histograms 
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // complete cleanup
  //
  delete G4AnalysisManager::Instance();  
}


