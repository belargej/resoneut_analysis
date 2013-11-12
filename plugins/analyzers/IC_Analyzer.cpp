
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
#include "../../include/RN_Root.hpp"
namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;

  Double32_t IC_TotalE(0);
  Double32_t IC_ELoss(0);

  int hi_check[3];

  sak::Histogram2D *hIC_ede;  
  sak::Hist1D * h_IC_t_F17gated;
  sak::Hist1D * h_IC_t_O16gated;



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
    
 
    rootfile->mkdir("IC/ede");
    rootfile->mkdir("IC/t");
  
    rootfile->cd("IC/ede");
    hIC_ede=new sak::Hist2D("hIC_EdE","E [arb]","dE [arb]",1024,0,4096,1024,0,4096);
    rootfile->cd("IC/t");
    h_IC_t_F17gated=new sak::Hist1D("h_IC_t_F17","t",1024,0,4095);
    h_IC_t_O16gated=new sak::Hist1D("h_IC_t_O16","t",1024,0,4095);

    return 1;
  }

  void IC_Analyzer::ResetGlobals(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
 
    IC_ELoss = 0;
    IC_TotalE = 0;
  }

  bool IC_Analyzer::Process(){
    IC_ELoss = ic.fdE;
    IC_TotalE = ic.fE + IC_ELoss;
     
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(IC_TotalE,IC_ELoss));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(IC_TotalE,IC_ELoss));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(IC_TotalE,IC_ELoss));
    
    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){
    
    hIC_ede->Fill(IC_TotalE,IC_ELoss);
    
    if(hi_check[0]){
      h_IC_t_F17gated->Fill(TDC1[1]);
    }
    if(hi_check[1]){
      h_IC_t_O16gated->Fill(TDC1[1]);


    }


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
  

  void LoadGates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("ede_hi1") && !ede_hi1)
      ede_hi1=new TCutG(*in.getCut("ede_hi1"));
    if(in.getCut("ede_hi2") && !ede_hi2)
      ede_hi2=new TCutG(*in.getCut("ede_hi2"));
    if(in.getCut("ede_hi3") && !ede_hi3)
      ede_hi3=new TCutG(*in.getCut("ede_hi3"));   

  
    in.Close();
  }
  
  void ClearGates(){
    if(ede_hi1)delete ede_hi1;
    if(ede_hi2)delete ede_hi2;
    if(ede_hi3)delete ede_hi3;



  }
}

#endif
