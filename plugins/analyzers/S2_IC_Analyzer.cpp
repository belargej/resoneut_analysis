#include "S2_IC_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "S2_Analyzer.hpp"
#include "IC_Analyzer.hpp"



namespace coinc{


  TCutG* si_ic_time1;
  TCutG* si_ic_time2;
  int si_ic_tcheck(0);
  

  sak::Histogram2D *hpede_ic1;    
  sak::Histogram2D *hpede_ic2;  
  sak::Histogram2D *hpede_ic3;  
  sak::Histogram2D *h_evtheta_ic1[2];
  sak::Histogram2D *h_evtheta_ic2[2];
  sak::Histogram2D *h_evtheta_protgated_ic1[2];
  sak::Histogram2D *h_evtheta_protgated_ic2[2];

  sak::Histogram2D *hpede_ic1_MCP;    
  sak::Histogram2D *hpede_ic2_MCP;  
  sak::Histogram2D *hpede_ic3_MCP;  
  sak::Histogram2D *h_evtheta_ic1_MCP[2];
  sak::Histogram2D *h_evtheta_ic2_MCP[2];
  sak::Histogram2D *h_evtheta_protgated_ic1_MCP[2];
  sak::Histogram2D *h_evtheta_protgated_ic2_MCP[2];

  sak::Hist2D* mcpvs1_t_prot_F17;
  sak::Hist2D* mcpvs2_t_prot_F17;

  sak::Hist2D* si_t_v_ic_t;
  sak::Hist2D* si_t_v_ic_t_F17;
  sak::Hist2D* si_t_v_ic_t_prot;
  sak::Hist2D* si_trel_v_ic_t_prot;
  sak::Hist2D* si_trel_v_ic_t;
  sak::Hist2D* si_trel_v_ic_t_prot_F17;
  sak::Hist2D* si_trel_v_ic_t_prot_O16;
  sak::Hist1D* ic_t_prot;
  sak::Hist1D* ic_t_prot_F17;
  sak::Hist1D* ic_t_prot_O16;

  sak::Hist2D* h_evtheta_prot_F17_timing;
  sak::Hist2D* h_evtheta_prot_O16_timing;
  sak::Hist2D* hpede_F17_timing; 

  S2_IC_Analyzer::S2_IC_Analyzer()
  {
    
  }
  void S2_IC_Analyzer::ResetGlobals(){

  }

  bool S2_IC_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    rootfile->mkdir("coinc/S2_IC");
    rootfile->mkdir("coinc/S2_IC/evtheta");
    rootfile->mkdir("coinc/S2_IC/ede");
    rootfile->mkdir("coinc/S2_IC/timing");
    rootfile->mkdir("coinc/S2_IC/MCP/evtheta/");  
    rootfile->mkdir("coinc/S2_IC/MCP/ede");
    rootfile->mkdir("coinc/S2_IC/MCP/timing");


    rootfile->cd("coinc/S2_IC/timing");
    ic_t_prot=new sak::Hist1D("h_ic_t_prot","t",4096,0,4095);
    ic_t_prot_F17=new sak::Hist1D("h_ic_t_prot_F17","t",4096,0,4095);
    ic_t_prot_O16=new sak::Hist1D("h_ic_t_prot_O16","t",4096,0,4095);
    si_t_v_ic_t=new sak::Hist2D("h_si_t_v_ic_t","sit","ict",1024,0,4095,1024,0,4095);
    si_t_v_ic_t_F17=new sak::Hist2D("h_si_t_v_ic_t_F17","sit","ict",1024,0,4095,1024,0,4095);
    si_t_v_ic_t_prot=new sak::Hist2D("h_si_t_v_ic_t_prot","sit","ict",1024,0,4095,1024,0,4095);
    si_trel_v_ic_t=new sak::Hist2D("h_si_trel_v_ic_t","si_trel","ict",1024,-2047,2048,1024,0,4095);
    si_trel_v_ic_t_prot=new sak::Hist2D("h_si_trel_v_ic_t_prot","si_trel","ict",1024,-2047,2048,1024,0,4095);
    si_trel_v_ic_t_prot_F17=new sak::Hist2D("h_si_trel_v_ic_t_prot_F17","si_trel","ict",1024,-2047,2048,1024,0,4095);
    si_trel_v_ic_t_prot_O16=new sak::Hist2D("h_si_trel_v_ic_t_prot_O16","si_trel","ict",1024,-2047,2048,1024,0,4095);
    h_evtheta_prot_F17_timing=new sak::Hist2D("h_evtheta_prot_F17_timing","theta[deg]","E",256,10,42,128,0,32);
    h_evtheta_prot_O16_timing=new sak::Hist2D("h_evtheta_prot_O16_timing","theta[deg]","E",256,10,42,128,0,32);
    hpede_F17_timing=new sak::Hist2D("hpEdE_prot_F17_timing","E [MeV]","dE [MeV]",64,0,20,64,0,6);

    rootfile->cd("coinc/S2_IC/ede");
    hpede_ic1=new sak::Hist2D("hpEdE_ic1","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic2=new sak::Hist2D("hpEdE_ic2","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic3=new sak::Hist2D("hpEdE_ic3","E [MeV]","dE [MeV]",64,0,20,64,0,6);


    rootfile->cd("coinc/S2_IC/MCP/ede");
    hpede_ic1_MCP=new sak::Hist2D("hpEdE_ic1_MCP","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic2_MCP=new sak::Hist2D("hpEdE_ic2_MCP","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    hpede_ic3_MCP=new sak::Hist2D("hpEdE_ic3_MCP","E [MeV]","dE [MeV]",64,0,20,64,0,6);
    


    for(int i=0;i<2;i++){
      rootfile->cd("coinc/S2_IC/evtheta");
      h_evtheta_ic1[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic1",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_ic2[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic2",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic1[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic1",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic2[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic2",i+1),"theta[deg]","E",256,10,42,128,0,32);
      rootfile->cd("coinc/S2_IC/MCP/evtheta/");
      h_evtheta_ic1_MCP[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic1_MCP",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_ic2_MCP[i]=new sak::Hist2D(Form("h_evtheta[%d]_ic2_MCP",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic1_MCP[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic1_MCP",i+1),"theta[deg]","E",256,10,42,128,0,32);
      h_evtheta_protgated_ic2_MCP[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]_ic2_MCP",i+1),"theta[deg]","E",256,10,42,128,0,32);

    }

    rootfile->cd("coinc/S2_IC/MCP/timing");
    mcpvs1_t_prot_F17=new sak::Hist2D("mcpvs1_t_prot_F17","mcp","s1_t",1024,0,4095,1024,0,4095);
    mcpvs2_t_prot_F17=new sak::Hist2D("mcpvs2_t_prot_F17","mcp","s2_t",1024,0,4095,1024,0,4095);

    return 1;
  }


  bool S2_IC_Analyzer::Process(){
    si_ic_tcheck=0;

    si_ic_tcheck=((si_ic_time1 && si_ic_time2) && (si_ic_time1->IsInside(rftime[0].fT -si_[0].Back_T(0),unpacker::TDC1[1]) || 
						   si_ic_time2->IsInside(rftime[0].fT -si_[0].Back_T(0),unpacker::TDC1[1]))); 
		  
    if(si_cal::protcheck && ionchamber::hi_check[0]){
      mcpvs1_t_prot_F17->Fill(rftime[1].fT,si_[0].Back_T(0));
      mcpvs2_t_prot_F17->Fill(rftime[1].fT,si_[1].Back_T(0)); 
      ic_t_prot_F17->Fill(unpacker::TDC1[1]);
      
    }
    
    if(si_ic_tcheck && ionchamber::hi_check[0])
      hpede_F17_timing->Fill(si_cal::prot_E,si_cal::prot_dE);
    if(si_cal::protcheck&& si_ic_tcheck && ionchamber::hi_check[0])
      h_evtheta_prot_F17_timing->Fill(si_cluster_[1].fPos[0].Theta()*180/3.14,si_cal::prot_E);
    
    if(si_cal::protcheck&& si_ic_tcheck && ionchamber::hi_check[1])
      h_evtheta_prot_O16_timing->Fill(si_cluster_[1].fPos[0].Theta()*180/3.14,si_cal::prot_E);
    


    if(si_cal::protcheck)
      ic_t_prot->Fill(unpacker::TDC1[1]);
    
    if(si_cal::protcheck && ionchamber::hi_check[1])
      ic_t_prot_O16->Fill(unpacker::TDC1[1]);
    
    
    si_t_v_ic_t->Fill(si_[0].Back_T(0),unpacker::TDC1[1]);
    si_trel_v_ic_t->Fill(rftime[0].fT - si_[0].Back_T(0),unpacker::TDC1[1]);
    
    if(ionchamber::hi_check[0])
      si_t_v_ic_t_F17->Fill(si_[0].Back_T(0),unpacker::TDC1[1]);

    if(si_cal::protcheck){
      si_t_v_ic_t_prot->Fill(si_[0].Back_T(0),unpacker::TDC1[1]);
      si_trel_v_ic_t_prot->Fill(rftime[0].fT - si_[0].Back_T(0),unpacker::TDC1[1]);
      if(ionchamber::hi_check[0])
	si_trel_v_ic_t_prot_F17->Fill(rftime[0].fT - si_[0].Back_T(0),unpacker::TDC1[1]);
      if(ionchamber::hi_check[1])
	si_trel_v_ic_t_prot_O16->Fill(rftime[0].fT - si_[0].Back_T(0),unpacker::TDC1[1]);
    }

    if(si_cal::prot_E>0){
      
      if(ionchamber::hi_check[0]){
	hpede_ic1->Fill(si_cal::prot_E,si_cal::prot_dE); //F17
	if(rftime[1].fT>0)
	  hpede_ic1_MCP->Fill(si_cal::prot_E,si_cal::prot_dE); //MCP

	for(int i=0;i<2;i++){
	  if(si_cluster_[i].fMult>0){
	    h_evtheta_ic1[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(rftime[1].fT>0)
	      h_evtheta_ic1_MCP[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(si_cal::protcheck){
	      h_evtheta_protgated_ic1[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	      if(rftime[1].fT>0)
		h_evtheta_protgated_ic1_MCP[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	      
	    }
	  }
	  
	  
	}
      }
      
      if(ionchamber::hi_check[1]){
	hpede_ic2->Fill(si_cal::prot_E,si_cal::prot_dE);
	if(rftime[1].fT>0)
	  hpede_ic2_MCP->Fill(si_cal::prot_E,si_cal::prot_dE); //MCP

	
	for(int i=0;i<2;i++){
	  if(si_cluster_[i].fMult>0){
	    h_evtheta_ic2[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(rftime[1].fT>0)
	      h_evtheta_ic2_MCP[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    if(si_cal::protcheck){
	      h_evtheta_protgated_ic2[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	      if(rftime[1].fT>0)
		h_evtheta_protgated_ic2_MCP[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,si_cal::prot_E);
	    }
	  }
	}
      }
      
      if(ionchamber::hi_check[2]){
	hpede_ic3->Fill(si_cal::prot_E,si_cal::prot_dE);    
	if(rftime[1].fT>0)
	  hpede_ic3_MCP->Fill(si_cal::prot_E,si_cal::prot_dE); //MCP
      }
    
      
    }
    return 1;
  }
  
  
  
  
  bool S2_IC_Analyzer::Terminate(){
    rootfile->Write();
    rootfile->Close();
    return 1;
  }
  
  void S2_IC_Analyzer::Clear(){
    
  }
  


  void Load_Si_IC_Gates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("si_ic_time1") && !si_ic_time1)
      si_ic_time1=new TCutG(*in.getCut("si_ic_time1"));
    if(in.getCut("si_ic_time2") && !si_ic_time2)
      si_ic_time2=new TCutG(*in.getCut("si_ic_time2"));

    in.Close();

  }




}
