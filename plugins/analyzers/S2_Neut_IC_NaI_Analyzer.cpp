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
#ifndef _S2_NAI_NEUT_IC_ANALYZER_CXX
#define _S2_NAI_NEUT_IC_ANALYZER_CXX

#include "S2_Neut_IC_NaI_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "S2_Analyzer.hpp"
#include "S2_IC_Analyzer.hpp"
#include "IC_Analyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "Trigger_Analyzer.hpp"
#include "NaI_NeutAnalyzer.hpp"

namespace coinc{

  TCutG* s1_n_time_cut;
  TCutG* s2_n_time_cut;
  
  double si_t(0);

  int s1_n_time_check(0);
  int s2_n_time_check(0);
  int _require_s1_n_time_check(0);

  sak::Histogram1D *h_n_minus_sit;
  sak::Histogram1D *h_n_minus_sitfirst;

  sak::Histogram2D *hpede_rawneut;    
  sak::Histogram2D *hpede_rawgamma;  
  sak::Histogram2D *hpede_rawneutsansgamma;  
  sak::Histogram2D *h_n_t_v_si_t;
  sak::Histogram2D *h_n_t_v_si_t_protongated;
  sak::Histogram2D *h_nai_t_v_si_t;
  sak::Histogram2D *h_n_t_v_si_t_F17_prot_timing;
  sak::Histogram2D *h_n_t_v_si_trel;
  sak::Histogram2D *h_n_trel_v_si_t;
  sak::Histogram2D *h_n_t_v_si_t_ngated;
  sak::Histogram2D *h_n_t_v_si_trel_ngated;
  sak::Histogram2D *h_n_trel_v_si_t_ngated;
  sak::Histogram2D *h_n_t_v_si_t_n_F17_gated;
  sak::Histogram2D *h_n_t_v_si_trel_n_F17_gated;
  sak::Histogram2D *h_n_trel_v_si_t_n_F17_gated;

  sak::Histogram2D *hevt_rawneut;


  sak::Histogram2D *h_n_t_v_si_t_F17_gated;
  sak::Histogram2D *h_n_t_v_si_trel_F17_gated;
  sak::Histogram2D *h_n_trel_v_si_t_F17_gated;

  
  sak::Histogram2D *rfvs1_t_rel_prot_F17;
  sak::Histogram2D *rfvs1_t_rel_prot_O16;

  sak::Histogram2D *hPSDq_n_prot_t_F17[NEUTNUM];
  sak::Histogram2D *hPSDq_n_prot_F17[NEUTNUM];
  sak::Histogram2D *hPSDq_n_prot[NEUTNUM];

  S2_Neut_IC_NaI_Analyzer::S2_Neut_IC_NaI_Analyzer()
  {
    
  }
  void S2_Neut_IC_NaI_Analyzer::ResetGlobals(){
    s1_n_time_check = 0;
    s2_n_time_check = 0;
    si_t = 0;

  }

  bool S2_Neut_IC_NaI_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    rfvs1_t_rel_prot_F17=new sak::Hist2D("rfvs1_t_rel_prot_F17","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);
    rfvs1_t_rel_prot_O16=new sak::Hist2D("rfvs1_t_rel_prot_O16","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);


    rootfile->mkdir("coinc/S2_Neut");
    rootfile->mkdir("coinc/S2_Neut/PSD");

    rootfile->mkdir("coinc/S2_Neut/ede");
    rootfile->mkdir("coinc/S2_Neut/timing");
    rootfile->mkdir("coinc/S2_Neut/timing/neutPSD");
    rootfile->mkdir("coinc/S2_Neut/timing/neutPSD/F17");
    rootfile->mkdir("coinc/S2_Neut/timing/F17");

    rootfile->mkdir("coinc/NaI_S2");    



    rootfile->cd("coinc/S2_Neut/PSD");
    for(int i=0;i<NEUTNUM;i++){
      hPSDq_n_prot_t_F17[i]=new sak::Histogram2D(Form("hPSDq_n%d_prot_t_F17",i),"fQ_long","fQ_short",1024,0,4096,1024,0,4096);
      hPSDq_n_prot[i]=new sak::Histogram2D(Form("hPSDq_n%d_prot",i),"fQ_long","fQ_short",1024,0,4096,1024,0,4096);
      hPSDq_n_prot_F17[i]=new sak::Histogram2D(Form("hPSDq_n%d_prot_F17",i),"fQ_long","fQ_short",1024,0,4096,1024,0,4096);
    }

    rootfile->cd("coinc/S2_Neut/ede");
    hpede_rawneut=new sak::Hist2D("hpEdE_rawneut","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_rawneutsansgamma=new sak::Hist2D("hpEdE_rawneut_sansgamma","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_rawgamma=new sak::Hist2D("hpEdE_rawgamma","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hevt_rawneut=new sak::Hist2D("hpEvT_rawneut","Theta","E [MeV]",180,0,179,256,0,20);


    rootfile->cd("coinc/S2_Neut/timing");
    h_n_t_v_si_t = new  sak::Hist2D("h_n_t_v_si_t","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_t_protongated = new  sak::Hist2D("h_n_t_v_si_t_protongated","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_t_F17_prot_timing = new  sak::Hist2D("h_n_t_v_si_t_F17_prot_timing","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_trel = new  sak::Hist2D("h_n_t_v_si_trel","n_t","si_t - rftime",1024,0,4095,1024,-2097,2098);
    h_n_trel_v_si_t = new  sak::Hist2D("h_n_trel_v_si_t","n_t-rftime","si_t",1024,-2097,2098,1024,0,4096);
    h_n_minus_sit=new sak::Histogram1D("h_n_minus_sit","nt-sit",256,-2048,2047);
    h_n_minus_sitfirst=new sak::Histogram1D("h_n_minus_sitfirst","nt-sitfirst",256,-2048,2047);


    
    rootfile->cd("coinc/S2_Neut/timing/neutPSD");
    h_n_t_v_si_t_ngated = new  sak::Hist2D("h_n_t_v_si_t_neutg","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_trel_ngated = new  sak::Hist2D("h_n_t_v_si_trel_neutg","n_t","si_t - rftime",1024,0,4095,1024,-2097,2098);
    h_n_trel_v_si_t_ngated = new  sak::Hist2D("h_n_trel_v_si_t_neutg","n_t-rftime","si_t",1024,-2097,2098,1024,0,4096);
    rootfile->cd("coinc/S2_Neut/timing/neutPSD/F17");
    h_n_t_v_si_t_n_F17_gated = new  sak::Hist2D("h_n_t_v_si_t_neut_F17g","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_trel_n_F17_gated = new  sak::Hist2D("h_n_t_v_si_trel_neut_F17g","n_t","si_t - rftime",1024,0,4095,1024,-2097,2098);
    h_n_trel_v_si_t_n_F17_gated = new  sak::Hist2D("h_n_trel_v_si_t_neut_F17g","n_t-rftime","si_t",1024,-2097,2098,1024,0,4096);
    
 
    
    rootfile->cd("coinc/S2_Neut/timing/F17");
    h_n_t_v_si_t_F17_gated = new  sak::Hist2D("h_n_t_v_si_t_F17g","n_t","si_t",1024,0,4095,1024,0,4095);
    h_n_t_v_si_trel_F17_gated = new  sak::Hist2D("h_n_t_v_si_trel_F17g","n_t","si_t - rftime",1024,0,4095,1024,-2097,2098);
    h_n_trel_v_si_t_F17_gated = new  sak::Hist2D("h_n_trel_v_si_t_F17g","n_t-rftime","si_t",1024,-2097,2098,1024,0,4096);

    
    rootfile->cd("coinc/NaI_S2");    
    h_nai_t_v_si_t=new sak::Hist2D("h_nai_t_v_si_t","nai_t","si_t",1024,0,4095,1024,0,4095);

    
    return 1;
  }




  bool S2_Neut_IC_NaI_Analyzer::Process(){
    si_t=si_[0].Back_T(0);    
    s1_n_time_check = (s1_n_time_cut && s1_n_time_cut->IsInside(neut_t,si_t));

    
    if(_require_s1_n_time_check && !s1_n_time_check){
      return 0;
    }
    
    return 1 ; 
  }    

  bool S2_Neut_IC_NaI_Analyzer::ProcessFill(){
    
    if(nai_t>0&&si_t>0)
      h_nai_t_v_si_t->Fill(nai_t,si_t);
    
    if(neut_t>0 && si_t>0){
      h_n_minus_sit->Fill(neut_t-si_t);
      h_n_minus_sitfirst->Fill(neut_t-trigger::s1_tfirst);
      h_n_t_v_si_t->Fill(neut_t,si_t);
      h_n_t_v_si_trel->Fill(neut_t,rftime[0].fT-si_t);
      h_n_trel_v_si_t->Fill(rftime[0].fT-neut_t,si_t);
      
      if(ionchamber::hi_check[0]&& coinc::si_ic_tcheck&&si_cal::protcheck){
	h_n_t_v_si_t_F17_prot_timing->Fill(neut_t,si_t);
      }
      if(si_cal::protcheck){
	h_n_t_v_si_t_protongated->Fill(neut_t,si_t);
      }
      

      if(psd::rawneut_orcheck){
	h_n_t_v_si_t_ngated->Fill(neut_t,si_t);
	h_n_t_v_si_trel_ngated->Fill(neut_t,rftime[0].fT-si_t);
	h_n_trel_v_si_t_ngated->Fill(rftime[0].fT-neut_t,si_t);
	if(ionchamber::hi_check[0]){
	  h_n_t_v_si_t_n_F17_gated->Fill(neut_t,si_t);
	  h_n_t_v_si_trel_n_F17_gated->Fill(neut_t,rftime[0].fT-si_t);
	  h_n_trel_v_si_t_n_F17_gated->Fill(rftime[0].fT-neut_t,si_t);

	}
      }
    }
     
    if(si_cal::protcheck && psd::rawneut_orcheck){
      hevt_rawneut->Fill(si_cal::prot_theta,si_cal::prot_E);
    }
   
	
      for(int i=0;i<NEUTNUM;i++){
	if(i>=neut.size())
	  break;
	if(neut[i].fQ_long>0){
	  if(coinc::si_ic_tcheck&&ionchamber::hi_check[0]&&si_cal::protcheck){
	    hPSDq_n_prot_t_F17[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
	  }
	  if(si_cal::protcheck)
	    hPSDq_n_prot[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
	  
	  if(si_cal::protcheck&&ionchamber::hi_check[0])
	    hPSDq_n_prot_F17[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);


	}


      }
    
    
    
    
    
    
    
    
    
    if(si_cal::protcheck&&ionchamber::hi_check[0]){
      rfvs1_t_rel_prot_F17->Fill(rftime[0].fT,rftime[0].fT-si_t);
    }
    
    if(si_cal::protcheck&&ionchamber::hi_check[1]){
      rfvs1_t_rel_prot_O16->Fill(rftime[0].fT,rftime[0].fT-si_t);
      
    }

    
    if(ionchamber::hi_check[0]){
      h_n_t_v_si_t_F17_gated->Fill(neut_t,si_t);
      h_n_t_v_si_trel_F17_gated->Fill(neut_t,rftime[0].fT-si_t);
      h_n_trel_v_si_t_F17_gated->Fill(rftime[0].fT-neut_t,si_t);	
    }
    

  
  
  
  if(psd::rawneut_orcheck)
    hpede_rawneut->Fill(si_cal::prot_E,si_cal::prot_dE);
  if(psd::rawgammacheck[0])
    hpede_rawgamma->Fill(si_cal::prot_E,si_cal::prot_dE);
  if(psd::rawneut_sansrawgamma_orcheck)
    hpede_rawneutsansgamma->Fill(si_cal::prot_E,si_cal::prot_dE);
  

  return 1;
  }
    
    
  bool S2_Neut_IC_NaI_Analyzer::Terminate(){

    return 1;    
  }
  
  bool S2_Neut_IC_NaI_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    return 1;    
  }
  
  void S2_Neut_IC_NaI_Analyzer::Clear(){
    
  }
  
  void Require_S1_N_TimePeak(){
    _require_s1_n_time_check = 1;

  }


  void Load_S2_Neut_IC_Gates(const std::string & input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("s1_n_time_cut") && !s1_n_time_cut)
      s1_n_time_cut=new TCutG(*in.getCut("s1_n_time_cut"));
    if(in.getCut("s2_n_time_cut") && !s2_n_time_cut)
      s2_n_time_cut=new TCutG(*in.getCut("s2_n_time_cut"));
    in.Close();
  }


}
#endif
