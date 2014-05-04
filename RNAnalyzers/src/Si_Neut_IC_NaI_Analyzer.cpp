/***************************************************************
//Class: S2_NEUT_IC_NAI_Analyzer
//
//Author:Sean Kuvin
//
//S2_NEUT_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters,Ion 
//Chamber parameters and Neutron Detector parameters.  
//The results of sorting through
//the S2_Analyzer,the IC_analyzer,S2_IC_Analyzer, and the NeutAnalyzer 
//are necessary for proper function of this class. 
//Therefore, all 4 analyzers must be 
//added to the analyzer list and they all must be
// added before this analyzer is added
********************************************************************/
#ifndef _Si_NAI_NEUT_IC_ANALYZER_CXX
#define _Si_NAI_NEUT_IC_ANALYZER_CXX

#include "Si_Neut_IC_NaI_Analyzer.hpp"
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"
#include "Si_IC_Analyzer.hpp"
#include "IC_Analyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "Trigger_Analyzer.hpp"
#include "NaI_NeutAnalyzer.hpp"

using namespace RNROOT;

namespace coinc{

  TCutG* sia_n_time_cut;
  TCutG* sib_n_time_cut;
  
  int sia_n_time_check(0);
  int sib_n_time_check(0);
  int _require_sia_n_time_check(0);
  int _require_sib_n_time_check(0);

  TH1D *h_n_minus_sit;
  TH2D *h_n_t_v_si_t; 
  TH2D *h_nai_t_v_si_t;
  

  Si_Neut_IC_NaI_Analyzer::Si_Neut_IC_NaI_Analyzer()
  {
    
  }
  void Si_Neut_IC_NaI_Analyzer::ResetGlobals(){
    sia_n_time_check = 0;
    sib_n_time_check = 0;

  }

  bool Si_Neut_IC_NaI_Analyzer::Begin(){   
    
    if(!RNROOT::gRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    //create directory structure
    if(!RNROOT::gRootFile->GetDirectory("coinc"))
      RNROOT::gRootFile->mkdir("coinc");
    RNROOT::gRootFile->cd("coinc");
    gDirectory->mkdir("Si_Neut");
    gDirectory->mkdir("Si_NaI");
    gDirectory->cd("Si_Neut");
    gDirectory->mkdir("timing");
    RNROOT::gRootFile->cd("coinc/Si_NaI");
    gDirectory->mkdir("timing");

    //create histograms
    
    RNROOT::gRootFile->cd("coinc/Si_NaI/timing");
    h_nai_t_v_si_t = new  TH2D("h_nai_t_v_si_t","h_nai_t_v_si_t;nai_t;si_t",1024,0,4095,1024,0,4095);
    RNROOT::gRootFile->cd("coinc/Si_Neut/timing");
    h_n_t_v_si_t = new  TH2D("h_n_t_v_si_t","h_n_t_v_si_t;n_t;si_t",1024,0,4095,1024,0,4095);
    h_n_minus_sit=new TH1D("h_minus_sit","h_n_minus_sit;nt-sit",4096,-2048,2047);
    

    return 1;
  }


  

  bool Si_Neut_IC_NaI_Analyzer::Process(){
 
    sia_n_time_check = (sia_n_time_cut && sia_n_time_cut->IsInside(Narray.fT_first,si_cluster_[0].fT[0]));

    
    if(_require_sia_n_time_check && !sia_n_time_check){
      return 0;
    }
    
    return 1 ; 
  }    

  bool Si_Neut_IC_NaI_Analyzer::ProcessFill(){
    
    if(nai_array.fT[0]>0&&si_cluster_[0].fT[0]>0)
      h_nai_t_v_si_t->Fill(nai_array.fT[0],si_cluster_[0].fT[0]);
    
    if(Narray.fT_mult > 0 && si_cluster_[0].fT[0]>0){
      h_n_minus_sit->Fill(Narray.fT_first-si_cluster_[0].fT[0]);
      h_n_t_v_si_t->Fill(Narray.fT_first,si_cluster_[0].fT[0]);

    }
 
  return 1;
  }
    
    
  bool Si_Neut_IC_NaI_Analyzer::Terminate(){

    return 1;    
  }
  
  bool Si_Neut_IC_NaI_Analyzer::TerminateIfLast(){
    RNROOT::gRootFile->Write();
    RNROOT::gRootFile->Close();
    return 1;    
  }
  
  void Si_Neut_IC_NaI_Analyzer::Clear(){
    
  }
  
  void Require_SIA_N_TimePeak(){
    _require_sia_n_time_check = 1;

  }
  void Require_SIB_N_TimePeak(){
    _require_sib_n_time_check = 1;

  }


  void Load_Si_Neut_IC_NaI_Gates(const std::string & input){
    TFile in(input.c_str());    
    if(in.Get("sia_n_time_cut") && !sia_n_time_cut)
      sia_n_time_cut=new TCutG(*(TCutG*)in.Get("sia_n_time_cut"));
    if(in.Get("sib_n_time_cut") && !sib_n_time_cut)
      sib_n_time_cut=new TCutG(*(TCutG*)in.Get("sib_n_time_cut"));
    in.Close();
  }


}
#endif
