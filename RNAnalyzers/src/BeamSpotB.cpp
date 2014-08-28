/****************************************************
//Class: ReconstructBeamSpotB
//
//Author:Sean Kuvin
//Original Date: 8/28/2014
//
//Intermediate method B for reconstructing the beamspot using 
//the IonChamber hit position.  This method follows the same 
//dispersion scale factor but also attempts to use kinematics
//reconstruction(determining expected heavy ion recoil position
//from light particle kinematics in the silicon)
// for further information on beamspot.
**************************************************/

#ifndef _RECOBEAMSPOTB_CXX
#define _RECOBEAMSPOTB_CXX

#include "RN_Root.hpp"
#include "TVector3.h"
#include "BeamSpotB.hpp"

using namespace RNROOT;

namespace beamspotB{

  TH2D* hBeamSpot;
  TH2D* hEvSiAAngle;
  TH2D* hEvSiBAngle;
  TH2D* hICPhivBeamPhi;
  TH2D* hHDPhivSiPhi;
  TH2D* hHDPhivICPhi;

  TH1D * hErCorrected;
  TH2D * hErCorrectedvProtonE;
  TH2D * hErCorrectedvSiPhi;
  TH2D * hErCorrectedvICPhi;
  TH2D * hErCorrectedvICTheta;
  TH2D * hICThetavICX;
  TH2D * hICThetavICR;


  ReconstructBeamSpotB::ReconstructBeamSpotB(): fRelVector(0,0,0),
						fBeamSpot(0,0,0),
						fSiAVector(0,0,0),
						fSiBVector(0,0,0),
						fSiAAngle(0),
						fSiBAngle(0),
						fErProton(0),
						fHDPhi(0),
						fDispersion(0.3),
						fDecayLV(0,0,0,0),
						fFragLV(0,0,0,0),
						fHeavyDecayLV(0,0,0,0)
  {
    
  }
  
  
  void ReconstructBeamSpotB::Reset(){
    fRelVector.SetXYZ(0,0,0);
    fBeamSpot.SetXYZ(0,0,0);
    fSiAVector.SetXYZ(0,0,0);
    fSiBVector.SetXYZ(0,0,0);
    fSiBAngle = 0;
    fSiAAngle = 0;
    fErProton = 0;
    fHDPhi = 0;
    fDecayLV.SetPxPyPzE(0,0,0,0);
    fFragLV.SetPxPyPzE(0,0,0,0);
    fHeavyDecayLV.SetPxPyPzE(0,0,0,0);
    

  }
 
  void ReconstructBeamSpotB::SetDispersionScaleFactor(double factor){
    fDispersion = factor;
  }
  
  
  bool ReconstructBeamSpotB::Begin(){
    
  if(!fgRootFile){
    std::cout<<"output file has not been created"<<std::endl;
    Clear();
    exit(EXIT_FAILURE);
  }
  
  fgRootFile->mkdir("BeamSpotB");
  fgRootFile->cd("BeamSpotB");

  hBeamSpot = new TH2D ("hBeamSpotB","hBeamSpot;X;Y",256,-128,127,256,-128,127);
  hEvSiAAngle = new TH2D ("hEvSiAangleB","hEvSiAangle",256,0,42,128,0,32);
  hEvSiBAngle = new TH2D ("hEvSiBangleB","hEvSiBangle",256,0,42,128,0,32);
  hICPhivBeamPhi = new TH2D ("hICPhivBeamPhiB","hICPhivBeamPhi",721,-180,180,721,-180,180);
  hHDPhivSiPhi = new TH2D ("hHDPhivSiPhiB","hHDPhivSiPhi",721,-180,180,721,-180,180);
  hHDPhivICPhi = new TH2D ("hHDPhivICPhiB","hHDPhivICPhi",721,-180,180,721,-180,180);
  hErCorrected = new TH1D("hErProtonCorrectedB","hErProtonCorrected",512,-1,10);
  hErCorrectedvProtonE = new TH2D ("hErCorrectedvProtEB","hErCorrectedvProtE",512,-1,10,512,0,32);
  hErCorrectedvSiPhi = new TH2D ("hErCorrectedvSiPhiB","hErCorrectedvSiPhi",512,-1,10,180,-180,180);
  hErCorrectedvICPhi = new TH2D ("hErCorrectedvICPhiB","hErCorrectedvICPhi",512,-1,10,180,-180,180);
  hErCorrectedvICTheta = new TH2D ("hErCorrectedvICThetaB","hErCorrectedvICTheta",512,-1,10,256,-1,14);
  hICThetavICX = new TH2D ("hICThetavICXB","hICThetavICX",512,-1,14,256,-62,64);
  hICThetavICR = new TH2D ("hICThetavICRB","hICThetavICR",512,-1,14,256,0,63);


  return 1;
 
}

bool ReconstructBeamSpotB::Process(){
 
  double pFragmentMag = TMath::Sqrt(2 * gReactionInfo.M_Fragment() * gReactionInfo.E_Fragment()); 
  double pDecayMag = TMath::Sqrt(2 * gReactionInfo.M_Decay_Product() * si_array.E_AB()); 
  
  fFragLV = TLorentzVector(0,0,pFragmentMag,gReactionInfo.E_Fragment() + gReactionInfo.M_Fragment());
  
  
  for(unsigned int i=0;i<10;i++){
    fDecayLV.SetVectMag(si_cluster_[1].fPos[0] - fBeamSpot,pDecayMag);
    fDecayLV.SetE(si_array.E_AB() + gReactionInfo.M_Decay_Product());
    fHeavyDecayLV = fFragLV - fDecayLV;
    double hdphi = fHeavyDecayLV.Phi();
    double hdtheta = fHeavyDecayLV.Theta();
    double hdr = TMath::Tan(hdtheta) * ic.GetHitPos().Z();
    double hdx = TMath::Cos(hdphi)*hdr;
    double hdy = TMath::Sin(hdphi)*hdr;    
    
    fHDPhi = TMath::ATan2(hdy,hdx) * TMath::RadToDeg();
    fBeamSpot = TVector3((ic.GetHitPos().X()- hdx)*fDispersion,(ic.GetHitPos().Y()- hdy)*fDispersion,0);
    
    
    }
  
  
  
  fSiBVector = si_cluster_[1].fPos[0] - fBeamSpot;
  fSiAVector = si_cluster_[0].fPos[0] - fBeamSpot;
  
  fSiBAngle = TMath::RadToDeg()*fSiBVector.Theta(); 
  fSiAAngle = TMath::RadToDeg()*fSiAVector.Theta();
  
    
  fErProton = gReactionInfo.DecayQValueEstimate(si_array.E_AB(),fSiBAngle*TMath::DegToRad());
  



  
  return 1;
}

bool ReconstructBeamSpotB::ProcessFill(){
  if(si_cluster_[1].Mult()>0 && si_cluster_[0].Mult()>0){
    hBeamSpot->Fill(fBeamSpot.X(),fBeamSpot.Y());
    hICPhivBeamPhi->Fill(fBeamSpot.Phi()*TMath::RadToDeg(),ic.Phi());
    hEvSiAAngle->Fill(fSiAAngle,si_array.E_AB());
    hEvSiBAngle->Fill(fSiBAngle,si_array.E_AB());
    

    hHDPhivSiPhi->Fill(si_array.Phi_B()*TMath::RadToDeg(),fHDPhi);
    hHDPhivICPhi->Fill(ic.Phi(),fHDPhi);

    hErCorrected->Fill(fErProton);
    hErCorrectedvProtonE->Fill(fErProton,si_array.E_AB());
    hErCorrectedvSiPhi->Fill(fErProton,si_array.Phi_B()*TMath::RadToDeg());
    hErCorrectedvICPhi->Fill(fErProton,ic.Phi());
    
    hErCorrectedvICTheta->Fill(fErProton,ic.Theta());
    hICThetavICX->Fill(ic.Theta(),ic.GetHitPos().X());
    hICThetavICR->Fill(ic.Theta(),ic.GetHitPos().Perp());


 }
  


  return 1;
}
  


  bool ReconstructBeamSpotB::Terminate(){
    
    return 1;
    
  }
  

  void LoadBeamSpotBGates(){
    
  }
  
}

#endif
