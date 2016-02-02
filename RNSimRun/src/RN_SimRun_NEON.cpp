////////////////////////////////////////////////////////////////////
// RN_Sim Binary:
// provides a main for a compilable version of RN_Sim
// that runs in terminal given a few arguments.  Currently it serves
// to quickly perform simulation given a single config file.
// 
// Author: Sean Kuvin 
////////////////////////////////////////////////////////////////////

#ifndef _RNSIMRUNNEON__CXX
#define _RNSIMRUNNEON__CXX

//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <numeric>

//ROOT libraties
#include <TROOT.h>
#include <TRint.h>
#include <TH1.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCutG.h>

#include "RN_Root.hpp"
#include "RN_SimRun_NEON.hpp"
#include "RN_ReactionInfo.hpp"
#include "RN_EnergyLoss.hpp"
#include "RN_Sim_NEON.hpp"

namespace sim{
 
  //declare histograms here
  TH1D* hn_tof;
  TH1D* htof_n[NEUTNUM];
  TH2D* hE_n;
  TH2D* hE_v_theta;
  TH2D* hT_v_theta;
  TH1D* hn_CM;
  TH1D* hn_CMReco;
  TH2D* hn_CMvLab;
  TH2D* hn_CMvCMReco;
  TH2D* hn_LabvCMReco[NEUTNUM];
  TH2D* hpos;
  TH2D* hpos_CM[NEUTNUM];
  TH2D* hpos_in;
  TH2D* hICSpot;
  TH1D* hQ;
  TH1D* hQ_n[NEUTNUM];
  TH1D* h_nKE;
  TH1D* h_hiKE;
  TH1D* hQ_proton;
  TH1D* hQ_proton_guessLV;
  TH1D* hQ_proton_guessSiA;
  TH1D* hQ_proton_guessSiB;
  TH2D* hQ_proton_guessvpE;
  TH2D* hQ_proton_guessSiBvpE;
  TH2D* hBeamSpot;
  TH2D* hEvThetaSiA;
  TH2D* hEvThetaSiB;
  TH2D* hHDPhivDecayPhi;
  TH2D* hICPhivSiPhi;
  TH2D* hErvRecoilE;

  //NEUTRECO from proton and heavy kinematics
  TH2D* hNeutLabAnglevReco;
  TH2D* hFragmentLabAnglevReco;
  TH2D* hNeutCMAnglevReco;
  TH2D* hNeutKEvReco;
  TH2D* hSiEvRecoAngle;
  TH1D* hErProtonReco;
  TH1D* hNeutQReco;
  TH2D* hNeutPhivReco;

  TF1* TOF_fit_n[NEUTNUM];
  TF1* Q_fit_n[NEUTNUM];
  TF1* Q_fit;
  TF1* TOF_fit;

}
using namespace sim;
using namespace RNROOT;
using namespace global;

void RN_SimRun_NEON::Print(){

}

RN_SimRun_NEON::RN_SimRun_NEON():fErProton(0),
		       fExHeavy(0),
		       fNCM_tree(0),
		       fICPos(TVector3(0,0,0))
{
 
}


//Create Histograms and Set up Tree
int RN_SimRun_NEON::Begin(){
    if(!fSimFile){
      std::cout<<"no fSimFile set"<<std::endl;
      exit(EXIT_FAILURE);
    }
  fSimFile->cd();
  
  fSimTree = new TTree("SimTree","SimTree");
  fSimTree->Branch("B." + RNROOT::gReactionInfo.BeamName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[0]));
  fSimTree->Branch("T." + RNROOT::gReactionInfo.TargetName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[1]));
  fSimTree->Branch("R." + RNROOT::gReactionInfo.RecoilName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[2]));
  fSimTree->Branch("F." + RNROOT::gReactionInfo.FragmentName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[3]));
  fSimTree->Branch("D." + RNROOT::gReactionInfo.DecayProductName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[4]));
  fSimTree->Branch("H." + RNROOT::gReactionInfo.HeavyDecayName(),"TLorentzVector",&(RNROOT::gReactionInfo.fLVarray[5]));
  fSimTree->Branch("ErProton",&fErProton);
  fSimTree->Branch("ExHeavy",&fExHeavy);
  fSimTree->Branch("NCM",&fNCM_tree);
  
  fSimTree->Branch("QVal_FragGuess",&QVal_FragGuess,"QVal_FragGuess/D");
  fSimTree->Branch("QVal_FragReal",&QVal_FragReal,"QVal_FragReal/D");

  fSimTree->Branch("QVal_InvMass",&QVal_InvMass,"QVal_InvMass/D");
  fSimTree->Branch("QVal_It",&QVal_It,"QVal_It/D");
  fSimTree->Branch("RecoilKinEn",&RecoilKinEn,"RecoilKinEn/D");
  fSimTree->Branch("RecoilTheta",&RecoilTheta,"RecoilTheta/D");
  fSimTree->Branch("NeutronEn",&NeutronEn,"NeutronEn/D");
  fSimTree->Branch("NeutronTheta",&NeutronTheta,"NeutronTheta/D");
  fSimTree->Branch("NeutronPhi",&NeutronPhi,"NeutronPhi/D");
  fSimTree->Branch("NeutronThetaCM",&NeutronThetaCM,"NeutronThetaCM/D");
  fSimTree->Branch("NeutronThetaCMSean",&NeutronThetaCMSean,"NeutronThetaCMSean/D");
  fSimTree->Branch("NeutronMass",&NeutronMass,"NeutronMass/D");
  fSimTree->Branch("ProtonThetaCM",&ProtonThetaCM,"ProtonThetaCM/D");

  fSimTree->Branch("SiEn",&SiEn,"SiEn/D");
  fSimTree->Branch("SiAng",&SiAng,"SiAng/D");
  fSimTree->Branch("ICEn",&ICEn,"ICEn/D");
  fSimTree->Branch("ICAngTheta",&ICAngTheta,"ICAngTheta/D");
  fSimTree->Branch("ICAngPhi",&ICAngPhi,"ICAngPhi/D");
  fSimTree->Branch("IterFragEnergy",&IterFragEnergy,"IterFragEnergy/D");

  fSimFile->cd();
  fSimFile->mkdir("histograms");

  fSimFile->cd("histograms");
  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMReco=new TH1D("h_nCMReco","h_nCMReco",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_CMvCMReco=new TH2D("hn_CMvCMReco","n_CMvCMReco",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",4096,1,128);
  hQ_proton=new TH1D("hQ_proton","hQ_proton;Q",512,-1,10);
  hQ_proton_guessLV=new TH1D("hQ_proton_guessLV","hQ_proton_guessLV;Q",512,-1,10);  
  hQ_proton_guessSiA=new TH1D("hQ_proton_guessSiA","hQ_proton_guessSiA;Q",512,-1,10);
  hQ_proton_guessSiB=new TH1D("hQ_proton_guessSiB","hQ_proton_guessSiB;Q",512,-1,10);
  hQ_proton_guessvpE=new TH2D("hQ_proton_guessvpE","hQ_proton_guessvpE;Q",512,-1,10,512,0,31);
  hQ_proton_guessSiBvpE=new TH2D("hQ_proton_guessSiBvpE","hQ_proton_guessSiBvpE;Q",512,-1,10,512,0,31);
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hEvThetaSiA=new TH2D("hEvThetaSiA","hEvThetaSiA",512,0,45,512,0,30);
  hEvThetaSiB=new TH2D("hEvThetaSiB","hEvThetaSiB",512,0,45,512,0,30);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hBeamSpot = new TH2D("hBeamSpot","hBeamSpot",128,-15.5,15.5,64,-15.5,15.5);
  hICSpot = new TH2D("hICSpot","hICSpot",64,-30,30,64,-30,30);
  hHDPhivDecayPhi = new TH2D("hHDPhivDecayPhi","hHDPhivDecayPhi",512,-180,180,512,-180,180);
  hICPhivSiPhi = new TH2D("hICPhivSiPhi","hICPhivSiPhi",512,-180,180,512,-180,180);
  hpos=new TH2D("hpos","hpos",64,-256,256,64,-256,256);
  hpos_in=new TH2D("hpos_in","hpos_in",64,-256,256,64,-256,256);  
  hE_n=new TH2D("hE_n","hE_n",17,0,16,512,0,5);
  TOF_fit=new TF1("tof_fit","gaus",1,128);
  hQ=new TH1D("hQ","hQ",512,-4,4);
  Q_fit=new TF1("Q_fit","gaus",-4,4);
  fSimFile->cd();

  //**************************************************************
  //Reconstructing Neutron Angle from proton and heavy kinematics
  gDirectory->mkdir("NeutronRECO");
  gDirectory->cd("NeutronRECO");
  hNeutLabAnglevReco = new TH2D("hNeutLabAnglevReco","hNeutLabAngleVReco",180,0,179,180,0,179);
  hFragmentLabAnglevReco = new TH2D("hFragmentLabAnglevReco","hFragmentLabAngleVReco",512,0,31,512,0,31);
  hNeutCMAnglevReco = new TH2D("hNeutCMAnglevReco","hNeutCMAngleVReco",180,0,179,180,0,179);
  hNeutKEvReco = new TH2D("hNeutKEvReco","hNeutKEvReco",128,0,31,128,0,31);
  hSiEvRecoAngle=new TH2D("hSiEvRecoAngle","hSiEvRecoAngle",512,0,45,512,0,30); 
  hErProtonReco=new TH1D("hErProtReco","hErProtReco;ErProtReco",512,-1,10); 
  hNeutQReco = new TH1D("hNeutQReco","hNeutQReco",512,-4,4);
  hNeutPhivReco = new TH2D("hNeutPhivReco","hNeutPhivReco",360,-179,179,360,-179,179);




  for(unsigned int i=0;i<neut.size();i++){
    fSimFile->cd("histograms");
    gDirectory->mkdir(Form("neut%d",i));
    gDirectory->cd(Form("neut%d",i));
    hn_LabvCMReco[i]=new TH2D(Form("hn_LabvCMReco_%d",i),Form("n_LabvCMReco_%d",i),128,120,174,64,164,179);
    hpos_CM[i]=new TH2D(Form("hpos_CM%d",i),Form("hpos_CM%d",i),64,-256,256,64,-256,256);
    htof_n[i]=new TH1D(Form("htof_n%d",i),Form("htof_n%d",i),512,1,128);  
    TOF_fit_n[i]=new TF1(Form("tof_fit_n%d",i),"gaus",1,128);
    hQ_n[i]=new TH1D(Form("hQ_n%d",i),Form("hQ_n%d",i),512,-4,4);
    Q_fit_n[i]=new TF1(Form("Q_fit_n%d",i),"gaus",-4,4);
  }
  return 1;
}


int RN_SimRun_NEON::Process(){
  SimRun_Reset();


  fErProton = gReactionInfo.DecayQValueEstimate(); 
  fNCM_tree=NCM();


  TLorentzVector hdlv = gReactionInfo.HeavyDecayLV();
  double hdtheta = hdlv.Theta();
  double hdphi = hdlv.Phi();
  double hdz = 340;
  double hdr = TMath::Tan(hdtheta) * hdz;
  double hdx = TMath::Cos(hdphi)*hdr + BeamSpot().X() + (-1.5 + myRnd.Rndm()*3);
  double hdy = TMath::Sin(hdphi)*hdr + BeamSpot().Y() + (-1.5 + myRnd.Rndm()*3);
  double icphi = TMath::ATan2(hdy,hdx) * TMath::RadToDeg();
  hdtheta = TMath::ATan2(TMath::Sqrt(hdx*hdx+hdy*hdy),hdz);

  fICPos.SetXYZ(hdx,hdy,hdz);
  //
  
  double heavyE = hdlv.E()-hdlv.M();
  double heavyP = TMath::Sqrt(2*hdlv.M()*heavyE);
  
  TLorentzVector heavyLV(heavyP*sin(hdtheta)*cos(hdphi),heavyP*sin(hdtheta)*sin(hdphi),heavyP*cos(hdtheta),hdlv.E());
  
 
  double protE = si_cluster_[1].E() + gReactionInfo.M_Decay_Product();
  double protP = TMath::Sqrt(protE*protE - gReactionInfo.M_Decay_Product()*gReactionInfo.M_Decay_Product());
  double protTheta = si_cluster_[1].Theta();
  double protPhi = si_cluster_[1].Phi();
   TLorentzVector protonLV(protP*sin(protTheta)*cos(protPhi),protP*sin(protTheta)*sin(protPhi),protP*cos(protTheta),si_cluster_[1].E()+gReactionInfo.M_Decay_Product());
  
  //  TLorentzVector protonLV(gReactionInfo.DecayProductLV());


  double beamE = gReactionInfo.BeamEnergy_Est() + gReactionInfo.M_Beam();
  double beamP = TMath::Sqrt(beamE*beamE-gReactionInfo.M_Beam()*gReactionInfo.M_Beam());
  
  TLorentzVector beamLV(0,0,beamP,beamE);
  
  double nPx = beamLV.X()-protonLV.X()-heavyLV.X();
  double nPy = beamLV.Y()-protonLV.Y()-heavyLV.Y();
  double nPz = beamLV.Z()-protonLV.Z()-heavyLV.Z();
  double nP = TMath::Sqrt(nPx*nPx+nPy*nPy+nPz*nPz);
  double nE = TMath::Sqrt(nP*nP + gReactionInfo.M_Recoil()*gReactionInfo.M_Recoil());

  fNeutReco.SetPxPyPzE(nPx,nPy,nPz,nE);

  
  fFragmentReco = beamLV + gReactionInfo.TargetLV() - fNeutReco;

  if(si_cluster_[1].E()>0){
    fExHeavy = gReactionInfo.IntHeavyQVal(si_cluster_[1].E(),heavyE,si_cluster_[1].Theta(),si_cluster_[1].Phi(),fICPos.Theta(),fICPos.Phi());
    
  }
  return 1;  
}


int RN_SimRun_NEON::ProcessFill(){
    TLorentzVector recoilLV = gReactionInfo.RecoilLV();
    int cref(0);
    double cmreco = 0;//gReactionInfo.GetCMAngle(recoilLV.Theta(),recoilLV.E()-recoilLV.M());
    hE_v_theta->Fill(recoilLV.Theta()*180/3.14,recoilLV.E()-recoilLV.M());
    hn_CM->Fill(NCM()*180/3.14);
    hn_CMvLab->Fill(NCM()*180/3.14,gReactionInfo.RecoilLV().Theta()*180/3.14);
    hn_CMvCMReco->Fill(NCM()*180/3.14,cmreco);


    //////
    hNeutLabAnglevReco->Fill(recoilLV.Theta()*TMath::RadToDeg(),fNeutReco.Theta()*TMath::RadToDeg());
    double NeutCMReco = 0;//gReactionInfo.GetCMAngle(fNeutReco.Theta(),fNeutReco.E()-fNeutReco.M());
    hNeutCMAnglevReco->Fill(NCM()*180/3.14,NeutCMReco);
    hNeutKEvReco->Fill(recoilLV.E()-recoilLV.M(),fNeutReco.E()-fNeutReco.M());
    hFragmentLabAnglevReco->Fill(gReactionInfo.FragmentLV().Theta()*TMath::RadToDeg(),fFragmentReco.Theta()*TMath::RadToDeg());
    double RecoRelAngle = si_cluster_[1].fPos[0].Angle(fFragmentReco.Vect());
    hSiEvRecoAngle->Fill(RecoRelAngle*TMath::RadToDeg(),si_cluster_[1].E());
    double ErProtonReco = gReactionInfo.DecayQValueEstimate(si_cluster_[1].E(),RecoRelAngle);
    double hiKEReco;
    hErProtonReco->Fill(ErProtonReco);
    hNeutQReco->Fill(gReactionInfo.RecoilQValue(fNeutReco.E()-fNeutReco.M(),fNeutReco.Theta(),hiKEReco));
    hNeutPhivReco->Fill(recoilLV.Phi()*TMath::RadToDeg(),fNeutReco.Phi()*TMath::RadToDeg());


    cref = 0 ;
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      if((*it).GetHitPos().X()!=0)
	hpos_in->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
      
      if((*it).fTSim>0){
	//hn_CMReco->Fill(gReactionInfo.GetCMAngle((*it).GetPosVect().Theta(),(*it).nKE_R((*it).fTSim)));
	hn_tof->Fill(((*it).fTSim));
	htof_n[cref]->Fill((*it).fTSim);
	hE_n->Fill(cref,(*it).fESum);
	hpos->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
	double nKE=0,hiKE=0;
	//	double q_value=gReactionInfo.RecoilQValue((*it).GetPosVect().Z(),(*it).fTSim,nKE,hiKE);
	//double q_value=gReactionInfo.RecoilQValue((*it).ZEst(),(*it).fTSim,nKE,hiKE);
	double q_value=gReactionInfo.RecoilQValue((*it).nKE_R((*it).fTSim),(*it).GetPosVect().Theta(),hiKE);
	hn_LabvCMReco[cref]->Fill(recoilLV.Theta()*180/3.14,cmreco);

	hQ->Fill(q_value);
	hQ_n[cref]->Fill(q_value);
	h_nKE->Fill(nKE);
	h_hiKE->Fill(hiKE);
	hT_v_theta->Fill(recoilLV.Theta()*180/3.14,(*it).fTSim);
	
      }
      cref++;
    }

    double q_val_p = gReactionInfo.DecayQValueExact();
    
    double q_val_p_guessLV = gReactionInfo.DecayQValueEstimate();
    
    QVal_FragGuess = q_val_p_guessLV;
    QVal_FragReal = q_val_p;
    


    double SiSig,ICSig,SiAngSig,ICAngThetaSig,ICAngPhiSig;
    gVariableMap.GetParam("SiliconSigma",SiSig);
    gVariableMap.GetParam("ICSigma",ICSig);
    gVariableMap.GetParam("SiAngSigma",SiAngSig);
    gVariableMap.GetParam("ICAngThetaSigma",ICAngThetaSig);
    gVariableMap.GetParam("ICAngPhiSigma",ICAngPhiSig);
    //std::cout << " Si Sig : " << SiSig << std::endl;
    //std::cout << " IC Sig : " << ICSig << std::endl;

    SiEn = global::myRnd.Gaus(gReactionInfo.DecayProductLV().E()-gReactionInfo.DecayProductLV().M(),SiSig);

    //std::cout << "=====================" << std::endl;
    //std::cout << " Heavy Energy : " << gReactionInfo.HeavyDecayLV().E()-gReactionInfo.BeamLV().M() << std::endl;

    // NOTE -- > I had to use the beam LV for the mass because when I change the total energy of 
    //           the lorentz vector for the heavy, it changes the M, not the KE.  So when I go 
    //           to call E-M, since M is different, T stays the same, and that is not what I want.
    //           Also, the mass does not change, so I use the initial mass from the beam, since
    //           they are the same particle.
    ICEn = global::myRnd.Gaus(gReactionInfo.HeavyDecayLV().E()-gReactionInfo.BeamLV().M(),ICSig);
    //ICEn = global::myRnd.Gaus(gReactionInfo.HeavyDecayLV().E()-gReactionInfo.HeavyDecayLV().M(),ICSig);
    SiAng = global::myRnd.Gaus(gReactionInfo.DecayProductLV().Theta(),SiAngSig);
    //std::cout << " Heavy Energy : " << ICEn << std::endl;
    ICAngTheta = global::myRnd.Gaus(gReactionInfo.HeavyDecayLV().Theta()*TMath::RadToDeg(),ICAngThetaSig);
    ICAngPhi = global::myRnd.Gaus(gReactionInfo.HeavyDecayLV().Phi()*TMath::RadToDeg(),ICAngPhiSig);

    ICAngTheta = ICAngTheta*TMath::DegToRad();
    ICAngPhi = ICAngPhi*TMath::DegToRad();
    //ICEn = gReactionInfo.HeavyDecayLV().E()-gReactionInfo.HeavyDecayLV().M();
    //SiEn = gReactionInfo.DecayProductLV().E()-gReactionInfo.DecayProductLV().M();

    double SiEn_Shift=0.0,ICEn_Shift=0.0,SiAng_Shift=0.0;
    gVariableMap.GetParam("SiEnShift",SiEn_Shift);
    gVariableMap.GetParam("ICEnShift",ICEn_Shift);
    gVariableMap.GetParam("SiAngShift",SiAng_Shift);

    SiAng_Shift = SiAng_Shift*TMath::DegToRad();
    //std::cout << " Si Ang Shift : " << SiAng_Shift*TMath::RadToDeg() << std::endl;
    //std::cout << " SI Ang       : " << SiAng*TMath::RadToDeg() << std::endl;
    //std::cout << " SiEn Shift : " << SiEn_Shift << std::endl;
    //std::cout << " ICEn Shift : " << ICEn_Shift << std::endl;
    //std::cout << " ICEn : " << ICEn << std::endl;
    SiEn_Shift = SiEn+SiEn_Shift;
    ICEn_Shift = ICEn+ICEn_Shift;
    //std::cout << " ICEn After : " << ICEn_Shift << std::endl;
    SiAng_Shift = SiAng+SiAng_Shift;
    //std::cout << "Si Ang After Shift : " << SiAng_Shift*TMath::RadToDeg() << std::endl;
    //QVal_It = gReactionInfo.DecayQValueIterations(SiEn, SiAng,5);
    QVal_It = gReactionInfo.DecayQValueIterations(SiEn_Shift, SiAng_Shift,5);
    IterFragEnergy =0;// gReactionInfo.DecayQValueIterations_RecEn(SiEn_Shift, SiAng,5);


    SiAng = SiAng*TMath::RadToDeg();

    QVal_InvMass = gReactionInfo.IntHeavyQVal(SiEn_Shift,ICEn_Shift,SiAng_Shift,gReactionInfo.DecayProductLV().Phi(),ICAngTheta,ICAngPhi);
    RecoilKinEn = gReactionInfo.IntHeavyKinEn(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    RecoilTheta = gReactionInfo.IntHeavyTheta(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    
    NeutronEn = gReactionInfo.NeutronEn(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    NeutronTheta = gReactionInfo.NeutronAngle(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    NeutronPhi =0;// gReactionInfo.NeutronPhi(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    NeutronThetaCM= gReactionInfo.NeutronAngleCM(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    ProtonThetaCM = gReactionInfo.ProtonAngleCM(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    NeutronMass= 0;//gReactionInfo.NeutronMass(gReactionInfo.DecayProductLV().E()-gReactionInfo.M_Decay_Product(),gReactionInfo.HeavyDecayLV().E()-gReactionInfo.M_Heavy_Decay(),gReactionInfo.DecayProductLV().Theta(),gReactionInfo.DecayProductLV().Phi(),gReactionInfo.HeavyDecayLV().Theta(),gReactionInfo.HeavyDecayLV().Phi());
    NeutronThetaCMSean = cmreco;



    hQ_proton->Fill(q_val_p);
    hQ_proton_guessLV->Fill(q_val_p_guessLV);
   
    if(si_cluster_[0].E()>0&& si_cluster_[1].E()>0){
      double q_val_p_guessSiA = gReactionInfo.DecayQValueEstimate(si_cluster_[0].E(),si_cluster_[0].Theta());
      double q_val_p_guessSiB = gReactionInfo.DecayQValueEstimate(si_cluster_[1].E(),si_cluster_[1].Theta());
      hEvThetaSiA->Fill(si_cluster_[0].Theta()*TMath::RadToDeg(),si_cluster_[0].E());
      hEvThetaSiB->Fill(si_cluster_[1].Theta()*TMath::RadToDeg(),si_cluster_[1].E());
      hQ_proton_guessSiA->Fill(q_val_p_guessSiA);
      hQ_proton_guessSiB->Fill(q_val_p_guessSiB);   
      hQ_proton_guessSiBvpE->Fill(q_val_p_guessSiB,si_cluster_[0].E());   
      hQ_proton_guessvpE->Fill(q_val_p_guessLV,si_cluster_[0].E());   
    }

    hICSpot->Fill(fICPos.X(),fICPos.Y());
    hHDPhivDecayPhi->Fill(gReactionInfo.DecayProductLV().Phi()*TMath::RadToDeg(),fICPos.Phi()*TMath::RadToDeg());
    hICPhivSiPhi->Fill(si_cluster_[0].Phi()*TMath::RadToDeg(),fICPos.Phi()*TMath::RadToDeg());
    hBeamSpot->Fill(BeamSpot().X(),BeamSpot().Y());
    fSimTree->Fill();
    
    return 1;
}





  
  int RN_SimRun_NEON::Terminate(){
    
    hQ->Fit(Q_fit,"","",-4,4);
    hn_tof->Fit(TOF_fit,"","",1,128);
    fSimLog<<"Reaction :"<<gReactionInfo.BeamName()<<"("<<gReactionInfo.TargetName()<<","<<gReactionInfo.RecoilName()<<")"<<gReactionInfo.FragmentName()<<"\n";
    fSimLog<<"Decay :"<<gReactionInfo.FragmentName()<<"->"<<gReactionInfo.DecayProductName() << "+" << gReactionInfo.HeavyDecayName()<<"\n\n";
      
      fSimLog<<"Beam Energy: "<<gReactionInfo.BeamEnergy()<<"\n";
      fSimLog<<"Beam E_loss(thickness): "<<gReactionInfo.BeamELoss()<<"\n";
      fSimLog<<"Excitation Energy: "<<gReactionInfo.Hi_Ex_Set()<<"\n";
      fSimLog<<"Number of Neutron Detectors: "<<neut.size()<<"\n";
      fSimLog<<"Total Entries: "<<TotEvents()<<"\n\n";

      fSimLog<<"All Detectors: \n";
      fSimLog<<"NeutronsDetected/NeutronsIn: "<<std::accumulate(fNeutronDetected.begin(),fNeutronDetected.end(),0)<<" / "<<std::accumulate(fNeutronIn.begin(),fNeutronIn.end(),0)<<"\n";
      
      fSimLog<<"Q_Value: "<<Q_fit->GetParameter(1)<<"\n ";
      fSimLog<<"Q_Value Resolution: "<<Q_fit->GetParameter(2) * 2.355<<"\n";      
      fSimLog<<"TOF_Value: "<<TOF_fit->GetParameter(1)<<"\n ";
      fSimLog<<"TOF_Value Resolution: "<<TOF_fit->GetParameter(2) * 2.355<<"\n\n";
      for(unsigned int i=0;i<si_.size();i++){
	fSimLog<<"***************************************************************\n";
	fSimLog<<si_[i].GetName()<<"\n";
	fSimLog<<"x,y,z: "<<si_[i].GetPosVect().X()<<" "
	      <<si_[i].GetPosVect().Y()<<" "
	      <<si_[i].GetPosVect().Z()<<"\n";
	fSimLog<<"Theta Range"<<si_[i].InnerTheta()<<" - "<<si_[i].OuterTheta()<<"\n";
	fSimLog<<"Protons&Neutron Coincidence / Protons In: "<< fProtonIn_NeutDet[i]<<" / "<<fProtonIn[i]<<"\n\n";
	
      }
          
      for(unsigned int i=0;i<neut.size();i++){
      
      htof_n[i]->Fit(TOF_fit_n[i],"","",1,128);
      hQ_n[i]->Fit(Q_fit_n[i],"","",-4,4);




      fSimLog<<"***************************************************************\n";
      fSimLog<<neut[i].GetName()<<"\n";
      fSimLog<<"Thickness: "<<neut[i].GetThickness()<<"\n";
      fSimLog<<"Threshold: "<<neut[i].GetThreshold()<<"\n";
      fSimLog<<"ArrayPos: "<<neut[i].GetArrayPos()<<"\n\n";

      fSimLog<<"NeutronsDetected/NeutronsIn: "<<fNeutronDetected[i]<<" / "<<fNeutronIn[i]<<"\n";
      fSimLog<<"Individual Efficiency: "<<(double)fNeutronDetected[i]/fNeutronIn[i]<<"\n";
      fSimLog<<"Q_Value: "<<Q_fit_n[i]->GetParameter(1)<<"\n ";
      fSimLog<<"Q_Value Resolution: "<<Q_fit_n[i]->GetParameter(2) * 2.355<<"\n";

      fSimLog<<"TOF_Value: "<<TOF_fit_n[i]->GetParameter(1)<<"\n ";
      fSimLog<<"TOF_Value Resolution: "<<TOF_fit_n[i]->GetParameter(2) * 2.355<<"\n" ;

      
    }



    fSimFile->Write();
    fSimFile->Close();
    fSimLog.close();
    return 1;
  }



#endif


int main(int argc, char*argv[])
{
  //std::cout << " OK So far 1 " << std::endl;
  if(argc==2){
    RNROOT::Initialize();//initialize default detector setup
   
    RN_SimRun_NEON a;
    a.Init();
    a.LoadConfig(argv[1]);
    a.Loop();

    return 1;
  }
  else{
    std::cout<<"RN_Sim expects one additional argument only for the configuration file"<<std::endl;
    exit(EXIT_FAILURE);
  }
}

 int RN_SimRun_NEON::SimRun_Reset(){
   fICPos.SetXYZ(0,0,0);
   fErProton = 0;
   fNCM_tree = 0;
   fExHeavy = 0;
   return 1;
 }
