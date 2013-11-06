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

  return 1;

  Q_Value_proton=new sak::Hist1D("Q_val_proton","Q_value",120,-10,10);
  Q_Value_proton_F17=new sak::Hist1D("Q_val_proton_F17","Q_value",120,-10,10);
  Q_Value_proton_O16=new sak::Hist1D("Q_val_proton_O16","Q_value",120,-10,10);

 
}

void Physics_Analyzer::ResetGlobals(){
  q_val_p=0;

}

bool Physics_Analyzer::Process(){
  double E_recoil= 86;
  double m_ex = global::m_frag + global::hi_ex_set;
  q_val_p = si_cal::prot_E * (1 + (global::m_decay_product / global::m_heavy_decay)) 
    - E_recoil * ( 1 - (m_ex / global::m_decay_product))
    - 2/global::m_heavy_decay * TMath::Sqrt(si_cal::prot_E*global::m_decay_product*E_recoil*m_ex)*cos(si_cal::prot_theta);
  
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
  


bool Physics_Analyzer::Terminate(){
  return 1;
  
}


}
