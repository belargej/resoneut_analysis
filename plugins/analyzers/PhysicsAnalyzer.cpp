/***********************************************************/
//Class: Physics_Analyzer
//
//Author:Sean Kuvin
//
//Physics Analyzer class to be inserted in the analyzer list for 
//calculating physical parameters using the detector parameters
/***********************************************************/

#ifndef _PHYSICS_ANALYZER_CXX
#define _PHYSICS_ANALYZER_CXX

#include "PhysicsAnalyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "../../include/RN_Unpack2Root.hpp"
#include "S2_IC_Analyzer.hpp"
#include "S2_Analyzer.hpp"
#include "IC_Analyzer.hpp"

namespace physical{

  sak::Hist1D *Q_Value_proton;
  sak::Hist1D *Q_Value_protontheta;
  sak::Hist1D *Q_Value_protontheta_F17;
  sak::Hist1D *Q_Value_proton_F17;
  sak::Hist1D *Q_Value_proton_O16;

  Double32_t q_val_p;
  Double32_t q_val_n;


  Physics_Analyzer::Physics_Analyzer()
  {
  
  }


  bool Physics_Analyzer::Begin(){

    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    if(!global::m_beam || !global::m_target || !global::m_recoil || !global::m_frag || !global::m_decay_product || !global::m_heavy_decay){
      std::cout<<"Reaction Masses have not been set"<<std::endl;
      exit(EXIT_FAILURE);

    }
    if(!global::E_fragment)
      std::cout<<"Heavy Ion energy ansatz for Q value reconstruction needed"<<std::endl;

    rootfile->mkdir("physical");
    rootfile->mkdir("physical/Q");
  
    rootfile->cd("physical/Q");
    Q_Value_proton=new sak::Hist1D("Q_val_proton","Q_value",1024,-10,10);
    Q_Value_proton_F17=new sak::Hist1D("Q_val_proton_F17","Q_value",1024,-10,10);
    Q_Value_proton_O16=new sak::Hist1D("Q_val_proton_O16","Q_value",1024,-10,10);


    rootfile->cd();
    return 1;

 
  }

  void Physics_Analyzer::ResetGlobals(){
    q_val_p=0;
    q_val_n=0;
  }

  bool Physics_Analyzer::Process(){
    //global::E_fragment is ansatz from MC simulation
  
    if(si_cal::prot_E>0&& si_cal::prot_theta!=0){
    
      q_val_p = (si_cal::prot_E * (1 + (global::m_decay_product / global::m_heavy_decay)) 
		 - (global::E_fragment * ( 1 - (global::m_frag / global::m_heavy_decay)))
		 - ((2 / global::m_heavy_decay) * TMath::Sqrt(si_cal::prot_E * global::m_decay_product * global::E_fragment * global::m_frag) * cos(3.14*si_cal::prot_theta/180)));
    
    }

    return 1;
  }
  
  bool Physics_Analyzer::ProcessFill(){
    if(si_cal::protcheck){
      Q_Value_proton->Fill(q_val_p);
      
      if(ionchamber::hi_check[0]){
	Q_Value_proton_F17->Fill(q_val_p);
      }
      if(ionchamber::hi_check[1]){
	Q_Value_proton_O16->Fill(q_val_p);
      }

    }


    return 1;

  }
  


  bool Physics_Analyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();

    return 1;
  
  }

  bool Physics_Analyzer::Terminate(){
 
    return 1;
  
  }


}

#endif
