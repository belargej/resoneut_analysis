/****************************************************
//Class: Mg24dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "../../include/RN_Root.hpp"
#include "Mg24dn_Analyzer.hpp"
#include "Q_ValueAnalyzer.hpp"

namespace _Mg24{
  TH1D * h_rftime;
  TH2D * h_QvRF;
  TH1D * h_neut_rftrel;
  TH2D *  h_Qvneut_rftrel;

   Mg24dn_Analyzer::Mg24dn_Analyzer()
  {
    
  }
  

  void Mg24dn_Analyzer::ResetGlobals(){

  }
  
  bool Mg24dn_Analyzer::Begin(){
    rootfile->mkdir("Mg24_analysis");
    rootfile->cd("Mg24_analysis");
    
    h_rftime = new TH1D("hrftime","hrftime;rftime[ns]",1024,0,1023); 
    h_QvRF =   new TH2D("QvRf","QvRF;RF;Qp",1024,0,1023,512,-1.,10.);
    h_neut_rftrel =   new TH1D("hneut_rftrel","hneut_rftrel;neut-rf[ns]",1024,-1023,0); 
    h_Qvneut_rftrel = new TH2D("QvneutRFrel","QvneutRFrel;neut-rf;Qp",1024,-1023,0,512,-1.,10.);   
    
  return 1;
  }
  
  
  bool Mg24dn_Analyzer::Process(){
    
    // if(nai_array.fE[0]<1500)
    //return 0;

    return 1;
  }
  bool Mg24dn_Analyzer::ProcessFill(){
    h_rftime->Fill(rftime[0].T());
    h_QvRF->Fill(rftime[0].T(),physical::q_val_p);
    h_neut_rftrel->Fill(coinc::neut_rf_trel);
    h_Qvneut_rftrel->Fill(coinc::neut_rf_trel,physical::q_val_p);
    return 1;
  }
  bool Mg24dn_Analyzer::Terminate(){
    
    return 1;
  
  }
  
  bool Mg24dn_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    
    return 1;
    
  }

  
  void Load_Mg24_dn_Gates(){
    
    
  }
}
