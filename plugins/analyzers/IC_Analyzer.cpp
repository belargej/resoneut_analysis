#include "IC_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;

  int hi_check[3];

  sak::Histogram2D *hIC_ede;  

  IC_Analyzer::IC_Analyzer()
  {
    
  }  
  void IC_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
    exit(EXIT_FAILURE);
    }
    
 
  rootfile->mkdir("IC/ede");
  
  rootfile->cd("IC/ede");
   hIC_ede=new sak::Hist2D("hIC_EdE","E [arb]","dE [arb]",1024,0,4096,1024,0,4096);
  
  
  }

void IC_Analyzer::Process(){
  for(int i=0;i<3;i++){
    hi_check[i]=0;
  }
  
  hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.fE,ic.fdE));
  hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.fE,ic.fdE));
  hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.fE,ic.fdE));

  hIC_ede->Fill(ic.fE,ic.fdE);
    
}


  void IC_Analyzer::Terminate(){
    rootfile->Write();
    rootfile->Close();
    
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
