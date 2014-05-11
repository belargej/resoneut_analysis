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

#include "Si_IC_Analyzer.hpp"
#include "Si_Analyzer.hpp"
#include "IC_Analyzer.hpp"

using namespace RNROOT;

#if 1
#define USEANGLE si_array.Theta_A()
#else
#define USEANGLE si_array.Theta_B()
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

    if(!RNROOT::gRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    //check for parameters necessary for q_value reconstruction
    if(!gReactionInfo.IsSet()){
      std::cout<<"Reaction Masses have not been set"<<std::endl;
      exit(EXIT_FAILURE);
    }
    if(!gReactionInfo.E_Fragment())
      std::cout<<"Heavy Ion energy ansatz for Q value reconstruction needed"<<std::endl;


    //if RNROOT::gNewTree is allocated go ahead and add the branch
    if(RNROOT::gNewTree){
      RNROOT::gNewTree->Branch("q_value_p",&q_val_p);
    }

    //create directory structure
    RNROOT::gRootFile->mkdir("physical");
    RNROOT::gRootFile->cd("physical");
    gDirectory->mkdir("Q");
    gDirectory->mkdir("QvParameters");


    //create histograms  
    RNROOT::gRootFile->cd("physical/Q");
    Q_Value=new TH1D("Q_val_p","Q_value_p;Q_value",512,-1,10);
    Q_Value_proton=new TH1D("Q_val_proton","Q_value_proton;Q_value",512,-1,10);
    Q_Value_proton_hi1=new TH1D("Q_val_proton_hi1","Q_val_proton_hi1;Q_value",512,-1,10);
    Q_Value_proton_hi1_ictime=new TH1D("Q_val_proton_hi1_ictime","Q_val_proton_hi1_ictime;Q_value",512,-1,10);
    Q_Value_proton_hi2=new TH1D("Q_val_proton_hi2","Q_val_proton_hi2;Q_value",512,-1,10);


    RNROOT::gRootFile->cd("physical/QvParameters");
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
    



    RNROOT::gRootFile->cd();
    return 1;

 
  }

  void Q_ValueAnalyzer::ResetGlobals(){
    q_val_p=0;
    q_val_n=0;
  }

  bool Q_ValueAnalyzer::Process(){
    //gPrimaryReaction.E_fragment is ansatz from MC simulation
  
    if(si_array.E_AB()>0&& USEANGLE!=0){
    
      q_val_p = gReactionInfo.DecayQValueEstimate(si_array.E_AB(),(TMath::Pi()*USEANGLE / 180));
    
    }

    // if(q_val_p<Q_MIN||q_val_p>Q_MAX)
    // return 0;

    return 1;
  }
  
  bool Q_ValueAnalyzer::ProcessFill(){

    //Q_Value parameter histograms
    Q_Value->Fill(q_val_p);
    Q_v_Phi->Fill(q_val_p,si_cluster_[0].fPos[0].Phi()*180/TMath::Pi());
    Q_v_cosTheta->Fill(q_val_p,cos(si_array.Theta_A()));
    Q_v_EcosTheta->Fill(q_val_p,TMath::Sqrt(si_array.E_AB())*cos(si_array.Theta_A()));

    if(silicon::protcheck){
      Q_Value_proton->Fill(q_val_p);
      Q_v_pTheta->Fill(q_val_p,si_array.Theta_A());   		       
      Q_v_pE->Fill(q_val_p,si_array.E_AB());   			       
      Q_v_pRelAngle->Fill(q_val_p,si_array.Theta_A());   		       
      
      if(ionchamber::hi_check[0]){
	Q_Value_proton_hi1->Fill(q_val_p);
	Q_v_pTheta_proton_hi1->Fill(q_val_p,si_array.Theta_A());   	       
	Q_v_pE_proton_hi1->Fill(q_val_p,si_array.E_AB());   	       
	Q_v_pRelAngle_proton_hi1->Fill(q_val_p,si_array.Theta_A());   
	
	if(coinc::si_ic_tcheck){
	  Q_Value_proton_hi1_ictime->Fill(q_val_p);
	  Q_v_pTheta_proton_hi1_ictime->Fill(q_val_p,si_array.Theta_A());   
	  Q_v_pE_proton_hi1_ictime->Fill(q_val_p,si_array.E_AB());          
	  Q_v_pRelAngle_proton_hi1_ictime->Fill(q_val_p,si_array.Theta_A());   
	  
    
	}
	
      }
      if(ionchamber::hi_check[1]){
	Q_Value_proton_hi2->Fill(q_val_p);
      }

    }


		       
    





    return 1;

  }
  


  bool Q_ValueAnalyzer::TerminateIfLast(){
    RNROOT::gRootFile->Write();
    RNROOT::gRootFile->Close();

    return 1;
  
  }

  bool Q_ValueAnalyzer::Terminate(){
 
    return 1;
  
  }


}

#endif
