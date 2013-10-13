#include "S2_IC_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "S2_Analyzer.hpp"
#include "IC_Analyzer.hpp"



namespace coinc{


  sak::Histogram2D *hpede_ic1;    
  sak::Histogram2D *hpede_ic2;  
  sak::Histogram2D *hpede_ic3;  
  sak::Histogram2D *h_evtheta_ic1[2];
  sak::Histogram2D *h_evtheta_ic2[2];
  sak::Histogram2D *h_evtheta_protgated_ic1[2];
  sak::Histogram2D *h_evtheta_protgated_ic2[2];


  S2_IC_Analyzer::S2_IC_Analyzer()
  {
    
  }

  void S2_IC_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    rootfile->mkdir("coinc/S2_IC");
    rootfile->mkdir("coinc/S2_IC/evtheta");
    rootfile->mkdir("coinc/S2_IC/ede");
  

    rootfile->cd("coinc/S2_IC/ede");
    hpede_ic1=new sak::Hist2D("hpEdE_ic1","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic2=new sak::Hist2D("hpEdE_ic2","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic3=new sak::Hist2D("hpEdE_ic3","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    
    for(int i=0;i<2;i++){
      rootfile->cd("coinc/S2_IC/evtheta");
      h_evtheta_ic1[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic1",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_ic2[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic2",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic1[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic1",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic2[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic2",i+1),"theta[deg]","E",256,10,42,128,0,32);
    }
  }


  void S2_IC_Analyzer::Process(){
    if(si_cal::prot_E>0){
      
      if(ionchamber::hi_check[0]){
	hpede_ic1->Fill(si_cal::prot_E,si_cal::prot_dE);
	
	for(int i=0;i<2;i++){
	  if(si_cluster_[i].fMult>0){
	    h_evtheta_ic1[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(si_cal::protcheck){
	      h_evtheta_protgated_ic1[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	      
	    }
	  }
	  
	  
	}
      }
      
      if(ionchamber::hi_check[1]){
	hpede_ic2->Fill(si_cal::prot_E,si_cal::prot_dE);
	
	
	for(int i=0;i<2;i++){
	  if(si_cluster_[i].fMult>0){
	    h_evtheta_ic2[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(si_cal::protcheck){
	      h_evtheta_protgated_ic2[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    
	    }
	  }
	}
      }
      
      if(ionchamber::hi_check[2]){
	hpede_ic3->Fill(si_cal::prot_E,si_cal::prot_dE);    
      }
    
      
    }
  }
  
  
  
  
  void S2_IC_Analyzer::Terminate(){
    rootfile->Write();
    rootfile->Close();
    
  }
  
  void S2_IC_Analyzer::Clear(){
    
  }
  

}
