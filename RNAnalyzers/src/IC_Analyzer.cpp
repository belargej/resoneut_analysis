
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
  TCutG* posede;

  int  _require_hi1(0);
  int  _require_hi2(0);
  int  _require_hi3(0);
  int  gRequirePosEDE(0);


  int hi_check[3];
  int PosEdECheck(0);

  TH2D *hRawICDEvE;  
  TH2D *hRawICPosEvE;  
  TH2D *hRawICfDEvfE; 
  TH2D *hCalICDEvE;  
  TH2D *hCalICPosEvE;  
  TH2D *hCalICfDEvfE;   
  TH1D *h_IC_t;
  TH1D *h_IC_trel_rf;
  TH1D *hIC_Theta;
  TH2D *hIC_EvRF;
  TH2D *hIC_TvRF;
  TH1D *hIC_Phi;
  TH2D *hIC_EvTheta;
  TH2D * h_xvy;
  TH2D * hIC_de_v_xgride;
  TH1D *hxgrid_mult;
  TH1D *hygrid_mult;


  IC_Analyzer::IC_Analyzer()
  {
    
  }  
  bool IC_Analyzer::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    fgRootFile->mkdir("IC");
    fgRootFile->cd("IC");
    gDirectory->mkdir("ede");
    gDirectory->mkdir("t");
    gDirectory->mkdir("xy");
    gDirectory->mkdir("mult");
    gDirectory->cd("ede");
    gDirectory->mkdir("rawpar");
    gDirectory->mkdir("calpar");

    //create histograms
    fgRootFile->cd("IC/xy");
    h_xvy= new TH2D("h_xvy","h_xvy;x_chan;y_chan",128,-128,128,128,-128,128);
    hIC_Theta = new TH1D("h_ICTheta","h_ICTheta;Theta",180,0,179);
    hIC_EvTheta = new TH2D("h_IC_EvTheta","h_IC_EvTheta;Theta;E",180,0,179,512,0,8191);
    hIC_Phi = new TH1D("h_ICPhi","h_ICPhi;Phi",180,0,179);

    fgRootFile->cd("IC/ede/rawpar");
    hRawICDEvE=new TH2D("hRawICDEvE","IRawCDEvE; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICfDEvfE=new TH2D("hRawICfDEvfE","hRawICfDEvfE;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICPosEvE=new TH2D("hRawICPosEvE","hRawICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,8124);

    fgRootFile->cd("IC/ede/calpar");
    hCalICDEvE=new TH2D("hCalICDEvE","hCalICDEvE; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICfDEvfE=new TH2D("hCalICfDEvfE","hCalICfDEvfE;E [arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICPosEvE=new TH2D("hCalICPosEvE","hCalICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,1023);




    fgRootFile->cd("IC/t");
    h_IC_t = new TH1D("h_IC_t","ICt;t",1024,0,4095);
    h_IC_trel_rf = new TH1D("h_IC_trelRF","ICtrelRF;trel_RF",1024,0,4095);
    hIC_EvRF = new TH2D("hIC_EvRF","hIC_EvRF;RF;E",1024,0,4095,1024,0,4095);
    hIC_TvRF = new TH2D("hIC_TvRF","hIC_TvRF;RF;T",1024,0,4095,1024,0,4095);

    fgRootFile->cd("IC/mult");    
    hxgrid_mult = new TH1D("xgrid_mult","xgrid_mult;mult",32,0,31);
    hygrid_mult = new TH1D("ygrid_mult","ygrid_mult;mult",32,0,31);

    return 1;
  }

  void IC_Analyzer::Reset(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
    PosEdECheck = 0;

  }

  bool IC_Analyzer::Process(){
    ic.ReconstructHitPos();
    
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.TotalE(),ic.DE()));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.TotalE(),ic.DE()));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.TotalE(),ic.DE()));
    PosEdECheck = (posede && posede->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));

 
    if (_require_hi1 && !hi_check[0])
      return 0;
    if (_require_hi2 && !hi_check[1])
      return 0;
    if (_require_hi3 && !hi_check[2])
      return 0;

    if(gRequirePosEDE && !PosEdECheck)
      return 0;


    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){

    //multiple configurations of the E-DE:    
    //SumEPos = SumXE + SumYE
 
    hRawICDEvE->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());
    hRawICfDEvfE->Fill(ic.ERaw(),ic.DERaw());
    hRawICPosEvE->Fill(ic.ERaw() + ic.DERaw()+ic.xgrid.ERaw(0) + ic.ygrid.ERaw(0),ic.xgrid.ERaw(0)+ic.ygrid.ERaw(0));

    hCalICDEvE->Fill(ic.E()+ic.DE(),ic.DE());
    hCalICfDEvfE->Fill(ic.E(),ic.DE());
    hCalICPosEvE->Fill(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y());
    



    hxgrid_mult->Fill(ic.xgrid.Mult());
    hygrid_mult->Fill(ic.ygrid.Mult());

   
    h_xvy->Fill(ic.GetHitPos().X(),ic.GetHitPos().Y());
    hIC_Theta->Fill(ic.Theta());
    hIC_EvTheta->Fill(ic.Theta(),ic.TotalE());
    hIC_Phi->Fill(ic.Phi());
    h_IC_t->Fill(ic.T());
    h_IC_trel_rf->Fill(ic.T()-rftime.T_Wrapped());
    hIC_EvRF->Fill(rftime.fT,ic.TotalE());
    hIC_TvRF->Fill(rftime.fT,ic.T());

    
    
    return 1;
  }


  bool IC_Analyzer::Terminate(){

    return 1;
  }

  bool IC_Analyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    
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
  void RequirePosEDE(){
    gRequirePosEDE=1;
  }


  void LoadGates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("ede_hi1") && !ede_hi1)
      ede_hi1=new TCutG(*(TCutG*)in.Get("ede_hi1"));
    if(in.Get("ede_hi2") && !ede_hi2)
      ede_hi2=new TCutG(*(TCutG*)in.Get("ede_hi2"));
    if(in.Get("ede_hi3") && !ede_hi3)
      ede_hi3=new TCutG(*(TCutG*)in.Get("ede_hi3"));   
    if(in.Get("posede") && !posede)
      posede=new TCutG(*(TCutG*)in.Get("posede"));   
   
  
    in.Close();
  }
  
  void ClearGates(){
    if(ede_hi1)delete ede_hi1;
    if(ede_hi2)delete ede_hi2;
    if(ede_hi3)delete ede_hi3;



  }
}

#endif
