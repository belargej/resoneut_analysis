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

namespace coinc{


  sak::Hist2D *h_nai_t_v_neut_t;			 
  sak::Hist2D *h_nai_t_v_neut_t_neutgated;	 
  Double32_t nai_t(0);
  Double32_t neut_t(0);



  NaI_NeutAnalyzer::NaI_NeutAnalyzer()
  {
  
  }

  void NaI_NeutAnalyzer::ResetGlobals(){
    nai_t=0;
    neut_t=0;
  }

  bool NaI_NeutAnalyzer::Begin(){
  
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    rootfile->mkdir("coinc/NaI_Neut/timing");
    rootfile->cd("coinc/NaI_Neut/timing");
    h_nai_t_v_neut_t=new sak::Hist2D("h_nai_t_v_neut_t","nai_t","neut_t",1024,0,4095,1024,0,4095);			 
    h_nai_t_v_neut_t_neutgated=new sak::Hist2D("h_nai_t_v_neut_t_neutgated","nai_t","neut_t",1024,0,4095,1024,0,4095);	   
  
    return 1;
  }

  bool NaI_NeutAnalyzer::Process(){ 

    for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
      if((*it).SumE()>750 && (*it).fT[0] > 0 ){
	nai_t=((*it).T());
	break;
      }
    }
    neut_t=Narray.fT[0];
    

  
    return 1;
  }
  bool NaI_NeutAnalyzer::ProcessFill(){
    
    if(nai_t>0&&neut_t>0){
      h_nai_t_v_neut_t->Fill(nai_t,neut_t);
    }

    return 1;
  }



  bool NaI_NeutAnalyzer::Terminate(){
    return 1;
  }
  bool NaI_NeutAnalyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();
    return 1;
  }

 

} 
		       



