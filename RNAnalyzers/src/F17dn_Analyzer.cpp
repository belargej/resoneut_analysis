/****************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"
#include "F17dn_Analyzer.hpp"

using namespace RNROOT;

namespace _F17{
  /*
  TH2D *hSiPIDFBand;
  TH2D *hSiPIDOBand;
  TH2D *hSiPIDNBand;
  TH2D *hICEdEProt;
  TH2D *hICEdEAlpha;
  TH2D *hEvThetaFBand;
  TH2D *hEvThetaOBand;
  TH2D *hEvThetaNBand;
  TH2D *hEvThetaFBandProt;
  TH2D *hEvThetaOBandProt;
  TH2D *hEvThetaNBandAlpha;
  TH2D *hSibTrelvICTrelRebin;
  */

F17dn_Analyzer::F17dn_Analyzer()
{
  
}


void F17dn_Analyzer::Reset(){

}

bool F17dn_Analyzer::Begin(){
  /*
  //make directory structure
  fgRootFile->mkdir("F17dx_Analysis");
  fgRootFile->cd("F17dx_Analysis");
  gDirectory->mkdir("Silicon");
  gDirectory->mkdir("IonChamber");
  gDirectory->mkdir("TRel");


  fgRootFile->cd("F17dx_Analysis/Silicon");




  fgRootFile->cd("F17dx_Analysis/IonChamber");
  hICEdEProt =new TH2D("hICEdEProt","hICEdEProt; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
  hICEdEAlpha =new TH2D("hICEdEAlpha","hICEdEAlpha; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);



  
  fgRootFile->cd("F17dx_Analysis/TRel");
  hsibtrel_v_ictrel = new  TH2D("hsibtrel_v_ictrel","hsibtrel_v_ictrel;ictrel_rf;sibtrel_rf",1024,-2048,2047,1024,-2048,2047);



  */


  return 1;
 
}

bool F17dn_Analyzer::Process(){
  //  if(silicon::protcheck 
  //hICEdEAlpha->Fill(ic.E()+ic.DE(),ic.DE());
  //hICEdEProt->Fill(ic.E()+ic.DE(),ic.DE());

  return 1;
}

bool F17dn_Analyzer::ProcessFill(){
 
  return 1;
}
  


bool F17dn_Analyzer::Terminate(){

  return 1;
  
}

bool F17dn_Analyzer::TerminateIfLast(){
  fgRootFile->Write();
  fgRootFile->Close();

  return 1;
  
}


void Load_F17_dn_Gates(){


}
}
