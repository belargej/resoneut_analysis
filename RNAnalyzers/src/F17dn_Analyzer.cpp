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
  TH2D *hICThetavSiTheta;
  TH2D *hICPhivSiPhi;

  TH2D *hNeutKE_RvTheta;
  TH2D *hNeutTvTheta;

  TH1D *hNeutT;
  TH1D *hNeutTheta;
  TH1D *hNeutKE_R;
  TH1D *hNeutKE;
  TH1D *hNeutQ;
  TH1D *hNeutQAngle;
  TH1D *hHiKE;

  TH2D *hErvRunNumber;
  TH2D *hNeutRFTRelvRunNumber;  
  TH2D *hICERawvRunNumber;  

  TH2D *hCalICDEvERebin;  
  TH2D *hCalICPosEvERebin;  
  TH2D *hCalICfDEvfERebin;


  TH2D *hEABvERecoAB;
  TH2D *hEAvERecoA;
  TH2D *hERecoABvThetaB;
  TH2D *hERecoABvThetaA;
  TH1D *hErReco;




  F17dn_Analyzer::F17dn_Analyzer():fErProton(0)
  {
    Reset();
  }


  void F17dn_Analyzer::Reset(){
    
    fErProton = 0;
    fErProtonReco = 0;
    fNeutTime = 0;
    fNeutKE_R = 0;
    fNeutKE = 0;
    fHiKE = 0;
    fNeutQ = 0;
    fNeutQAngle = 0;
    fNeutTheta = 0;

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
  gDirectory->mkdir("SiIC");
  gDirectory->mkdir("reco");


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

  hErvRunNumber = new TH2D("hErvRunNumber","hErvRunNumber;RunNumber;Er",1024,3000,4023,512,-1,10);

 

  fgRootFile->cd("F17dx_Analysis/IonChamber");
  hCalICDEvERebin=new TH2D("hCalICDEvERebin","hCalICDEvERebin; E + dE[arb];dE [arb]",1024,0,128,1024,0,128);
  hCalICfDEvfERebin=new TH2D("hCalICfDEvfERebin","hCalICfDEvfERebin;E [arb];dE [arb]",1024,0,128,1024,0,128);
  hCalICPosEvERebin=new TH2D("hCalICPosEvERebin","hCalICPosEvERebin;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,128,1024,0,128);
  hICEdEProt =new TH2D("hICEdEProt","hICEdEProt; E + dE[arb];dE [arb]",1024,0,128,1024,0,128);
  hICEdEAlpha =new TH2D("hICEdEAlpha","hICEdEAlpha; E + dE[arb];dE [arb]",1024,0,128,1024,0,128);
  hICEdEDeuteron =new TH2D("hICEdEDeuteron","hICEdEDeuteron; E + dE[arb];dE [arb]",1024,0,128,1024,0,128);

  fgRootFile->cd("F17dx_Analysis/TRel");
  hSibTrelvICTrelRebin = new TH2D("hSibTrelvICTrelRebin","hSibTrelvICTrelRebin",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProton = new TH2D("hSibTrelvICTrelRebinProton","hSibTrelvICTrelRebinProton",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProtonFBand = new TH2D("hSibTrelvICTrelRebinProtonFBand","hSibTrelvICTrelRebinProtonFBand",1024,-100,1434,1024,-100,1434);
  hSibTrelvICTrelRebinProtonOBand = new TH2D("hSibTrelvICTrelRebinProtonOBand","hSibTrelvICTrelRebinProtonOBand",1024,-100,1434,1024,-100,1434);
  
  fgRootFile->cd("F17dx_Analysis/Neut");
  hNeutRFTRelvRunNumber = new TH2D("hNeutRFTRelvRunNumber","hNeutRFTrelvRunNumber;RunNumber;NeutRFTRel",1024,3000,4023,1024,0,4095);
  hErvNeutT = new TH2D ("hErvNeutT","hErvNeutT",256,-100,410,512,-1,10);
  hErvNeutKE_R = new TH2D ("hErvNeutKE_R","hErvNeutKE_R",256,0,1,512,-1,10);
  hErvNeutKE = new TH2D ("hErvNeutKE","hErvNeutKE",256,0,1,512,-1,10);
  hErvNeutQ = new TH2D ("hErvNeutQ","hErvNeutQ",256,-10,10,512,-1,10);
  hNeutT = new TH1D ("hNeutT","hNeutT",256,-100,410);
  hNeutTheta = new TH1D ("hNeutTheta","hNeutTheta",180,0,179);
  hNeutKE_R = new TH1D ("hNeutKE_R","hNeutKE_R",256,0,1);
  hNeutKE = new TH1D ("hNeutKE","hNeutKE",256,0,1);
  hNeutQ = new TH1D ("hNeutQ","hNeutQ",256,-10,10);
  hNeutQAngle = new TH1D ("hNeutQAngle","hNeutQAngle",256,-10,10);
  hHiKE = new TH1D ("hHiKE","hHiKE",256,-10,10);
  hNeutKE_RvTheta = new TH2D ("hNeutKE_RvTheta","hNeutKE_RvTheta",180,0,179,256,0,1);
  hNeutTvTheta = new TH2D ("hNeutTvTheta","hNeutTvTheta",180,0,179,256,0,128);



  fgRootFile->cd("F17dx_Analysis/SiIC");
  hICThetavSiTheta = new TH2D("hICThetavSiTheta","hICThetavSiTheta;SiTheta;ICTheta",256,0,31,128,0,15);
  hICPhivSiPhi = new TH2D("hICPhivSiPhi","hICPhivSiPhi;SiPhi;ICPhi",360,-180,180,360,-180,180);
  hICERawvRunNumber = new TH2D("hICERawvRunNumber","hICERawvRunNumber;RunNumber;ICERaw",1024,3000,4023,512,0,4095);


  fgRootFile->cd("F17dx_Analysis/reco");
  hEABvERecoAB = new TH2D("hEABvERecoAB","hEBvERecoAB",512,0,32,512,0,32);
  hEAvERecoA = new TH2D("hEAvERecoA","hEAvERecoA",128,0,5,128,0,5);
  hERecoABvThetaB = new TH2D("hERecoABvThetaB","hERecoABvThetaB",256,0,31,512,0,31);
  hERecoABvThetaA = new TH2D("hERecoABvThetaA","hERecoABvThetaA",256,0,31,512,0,31);
  hErReco = new TH1D("hErReco","hErReco",512,-1,10);

  return 1;
 
}

bool F17dn_Analyzer::Process(){

  for(unsigned int i=0; i<neut.size();i++){
    if(psd::rawneutcheck[i]){
      fNeutTime = (neut[i].T() - rftime.TMod2() - 43.55); //43.55 is ansatz on t=zero
      fNeutKE_R = neut[i].nKE_R(fNeutTime);
      fNeutQ = gReactionInfo.RecoilQValue(neut[i].GetPosVect().Z(),fNeutTime,fNeutKE,fHiKE);
      fNeutTheta = (neut[i].GetPosVect().Theta()*TMath::RadToDeg());
      fNeutQAngle = gReactionInfo.RecoilQValue(fNeutKE_R,fNeutTheta*TMath::DegToRad(),fHiKE);
      break;
    }
  }
  
  if(si_array.E_AB(),si_array.Theta_B()){
    fErProton = gReactionInfo.DecayQValueEstimate(si_array.E_AB(),si_array.Theta_B());
    fErProtonReco = gReactionInfo.DecayQValueEstimate(si_array.ERecoAB(),si_array.Theta_B());
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
  hErvRunNumber->Fill(gMainAnalyzer.EventInfo(0),fErProton);;
  hNeutRFTRelvRunNumber->Fill(gMainAnalyzer.EventInfo(0),fNeutTime);
  hICERawvRunNumber->Fill(gMainAnalyzer.EventInfo(0),ic.ERaw());

  hCalICDEvERebin->Fill(ic.E()+ic.DE(),ic.DE());
  hCalICfDEvfERebin->Fill(ic.E(),ic.DE());
  hCalICPosEvERebin->Fill(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y());
    


  hEABvERecoAB->Fill(si_array.E_AB(),si_array.ERecoAB());
  hEAvERecoA->Fill(si_array.E_A(),si_array.ERecoA());
  hERecoABvThetaB->Fill(si_array.Theta_B()*RadToDeg(),si_array.ERecoAB());
  hERecoABvThetaA->Fill(si_array.Theta_A()*RadToDeg(),si_array.ERecoAB());
  hErReco->Fill(fErProtonReco);



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
      hNeutT->Fill(fNeutTime);
      hNeutKE_R->Fill(fNeutKE_R);
      hNeutKE->Fill(fNeutKE);
      hNeutQ->Fill(fNeutQ);
      hNeutTheta->Fill(fNeutTheta);
      hNeutKE_RvTheta->Fill(fNeutTheta,fNeutKE_R);
      hNeutTvTheta->Fill(fNeutTheta,fNeutTime);
      hNeutQAngle->Fill(fNeutQAngle);
      hHiKE->Fill(fHiKE);
    }

    hICThetavSiTheta->Fill(si_array.Theta_B()*RadToDeg(),ic.Theta());
    hICPhivSiPhi->Fill(si_array.Phi_B()*RadToDeg(),ic.Phi());


 
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
