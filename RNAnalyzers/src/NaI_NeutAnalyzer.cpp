/***************************************************************/
//Class: NaI_Neut_Analyzer
//
//Author:Sean Kuvin
//
//NaI_Neut_Analyzer is added to the analyzer list just as NaI_Analyzer
//is except now we are looking at Neut and NaI.  The results of sorting through
//the Neut_analyzer is necessary for proper function
//of this class. Therefore, Neut_Analyzer must be 
//added to the analyzer list and must be added before this analyzer
//is added
/********************************************************************/


#include "NaI_NeutAnalyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace coinc{


  TH2D *h_nai_t_v_neut_t;			 
  TH2D *h_nai_t_v_neut_t_neutgated;	 

  NaI_NeutAnalyzer::NaI_NeutAnalyzer()
  {
  
  }

  void NaI_NeutAnalyzer::Reset(){

  }

  bool NaI_NeutAnalyzer::Begin(){
  
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    //make directory structure
    if(!fgRootFile->GetDirectory("coinc"))
      fgRootFile->mkdir("coinc");
    fgRootFile->cd("coinc");
    gDirectory->mkdir("NaI_Neut");
   
    //create histrograms
    fgRootFile->cd("coinc/NaI_Neut");
    h_nai_t_v_neut_t=new TH2D("h_nai_t_v_neut_t","nai_t_v_neut_;nai_t;neut_t",1024,0,4095,1024,0,4095);			 
    h_nai_t_v_neut_t_neutgated=new TH2D("h_nai_t_v_neut_t_neutgated","nai_t_v_neut_t_neutgated;nai_t;neut_t",1024,0,4095,1024,0,4095);	   
  
    return 1;
  }

  bool NaI_NeutAnalyzer::Process(){ 
    return 1;
  }
  bool NaI_NeutAnalyzer::ProcessFill(){
    
    if(nai_array.TRaw(0)>0 && Narray.fT_mult>0){
      h_nai_t_v_neut_t->Fill(nai_array.TRaw(0), Narray.fT_first);
    }

    return 1;
  }



  bool NaI_NeutAnalyzer::Terminate(){
    return 1;
  }
  bool NaI_NeutAnalyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    return 1;
  }

 

} 
		       



