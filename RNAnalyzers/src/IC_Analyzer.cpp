
/***********************************************************/
//Class: IC_Analyzer
//
//Author:Sean Kuvin
//
//IC Analyzer class to be inserted in the analyzer list for 
//sorting Ion Chamber parameters. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Ion chamber 
//parameters on their own. 
/***********************************************************/

#ifndef _IC_ANALYZER_CXX
#define _IC_ANALYZER_CXX

#include "IC_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;

  int  _require_hi1(0);
  int  _require_hi2(0);
  int  _require_hi3(0);

  int hi_check[3];

  TH2D *hIC_ede;  
  TH2D *hIC_fefde;  
  TH1D *h_IC_t;
  TH1D *hIC_Theta;
  TH1D *hIC_Phi;
  TH2D *hIC_EvTheta;
  TH2D * h_xvy;
  TH2D * hIC_de_v_xgride;

  IC_Analyzer::IC_Analyzer()
  {
    
  }  
  bool IC_Analyzer::Begin(){   
    
    if(!RNROOT::gRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    RNROOT::gRootFile->mkdir("IC");
    RNROOT::gRootFile->cd("IC");
    gDirectory->mkdir("ede");
    gDirectory->mkdir("t");
    gDirectory->mkdir("xy");


    //create histograms
    RNROOT::gRootFile->cd("IC/xy");
    h_xvy= new TH2D("h_xvy","h_xvy;x_chan;y_chan",128,-128,128,128,-128,128);
    hIC_Theta = new TH1D("h_ICTheta","h_ICTheta;Theta",180,0,179);
    hIC_EvTheta = new TH2D("h_IC_EvTheta","h_IC_EvTheta;Theta;E",180,0,179,512,0,8191);
    hIC_Phi = new TH1D("h_ICPhi","h_ICPhi;Phi",180,0,179);
    RNROOT::gRootFile->cd("IC/ede");
    hIC_ede=new TH2D("hIC_EdE","IC_EdE;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    hIC_fefde=new TH2D("hIC_fEdE","hIC_fEdE;fE [arb];fdE [arb]",1024,0,4096,1024,0,4096);
    hIC_de_v_xgride=new TH2D("hIC_dE_v_xgride","hIC_dE_v_xgride;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    RNROOT::gRootFile->cd("IC/t");
    h_IC_t = new TH1D("h_IC_t","ICt;t",1024,0,4095);


    return 1;
  }

  void IC_Analyzer::ResetGlobals(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
 
  }

  bool IC_Analyzer::Process(){
    ic.ReconstructHitPos();
    
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.E(),ic.DE()));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.E(),ic.DE()));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.E(),ic.DE()));
    
    if (_require_hi1 && !hi_check[0])
      return 0;
    if (_require_hi2 && !hi_check[1])
      return 0;
    if (_require_hi3 && !hi_check[2])
      return 0;

    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){
    
    hIC_ede->Fill(ic.TotalE(),ic.DE());
    hIC_fefde->Fill(ic.E(),ic.DE());
    h_xvy->Fill(ic.GetHitPos().X(),ic.GetHitPos().Y());
    hIC_de_v_xgride->Fill(ic.DE(),ic.xgrid.fE[0]);
    hIC_Theta->Fill(ic.Theta());
    hIC_EvTheta->Fill(ic.Theta(),ic.TotalE());
    hIC_Phi->Fill(ic.Phi());
    h_IC_t->Fill(ic.T());
    

    
    
    return 1;
  }


  bool IC_Analyzer::Terminate(){

    return 1;
  }

  bool IC_Analyzer::TerminateIfLast(){
    RNROOT::gRootFile->Write();
    RNROOT::gRootFile->Close();
    
    return 1;
  }

  
  void IC_Analyzer::Clear(){
    
  }
  
  void RequireHI1(){
    _require_hi1=1;
  }
 void RequireHI2(){
    _require_hi2=1;
  }
 void RequireHI3(){
    _require_hi3=1;
  }


  void LoadGates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("ede_hi1") && !ede_hi1)
      ede_hi1=new TCutG(*(TCutG*)in.Get("ede_hi1"));
    if(in.Get("ede_hi2") && !ede_hi2)
      ede_hi2=new TCutG(*(TCutG*)in.Get("ede_hi2"));
    if(in.Get("ede_hi3") && !ede_hi3)
      ede_hi3=new TCutG(*(TCutG*)in.Get("ede_hi3"));   

  
    in.Close();
  }
  
  void ClearGates(){
    if(ede_hi1)delete ede_hi1;
    if(ede_hi2)delete ede_hi2;
    if(ede_hi3)delete ede_hi3;



  }
}

#endif
