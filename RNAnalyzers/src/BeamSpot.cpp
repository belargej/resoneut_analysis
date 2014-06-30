/****************************************************
//Class: BeamSpot reconstruction
//
//Author:Sean Kuvin
//
After you have read through this template.
make a copy of it with a new name and create your
own Analyzer class.  To add it the libRNanalyzers package, add the file to the Makefile & myLinkdef.h file following the example.  You can also compile the class using AcLiC as long as your root environment is set up to load the RN_Root Libs(see rootlogon.C) and your include path is set properly.

**************************************************/
#include "RN_Root.hpp"
#include "TVector3.h"
#include "BeamSpot.hpp"
using namespace RNROOT;

namespace beamspot{

  TH2D* hBeamSpot;
  TH1D* hZRel;
  TH1D* hRelAngle;
  TH2D* hEvRelAngle;
  TVector3 gRelVector(0,0,0);
  TVector3 gBeamSpot(0,0,0);
  

  ReconstructBeamSpot::ReconstructBeamSpot()
{
  
}


void ReconstructBeamSpot::Reset(){
  gRelVector.SetXYZ(0,0,0);
  gBeamSpot.SetXYZ(0,0,0);
}

bool ReconstructBeamSpot::Begin(){

  if(!fgRootFile){
    std::cout<<"output file has not been created"<<std::endl;
    Clear();
    exit(EXIT_FAILURE);
  }
  
  fgRootFile->mkdir("BeamSpot");
  fgRootFile->cd("BeamSpot");

  hBeamSpot = new TH2D ("hBeamSpot","hBeamSpot;X;Y",256,-128,127,256,-128,127);
  hZRel = new TH1D ("hZREL","hZREL;X;Y",4095,-256,255);
  hRelAngle = new TH1D ("hrelangle","hrelangle",128,0,40);
  hEvRelAngle = new TH2D ("hEvrelangle","hEvrelangle",256,0,64,256,0,32);
  return 1;
 
}

bool ReconstructBeamSpot::Process(){

  gRelVector = si_cluster_[1].fPos[0] - si_cluster_[0].fPos[0];
  gBeamSpot = si_cluster_[0].fPos[0] - ((si_cluster_[0].fPos[0].Z() / gRelVector.Z()) * gRelVector);
  

  return 1;
}

bool ReconstructBeamSpot::ProcessFill(){
  if(si_cluster_[1].Mult()>0 && si_cluster_[0].Mult()>0){
    hBeamSpot->Fill(gBeamSpot.X(),gBeamSpot.Y());
    hZRel->Fill(gBeamSpot.Z());
    hRelAngle->Fill(gRelVector.Theta()*TMath::RadToDeg());
    hEvRelAngle->Fill(gRelVector.Theta()*TMath::RadToDeg(),si_array.E_AB());
}
  return 1;
}
  


bool ReconstructBeamSpot::Terminate(){

  return 1;
  
}

bool ReconstructBeamSpot::TerminateIfLast(){

  return 1;
  
}


void LoadBeamSpotGates(){

}

}
