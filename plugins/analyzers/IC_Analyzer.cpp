#include "IC_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;

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

bool IC_Analyzer::Process(){
  for(int i=0;i<3;i++){
    hi_check[i]=0;
  }
  
  hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.fE,ic.fdE));
  hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.fE,ic.fdE));
  hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.fE,ic.fdE));

  hIC_ede->Fill(ic.fE,ic.fdE);
  
  if(hi_check[0]){
    h_IC_t_F17gated->Fill(TDC1[1]);
  }
  if(hi_check[1]){
    h_IC_t_O16gated->Fill(TDC1[1]);


  }


  return 1;
}


  bool IC_Analyzer::Terminate(){
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
