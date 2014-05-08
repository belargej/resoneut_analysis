/***********************************************************/
//Class: Q_ValueAnalyzer
//
//Author:Sean Kuvin
//
//Physics Analyzer class to be inserted in the analyzer list for 
//calculating Q_Value parameters using the detector parameters
/***********************************************************/

#ifndef _QV_ANALYZER_CXX
#define _QV_ANALYZER_CXX

#include "Q_ValueAnalyzer.hpp"
#include "RN_Root.hpp"
#include "RN_Unpack2Root.hpp"
#include "Si_IC_Analyzer.hpp"
#include "Si_Analyzer.hpp"
#include "IC_Analyzer.hpp"

#if 1
#define USEANGLE silicon::prot_theta
#else
#define USEANGLE silicon::rel_angle
#endif

#define Q_MIN 1.2
#define Q_MAX 1.6

namespace physical{

  TH1D *Q_Value;
  TH1D *Q_Value_proton;
  TH1D *Q_Value_protontheta;
  TH1D *Q_Value_protontheta_hi1;
  TH1D *Q_Value_proton_hi1;
  TH1D *Q_Value_proton_hi1_ictime;
  TH1D *Q_Value_proton_hi2;

  //Q v Parameters
  TH2D *Q_v_Phi;	
  TH2D *Q_v_cosTheta;
  TH2D *Q_v_EcosTheta;
  TH2D *Q_v_pTheta;		       
  TH2D *Q_v_pE;			       
  TH2D *Q_v_pRelAngle;		       
	       				       	       
  TH2D *Q_v_pTheta_proton_hi1;	       
  TH2D *Q_v_pE_proton_hi1;	       
  TH2D *Q_v_pRelAngle_proton_hi1;       
	       				       	       
  TH2D *Q_v_pTheta_proton_hi1_ictime;   
  TH2D *Q_v_pE_proton_hi1_ictime;       
  TH2D *Q_v_pRelAngle_proton_hi1_ictime;


  Double32_t q_val_p;
  Double32_t q_val_n;


  Q_ValueAnalyzer::Q_ValueAnalyzer()
  {
  
  }


  bool Q_ValueAnalyzer::Begin(){

    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    //check for parameters necessary for q_value reconstruction
    if(!global::m_beam || !global::m_target || !global::m_recoil || !global::m_frag || !global::m_decay_product || !global::m_heavy_decay){
      std::cout<<"Reaction Masses have not been set"<<std::endl;
      exit(EXIT_FAILURE);
    }
    if(!global::E_fragment)
      std::cout<<"Heavy Ion energy ansatz for Q value reconstruction needed"<<std::endl;


    //if newtree is allocated go ahead and add the branch
    if(newtree){
      newtree->Branch("q_value_p",&q_val_p);
    }

    //create directory structure
    rootfile->mkdir("physical");
    rootfile->cd("physical");
    gDirectory->mkdir("Q");
    gDirectory->mkdir("QvParameters");


    //create histograms  
    rootfile->cd("physical/Q");
    Q_Value=new TH1D("Q_val_p","Q_value_p;Q_value",512,-1,10);
    Q_Value_proton=new TH1D("Q_val_proton","Q_value_proton;Q_value",512,-1,10);
    Q_Value_proton_hi1=new TH1D("Q_val_proton_hi1","Q_val_proton_hi1;Q_value",512,-1,10);
    Q_Value_proton_hi1_ictime=new TH1D("Q_val_proton_hi1_ictime","Q_val_proton_hi1_ictime;Q_value",512,-1,10);
    Q_Value_proton_hi2=new TH1D("Q_val_proton_hi2","Q_val_proton_hi2;Q_value",512,-1,10);


    rootfile->cd("physical/QvParameters");
    Q_v_Phi=new TH2D("Q_v_Phi","Q_v_Phi;Q;Phi",512,-1,10,180,-180,180);
    Q_v_cosTheta=new TH2D("Q_v_cosTheta","Q_v_cosTheta;Q;cosTheta",512,-1,10,256,0.5,1);
    Q_v_EcosTheta=new TH2D("Q_v_EcosTheta","Q_v_EcosTheta;Q;EcosTheta",512,-1,10,512,0,20);
    Q_v_pTheta=new TH2D("Q_v_pTheta","Q_v_pTheta;Q;Theta",512,-1,10,180,0,179);
    Q_v_pE=new TH2D("Q_v_pE","Q_v_pE;Q;E",512,-1,10,512,0,32);
    Q_v_pRelAngle=new TH2D("Q_v_pRelAngle","Q_v_pRelAngle;Q;RelAngle",512,-1,10,180,0,179);;
    
    Q_v_pTheta_proton_hi1=new TH2D("Q_v_pTheta_proton_hi1","Q_v_pTheta_proton_hi1;Q;Theta",512,-1,10,180,0,179);;
    Q_v_pE_proton_hi1=new TH2D("Q_v_pE_proton_hi1","Q_v_pE_proton_hi1;Q;E",512,-1,10,512,0,32);;
    Q_v_pRelAngle_proton_hi1=new TH2D("Q_v_pRelAngle_proton_hi1","Q_v_pRelAngle_proton_hi1;Q;RelAngle",512,-1,10,180,0,179);

    Q_v_pTheta_proton_hi1_ictime=new TH2D("Q_v_pTheta_proton_hi1_ictime","Q_v_pTheta_proton_hi1_ictime;Q;Theta",512,0,32,180,0,179);
    Q_v_pE_proton_hi1_ictime=new TH2D("Q_v_pE_proton_hi1_ictime","Q_v_pE_proton_hi1_ictime;Q;E",512,-1,10,512,0,32);
    Q_v_pRelAngle_proton_hi1_ictime=new TH2D("Q_v_pRelAngle_proton_hi_ictime","Q_v_pRelAngle_proton_hi_ictime;Q;RelAngle",512,-1,10,180,0,179);;
    



    rootfile->cd();
    return 1;

 
  }

  void Q_ValueAnalyzer::ResetGlobals(){
    q_val_p=0;
    q_val_n=0;
  }

  bool Q_ValueAnalyzer::Process(){
    //global::E_fragment is ansatz from MC simulation
  
    if(silicon::prot_E>0&& silicon::prot_theta!=0){
    
      q_val_p = (silicon::prot_E * (1.0 + (global::m_decay_product / global::m_heavy_decay)) 
		 - (global::E_fragment * ( 1.0 - (global::m_frag / global::m_heavy_decay)))
		 - ((2.0 / global::m_heavy_decay) * TMath::Sqrt(silicon::prot_E * global::m_decay_product * global::E_fragment * global::m_frag) * cos(TMath::Pi()*USEANGLE / 180)));
    
    }

    // if(q_val_p<Q_MIN||q_val_p>Q_MAX)
    // return 0;

    return 1;
  }
  
  bool Q_ValueAnalyzer::ProcessFill(){

    //Q_Value parameter histograms
    Q_Value->Fill(q_val_p);
    Q_v_Phi->Fill(q_val_p,si_cluster_[0].fPos[0].Phi()*180/TMath::Pi());
    Q_v_cosTheta->Fill(q_val_p,cos(TMath::Pi()*silicon::rel_angle/180));
    Q_v_EcosTheta->Fill(q_val_p,TMath::Sqrt(silicon::prot_E)*cos(TMath::Pi()*silicon::rel_angle/180));

    if(silicon::protcheck){
      Q_Value_proton->Fill(q_val_p);
      Q_v_pTheta->Fill(q_val_p,silicon::prot_theta);   		       
      Q_v_pE->Fill(q_val_p,silicon::prot_E);   			       
      Q_v_pRelAngle->Fill(q_val_p,silicon::rel_angle);   		       
      
      if(ionchamber::hi_check[0]){
	Q_Value_proton_hi1->Fill(q_val_p);
	Q_v_pTheta_proton_hi1->Fill(q_val_p,silicon::prot_theta);   	       
	Q_v_pE_proton_hi1->Fill(q_val_p,silicon::prot_E);   	       
	Q_v_pRelAngle_proton_hi1->Fill(q_val_p,silicon::rel_angle);   
	
	if(coinc::si_ic_tcheck){
	  Q_Value_proton_hi1_ictime->Fill(q_val_p);
	  Q_v_pTheta_proton_hi1_ictime->Fill(q_val_p,silicon::prot_theta);   
	  Q_v_pE_proton_hi1_ictime->Fill(q_val_p,silicon::prot_E);          
	  Q_v_pRelAngle_proton_hi1_ictime->Fill(q_val_p,silicon::rel_angle);   
    
    
	}
	
      }
      if(ionchamber::hi_check[1]){
	Q_Value_proton_hi2->Fill(q_val_p);
      }

    }


		       
    





    return 1;

  }
  


  bool Q_ValueAnalyzer::TerminateIfLast(){
    rootfile->Write();
    rootfile->Close();

    return 1;
  
  }

  bool Q_ValueAnalyzer::Terminate(){
 
    return 1;
  
  }


}

#endif
