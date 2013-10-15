#include "NaI_NeutAnalyzer.hpp"
#include "PSD_Analyzer.hpp"
#include "IC_Analyzer.hpp"
#include "S2_Analyzer.hpp"


sak::Hist2D *h_nai_t_v_neut_t;			 
sak::Hist2D *h_nai_t_v_neut_t_neutgated;	 
sak::Hist2D *h_nai_t_v_neut_t_neut_HI_gated;	 
sak::Hist2D *h_nai_t_v_neut_t_neut_HI_MCP_gated;
sak::Hist2D *h_nai_t_v_neut_t_neut_HI_MCP__S1_gated;
sak::Hist2D *h_nai_t_v_neut_t_prot;			 
sak::Hist2D *h_nai_t_v_neut_t_neut_prot_gated;	 
sak::Hist2D *h_nai_t_v_neut_t_neut_HI_prot_gated;	 
sak::Hist2D *h_nai_t_v_neut_t_neut_HI_MCP_prot_gated;


NaI_NeutAnalyzer::NaI_NeutAnalyzer()
{
  
}

void NaI_NeutAnalyzer::ResetGlobals(){

}

void NaI_NeutAnalyzer::Begin(){
  
  if(!rootfile){
    std::cout<<"output file has not been created"<<std::endl;
    Clear();
    exit(EXIT_FAILURE);
  }
  rootfile->mkdir("coinc/NaI_Neut/timing");
  rootfile->cd("coinc/NaI_Neut/timing");
  h_nai_t_v_neut_t=new sak::Hist2D("h_nai_t_v_neut_t","nai_t","neut_t",1024,0,4095,1024,0,4095);			 
  h_nai_t_v_neut_t_neutgated=new sak::Hist2D("h_nai_t_v_neut_t_neutgated","nai_t","neut_t",1024,0,4095,1024,0,4095);	   
  h_nai_t_v_neut_t_neut_HI_gated=new sak::Hist2D("h_nai_t_v_neut_t_neutHIgated","nai_t","neut_t",1024,0,4095,1024,0,4095);
  h_nai_t_v_neut_t_neut_HI_MCP_gated=new sak::Hist2D("h_nai_t_v_neut_t_neutHIMCPgated","nai_t","neut_t",1024,0,4095,1024,0,4095);

  rootfile->mkdir("coinc/NaI_Neut_SI/timing");
  rootfile->cd("coinc/NaI_Neut_SI/timing");
  h_nai_t_v_neut_t_prot=new sak::Hist2D("h_nai_t_v_neut_t_prot","nai_t","neut_t",1024,0,4095,1024,0,4095);			 
  h_nai_t_v_neut_t_neut_prot_gated=new sak::Hist2D("h_nai_t_v_neut_t_neut_prot_gated","nai_t","neut_t",1024,0,4095,1024,0,4095);	   
  h_nai_t_v_neut_t_neut_HI_prot_gated=new sak::Hist2D("h_nai_t_v_neut_t_neutHI_prot_gated","nai_t","neut_t",1024,0,4095,1024,0,4095);
  h_nai_t_v_neut_t_neut_HI_MCP_prot_gated=new sak::Hist2D("h_nai_t_v_neut_t_neutHIMCP_prot_gated","nai_t","neut_t",1024,0,4095,1024,0,4095);


}
void NaI_NeutAnalyzer::Process(){ 
  double nai_t(0);
  double neut_t(0);
  if(!Narray.fMult>0)
    return;
  
  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    if((*it).SumE()>750 && (*it).fT[0] > 0 ){
      nai_t=((*it).T(0));
      break;
    }
  }
  neut_t=Narray.fT[0];
  
  if(nai_t>0&&neut_t>0){
    h_nai_t_v_neut_t->Fill(nai_t,neut_t);
    if(si_cal::protcheck){
      h_nai_t_v_neut_t_prot->Fill(nai_t,neut_t);
    }
    if(psd::rawneut_orcheck){
      if(si_cal::protcheck){
	h_nai_t_v_neut_t_neut_prot_gated->Fill(nai_t,neut_t);
      }	
      h_nai_t_v_neut_t_neutgated->Fill(nai_t,neut_t);
      
      if(ionchamber::hi_check[2]){
	if(si_cal::protcheck){
	  h_nai_t_v_neut_t_neut_HI_prot_gated->Fill(nai_t,neut_t);
	}
	h_nai_t_v_neut_t_neut_HI_gated->Fill(nai_t,neut_t);
	if(rftime[1].fT>0){
	  h_nai_t_v_neut_t_neut_HI_MCP_gated->Fill(nai_t,neut_t);
	  if(si_cal::protcheck){
	    h_nai_t_v_neut_t_neut_HI_MCP_prot_gated->Fill(nai_t,neut_t);
	  }
	}
      }
    }
  }


}


 void NaI_NeutAnalyzer::Terminate(){
  rootfile->Write();
  rootfile->Close();
  
}

  
		       



