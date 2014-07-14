/****************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "F17dn_Analyzer.hpp"

using namespace RNROOT;
using namespace TMath;

namespace _F17{
  //declare cuts
  TCutG *gFBand;
  TCutG *gOBand;
  TCutG *gNBand; 
  TCutG *gFBandB;
  TCutG *gOBandB;
  TCutG *gNBandB;
  TCutG *gProton;
  TCutG *gAlpha;
  TCutG *gDeuteron;

 
  Bool_t FBandCheck(0);
  Bool_t OBandCheck(0);
  Bool_t NBandCheck(0); 
  Bool_t FBandBCheck(0);
  Bool_t OBandBCheck(0);
  Bool_t NBandBCheck(0);
  Bool_t ProtonCheck(0);
  Bool_t AlphaCheck(0);
  Bool_t DeuteronCheck(0);


  
  TH2D *hSiPIDFBand;
  TH2D *hSiPIDOBand;
  TH2D *hSiPIDNBand;
  TH2D *hSiPIDFBandB;
  TH2D *hSiPIDOBandB;
  TH2D *hSiPIDNBandB;
  TH2D *hICEdEProt;
  TH2D *hICEdEAlpha;
  TH2D *hICEdEDeuteron;
  TH2D *hEvThetaFBand;
  TH2D *hEvThetaOBand;
  TH2D *hEvThetaNBand;
  TH2D *hEvThetaFBandB;
  TH2D *hEvThetaOBandB;
  TH2D *hEvThetaNBandB;
  TH2D *hEvThetaFBandProt;
  TH2D *hEvThetaOBandProt;
  TH2D *hEvThetaNBandAlpha;
  TH2D *hSibTrelvICTrelRebin;
  TH2D *hSibTrelvICTrelRebinProton;
  TH2D *hSibTrelvICTrelRebinProtonFBand;
  TH2D *hSibTrelvICTrelRebinProtonOBand;
  TH2D *hErvNeutT;
  TH2D *hErvNeutKE_R;
  TH2D *hErvNeutKE;
  TH2D *hErvNeutQ;



  F17dn_Analyzer::F17dn_Analyzer():fErProton(0)
  {
    
  }


  void F17dn_Analyzer::Reset(){
    
    fErProton = 0;
    fNeutTime = 0;
    fNeutKE_R = 0;
    fNeutKE = 0;
    fHiKE = 0;
    fNeutQ = 0;

    OBandCheck = 0;
    NBandCheck = 0; 
    FBandBCheck = 0;
    OBandBCheck = 0;
    NBandBCheck = 0;
    ProtonCheck = 0;
    AlphaCheck = 0;
    DeuteronCheck = 0;
    
    
    
  }
  
  bool F17dn_Analyzer::Begin(){

    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    
    //check for parameters necessary for q_value reconstruction
    if(!gReactionInfo.IsSet()){
      std::cout<<"Reaction Masses have not been set"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    if(!gReactionInfo.E_Fragment())
      std::cout<<"Heavy Ion energy ansatz for Q value reconstruction needed"<<std::endl;
    

  
  //make directory structure
  fgRootFile->mkdir("F17dx_Analysis");
  fgRootFile->cd("F17dx_Analysis");
  gDirectory->mkdir("Silicon");
  gDirectory->mkdir("IonChamber");
  gDirectory->mkdir("TRel");
  gDirectory->mkdir("Neut");


  fgRootFile->cd("F17dx_Analysis/Silicon");

  hSiPIDFBand = new TH2D("hSiPIDFBand","hSiPIDFBand",1024,0,64,512,0,32);
  hSiPIDOBand = new TH2D("hSiPIDOBand","hSiPIDOBand",1024,0,64,512,0,32);
  hSiPIDNBand = new TH2D("hSiPIDNBand","hSiPIDNBand",1024,0,64,512,0,32);
 
  hSiPIDFBandB = new TH2D("hSiPIDFBandB","hSiPIDFBandB",1024,0,64,512,0,32);
  hSiPIDOBandB = new TH2D("hSiPIDOBandB","hSiPIDOBandB",1024,0,64,512,0,32);
  hSiPIDNBandB = new TH2D("hSiPIDNBandB","hSiPIDNBandB",1024,0,64,512,0,32);

  hEvThetaFBand = new TH2D("hEvThetaFBand","hEvThetaFBand",256,0,32,1024,0,64);
  hEvThetaOBand = new TH2D("hEvThetaOBand","hEvThetaOBand",256,0,32,1024,0,64);
  hEvThetaNBand = new TH2D("hEvThetaNBand","hEvThetaNBand",256,0,32,1024,0,64);

  hEvThetaFBandB = new TH2D("hEvThetaFBandB","hEvThetaFBandB",256,0,32,1024,0,64);
  hEvThetaOBandB = new TH2D("hEvThetaOBandB","hEvThetaOBandB",256,0,32,1024,0,64);
  hEvThetaNBandB = new TH2D("hEvThetaNBandB","hEvThetaNBandB",256,0,32,1024,0,64);

  hEvThetaFBandProt = new TH2D("hEvThetaFBandProton","hEvThetaFBandProton",256,0,32,1024,0,64);
  hEvThetaOBandProt = new TH2D("hEvThetaOBandProton","hEvThetaOBandProton",256,0,32,1024,0,64);
  hEvThetaNBandAlpha = new TH2D("hEvThetaNBandAlpha","hEvThetaNBandAlpha",256,0,32,1024,0,64);



  fgRootFile->cd("F17dx_Analysis/IonChamber");
  hICEdEProt =new TH2D("hICEdEProt","hICEdEProt; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
  hICEdEAlpha =new TH2D("hICEdEAlpha","hICEdEAlpha; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
  hICEdEDeuteron =new TH2D("hICEdEDeuteron","hICEdEDeuteron; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);

  fgRootFile->cd("F17dx_Analysis/TRel");
  hSibTrelvICTrelRebin = new TH2D("hSibTrelvICTrelRebin","hSibTrelvICTrelRebin",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProton = new TH2D("hSibTrelvICTrelRebinProton","hSibTrelvICTrelRebinProton",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProtonFBand = new TH2D("hSibTrelvICTrelRebinProtonFBand","hSibTrelvICTrelRebinProtonFBand",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProtonOBand = new TH2D("hSibTrelvICTrelRebinProtonOBand","hSibTrelvICTrelRebinProtonOBand",1024,-100,1434,1024,-100,1434);
  
  fgRootFile->cd("F17dx_Analysis/Neut");
  hErvNeutT = new TH2D ("hErvNeutT","hErvNeutT",1024,-100,410,512,-1,10);
  hErvNeutKE_R = new TH2D ("hErvNeutKE_R","hErvNeutKE_R",1024,0,1,512,-1,10);
  hErvNeutKE = new TH2D ("hErvNeutKE","hErvNeutKE",1024,0,1,512,-1,10);
  hErvNeutQ = new TH2D ("hErvNeutQ","hErvNeutQ",1024,-10,10,512,-1,10);



  return 1;
 
}

bool F17dn_Analyzer::Process(){

  for(unsigned int i=0; i<neut.size();i++){
    if(psd::rawneutcheck[i]){
      fNeutTime = (neut[i].T() - rftime.TMod2() - 41);
      fNeutKE_R = neut[i].nKE_R(fNeutTime);
      fNeutQ = gReactionInfo.RecoilQValue(neut[i].GetPosVect().Z(),fNeutTime,fNeutKE,fHiKE);
      break;
    }
  }
  
  if(si_array.E_AB(),si_array.Theta_B()){
    fErProton = gReactionInfo.DecayQValueEstimate(si_array.E_AB(),si_array.Theta_B());
  }

  FBandCheck = (gFBand && gFBand->IsInside(ic.TotalE(),ic.DE()));
  OBandCheck = (gOBand && gOBand->IsInside(ic.TotalE(),ic.DE()));
  NBandCheck = (gNBand && gNBand->IsInside(ic.TotalE(),ic.DE()));
  FBandBCheck = (gFBandB && gFBandB->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));
  OBandBCheck = (gOBandB && gOBandB->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));
  NBandBCheck = (gNBandB && gNBandB->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));
  ProtonCheck = (gProton && gProton->IsInside(si_array.E_AB(),si_array.E_A()));
  AlphaCheck = (gAlpha && gAlpha->IsInside(si_array.E_AB(),si_array.E_A()));
  DeuteronCheck = (gDeuteron && gDeuteron->IsInside(si_array.E_AB(),si_array.E_A()));


  return 1;
}

bool F17dn_Analyzer::ProcessFill(){
  if(FBandCheck){
    hSiPIDFBand->Fill(si_array.E_AB(),si_array.E_A());
    hEvThetaFBand->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    
    if(ProtonCheck){
      hEvThetaFBandProt->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
      hSibTrelvICTrelRebinProtonFBand->Fill(si_array.T_B()-rftime.TMod2(),ic.T()-rftime.TMod2());
      }
    
  }
  if(OBandCheck){
    hSiPIDOBand->Fill(si_array.E_AB(),si_array.E_A());
    hEvThetaOBand->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    if(ProtonCheck){
      hEvThetaOBandProt->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
      hSibTrelvICTrelRebinProtonOBand->Fill(si_array.T_B()-rftime.TMod2(),ic.T()-rftime.TMod2());
    }
  }

  if(NBandCheck){
    hSiPIDNBand->Fill(si_array.E_AB(),si_array.E_A()); 
    hEvThetaNBand->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    if(AlphaCheck){
      hEvThetaNBandAlpha->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    }
 
  }

  if(FBandBCheck){
    hSiPIDFBandB->Fill(si_array.E_AB(),si_array.E_A());
    hEvThetaFBandB->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
  }

  if(OBandBCheck){
    hEvThetaOBandB->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
    hSiPIDOBandB->Fill(si_array.E_AB(),si_array.E_A());
  }
  if(NBandBCheck){
    hSiPIDNBandB->Fill(si_array.E_AB(),si_array.E_A());
    hEvThetaNBandB->Fill(si_array.Theta_B()*RadToDeg(),si_array.E_AB());
  }


  if(ProtonCheck){
    hICEdEProt->Fill(ic.TotalE(),ic.DE());
    hSibTrelvICTrelRebinProton->Fill(si_array.T_B()-rftime.TMod2(),ic.T()-rftime.TMod2());
  }
  if(AlphaCheck){
    hICEdEAlpha->Fill(ic.TotalE(),ic.DE());
  }
  if(DeuteronCheck){
    hICEdEDeuteron->Fill(ic.TotalE(),ic.DE()); 
  }
  
    hSibTrelvICTrelRebin->Fill(si_array.T_B()-rftime.TMod2(),ic.T()-rftime.TMod2());

    if(ProtonCheck){
      hErvNeutT->Fill(fNeutTime,fErProton);
      hErvNeutKE_R->Fill(fNeutKE_R,fErProton);
      hErvNeutKE->Fill(fNeutKE,fErProton);
      hErvNeutQ->Fill(fNeutQ,fErProton);
    }
 
  return 1;
}
  


bool F17dn_Analyzer::Terminate(){
  return 1;
  
}


  void LoadF17AnalysisGates(const std::string & input){
    TFile in(input.c_str());    
    if(in.Get("gProton") && !gProton)
      gProton=new TCutG(*(TCutG*)in.Get("gProton"));
    if(in.Get("gAlpha") && !gAlpha)
      gAlpha=new TCutG(*(TCutG*)in.Get("gAlpha"));
    if(in.Get("gDeuteron") && !gDeuteron)
      gDeuteron=new TCutG(*(TCutG*)in.Get("gDeuteron"));
    if(in.Get("gFBand") && !gFBand)
      gFBand=new TCutG(*(TCutG*)in.Get("gFBand"));
    if(in.Get("gOBand") && !gOBand)
      gOBand=new TCutG(*(TCutG*)in.Get("gOBand"));   
    if(in.Get("gNBand") && !gNBand)
      gNBand=new TCutG(*(TCutG*)in.Get("gNBand")); 
    if(in.Get("gFBandB") && !gFBandB)
      gFBandB=new TCutG(*(TCutG*)in.Get("gFBandB"));
    if(in.Get("gOBandB") && !gOBandB)
      gOBandB=new TCutG(*(TCutG*)in.Get("gOBandB"));   
    if(in.Get("gNBandB") && !gNBandB)
      gNBandB=new TCutG(*(TCutG*)in.Get("gNBandB")); 
     in.Close();
    
  }
}
