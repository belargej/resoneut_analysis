
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
  
  TH2D * h_xvy;
  TH2D * hIC_de_v_xgride;

  using unpacker::TDC1;

  IC_Analyzer::IC_Analyzer()
  {
    
  }  
  bool IC_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    rootfile->mkdir("IC");
    rootfile->cd("IC");
    gDirectory->mkdir("ede");
    gDirectory->mkdir("t");
    gDirectory->mkdir("xy");


    //create histograms
    rootfile->cd("IC/xy");
    h_xvy= new TH2D("h_xvy","h_xvy;x_chan;y_chan",65,0,64,65,0,64);
    
    rootfile->cd("IC/ede");
    hIC_ede=new TH2D("hIC_EdE","IC_EdE;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    hIC_fefde=new TH2D("hIC_fEdE","hIC_fEdE;fE [arb];fdE [arb]",1024,0,4096,1024,0,4096);
    hIC_de_v_xgride=new TH2D("hIC_dE_v_xgride","hIC_dE_v_xgride;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    rootfile->cd("IC/t");
    h_IC_t = new TH1D("h_IC_t","ICt;t",1024,0,4095);


    return 1;
  }

  void IC_Analyzer::ResetGlobals(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
 
  }

  bool IC_Analyzer::Process(){

    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(IC_TotalE,IC_ELoss));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(IC_TotalE,IC_ELoss));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(IC_TotalE,IC_ELoss));
    
    if (_require_hi1 && !hi_check[0])
      return 0;
    if (_require_hi2 && !hi_check[1])
      return 0;
    if (_require_hi3 && !hi_check[2])
      return 0;

    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){
    
    hIC_ede->Fill(IC_TotalE,IC_ELoss);
    hIC_fefde->Fill(ic.fE,ic.fdE);
    h_xvy->Fill(ic.xgrid.Ch(),ic.ygrid.Ch());
    hIC_de_v_xgride->Fill(IC_ELoss,ic.xgrid.fE[0]);

    h_IC_t->Fill(ic.T());
    

    
    
    return 1;
  }


  bool IC_Analyzer::Terminate(){

    return 1;
  }

  bool IC_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    
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
