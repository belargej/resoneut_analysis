/***************************************************************/
//Class: Si_IC_Analyzer
//
//Author:Sean Kuvin
//
//Si_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters
//alongside Ion Chamber parameters.  The results of sorting through
//the S2_Analyzer and the IC_analyzer are necessary for proper function
//of this class. Therefore, both S2_Analyzer and IC_Analyzer must be 
//added to the analyzer list and they both must be added before this analyzer
//is added
/********************************************************************/


#ifndef _Si_IC_ANALYZER_CXX
#define _Si_IC_ANALYZER_CXX

#include "Si_IC_Analyzer.hpp"
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"
#include "IC_Analyzer.hpp"

using namespace RNROOT;

namespace coinc{
  Double32_t sia_rf_trel(0);
  Double32_t sib_rf_trel(0);
  Double32_t ic_rf_trel(0);

  TCutG* si_ic_time1;
  TCutG* si_ic_time2;
  int si_ic_tcheck(0);
  int _require_si_ic_time(0);

  TH2D* h_si_t_v_ic_t;
  TH2D* h_si_trel_v_ic_t;
  TH2D *h_sia_trel_v_ic_trel;
  TH2D *h_sib_trel_v_ic_trel;
  
  void Require_Si_IC_Time(){
    _require_si_ic_time = 1;
  }

  Si_IC_Analyzer::Si_IC_Analyzer()
  {
    
  }
  void Si_IC_Analyzer::Reset(){
    sia_rf_trel = 0;
    sib_rf_trel = 0;
    ic_rf_trel = 0;
    si_ic_tcheck=0;
  }

  bool Si_IC_Analyzer::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    if(!fgRootFile->GetDirectory("coinc"))
      fgRootFile->mkdir("coinc");
    fgRootFile->cd("coinc");
    gDirectory->mkdir("Si_IC");
    gDirectory->cd("Si_IC");
    gDirectory->mkdir("timing");


    fgRootFile->cd("coinc/Si_IC/timing");
    h_si_t_v_ic_t= new TH2D("h_si_t_v_ic_t","h_si_t_v_ic_t;si_t;ic_t",1024,0,4095,1024,0,4095);
    h_si_trel_v_ic_t=new TH2D("h_si_trel_v_ic_t","h_si_trel_v_ic_t;si_trel;ic_t",1024,-2047,2048,1024,0,4095);
    h_sia_trel_v_ic_trel = new TH2D("sia_trel_v_ic_trel","sia_trel_v_ic_trel;sia_trel[ns];ic_trel[ns]",1024,-1023.,1023.,1024,-1023.,1023.);
    h_sib_trel_v_ic_trel = new TH2D("sib_trel_v_ic_trel","sib_trel_v_ic_trel;sib_trel[ns];ic_trel[ns]",1024,-1023.,1023.,1024,-1023.,1023.);

    return 1;
  }


  bool Si_IC_Analyzer::Process(){
    sia_rf_trel = si_[0].Back_T(0) - rftime.T_Wrapped();
    sib_rf_trel = si_[1].Back_T(0) - rftime.T_Wrapped();
    ic_rf_trel = ic.T() - rftime.T_Wrapped();

    si_ic_tcheck = (si_ic_time1 &&si_ic_time1->IsInside(coinc::sib_rf_trel,coinc::ic_rf_trel)); 
    //si_ic_tcheck = si_ic_tcheck || (si_ic_time2 && si_ic_time2->IsInside(coinc::sia_rf_trel,coinc::ic_rf_trel));
    
    if(_require_si_ic_time && !si_ic_tcheck){
      return 0;
    }

    return 1;
  }

  bool Si_IC_Analyzer::ProcessFill(){
    h_si_t_v_ic_t->Fill(si_cluster_[0].TRaw(0),ic.T());
    h_si_trel_v_ic_t->Fill(rftime.T() - si_cluster_[0].TRaw(0),ic.T());
    h_sia_trel_v_ic_trel->Fill(coinc::sia_rf_trel,coinc::ic_rf_trel);
    h_sib_trel_v_ic_trel->Fill(coinc::sib_rf_trel,coinc::ic_rf_trel);
    return 1;
  }
  
  
  
  
  bool Si_IC_Analyzer::Terminate(){

    return 1;
  }

  bool Si_IC_Analyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    return 1;
  }
  
  void Si_IC_Analyzer::Clear(){
    
  }
  


  void Load_Si_IC_Gates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("si_ic_time1") && !si_ic_time1)
      si_ic_time1=new TCutG(*(TCutG*)in.Get("si_ic_time1"));
    if(in.Get("si_ic_time2") && !si_ic_time2)
      si_ic_time2=new TCutG(*(TCutG*)in.Get("si_ic_time2"));

    in.Close();

  }




}


#endif
