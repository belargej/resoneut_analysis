/****************************************************
//Class: BeamPathLocator
//
//Author: Joe Belarge
//
// This class is used to test the si_a/b.x/ypos variables.
// You can set various x,y pairs for si_a and si_b,
// then things will be plotted so you can tell if these
// are good or not.

**************************************************/
#include "../../include/RN_Root.hpp"
#include "BeamPathLocator.hpp"
/*
  #include any other analyzers' .hpp files to use the parameters of that analyzer that have been declared extern
  when you add this analyzer to the analyzer list make sure to add the above analyzers to the analyzer list as well.  Since this class will rely on the calculations performed in those analyzers, it is necessary that they are added to the analyzer list BEFORE this one.
 */

using namespace RNROOT;

namespace BeamPathLocator{

  // Declaring Histograms:
  TH2D *ThetaA_vs_ThetaB;
  TH2D *Q_vs_Phi_A;
  TH2D *Q_vs_Theta_A;
  TH2D *Q_vs_EnTot_A;
  TH2D *Q_vs_CosTheta_A;
  TH2D *Q_vs_EnCosTheta_A;
  TH2D *Q_vs_Phi_B;
  TH2D *Q_vs_Theta_B;
  TH2D *Q_vs_EnTot_B;
  TH2D *Q_vs_CosTheta_B;
  TH2D *Q_vs_EnCosTheta_B;

  // Declare TLine:
  TLine *X_equals_Y;
  

////////////////////////////////////////////////
/*
  Declare any root objects (histograms, cuts, etc) that
are going to be used in this analysis,
also any global parameters.  If these parameters are wanted in follow up analysis scripts make sure to declare these parameters as extern in the .hpp file
 */

/////////////////////////////////////////////////

  BeamPathLocator::BeamPathLocator(int number){
    
    NumberPrefix = number;
  }


  void BeamPathLocator::Reset()(){
    
  }

  void SetSiA_XY_Pos(Double32_t xpos,Double32_t ypos){
    // I dont think this is necessary.  When SetCalibrations is called 
    // these values are stored in tempx,y then sent to the vector.
    //si_a.xpos = xpos;
    //si_a.ypos = ypos;
    si_a.SetPosVectX(xpos);
    si_a.SetPosVectY(ypos);
  } 

  void SetSiB_XY_Pos(Double32_t xpos,Double32_t ypos){
    si_b.SetPosVectX(xpos);
    si_b.SetPosVectY(ypos);
  }
  
  bool BeamPathLocator::Begin(){

  
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
  
    // check to see the reaction info is set:
    if(!gReactionInfo.IsSet()){
      std::cout << "> Reaction masses have not been set" << std::endl;
      exit(EXIT_FAILURE);
    }

    if(!gReactionInfo.E_Fragment()){
      std::cout << "> Heavy ion energy for QVal reconstruction needed. " << std::endl;
      exit(EXIT_FAILURE);
    }


    // create directory:
    fgRootFile->mkdir(Form("BeamPathLocator_%d",NumberPrefix));
    fgRootFile->cd(Form("BeamPathLocator_%d",NumberPrefix));

    // create histograms:
    ThetaA_vs_ThetaB = new TH2D("ThetaA_vs_ThetaB","ThetaA_vs_ThetaB",100,0,50,100,0,50);
    Q_vs_Phi_A = new TH2D("Q_vs_Phi_A","Q_vs_Phi_A",110,-1,10,360,0,360);
    Q_vs_Theta_A = new TH2D("Q_vs_Theta_A","Q_vs_Theta_A",110,-1,10,100,0,50);
    Q_vs_EnTot_A = new TH2D("Q_vs_EnTot_A","Q_vs_En_Tot_A",110,-1,10,1000,0,600);
    Q_vs_CosTheta_A = new TH2D("Q_vs_CosTheta_A","Q_vs_CosTheta_A",110,-1,10,100,0,1);
    Q_vs_EnCosTheta_A = new TH2D("Q_vs_EnCosTheta_A","Q_vs_EnCosTheta_A",110,-1,10,1000,0,12);
    Q_vs_Phi_B = new TH2D("Q_vs_Phi_B","Q_vs_Phi_B",110,-1,10,360,0,360);
    Q_vs_Theta_B = new TH2D("Q_vs_Theta_B","Q_vs_Theta_B",110,-1,10,100,0,50);
    Q_vs_EnTot_B = new TH2D("Q_vs_EnTot_B","Q_vs_En_Tot_B",110,-1,10,1000,0,600);
    Q_vs_CosTheta_B = new TH2D("Q_vs_CosTheta_B","Q_vs_CosTheta_B",110,-1,10,100,0,1);
    Q_vs_EnCosTheta_B = new TH2D("Q_vs_EnCosTheta_B","Q_vs_EnCosTheta_B",110,-1,10,1000,0,12);
  
    X_equals_Y = new TLine(0,0,50,50);

    fgRootFile->cd();
    return 1;
 
  }

bool BeamPathLocator::Process(){
  //perform calculations for this analysis' parameters

  /*check any Gates and save the results of those checks
    doing so will speed up the analysis since you won't have to check the result of your gate repeatedly
   */
  
  /*
    The return for this process gives you the 
    control over the filtering process for the 
    cumulative analysis.  If you impose some 
    requirement on the parameters here or the
    gate result and the requirement fails, then you 
    can abort the analysis of that entry.  Doing
    so will skip the ProcessFill() for ALL analyzers 
    in the list.
    this is done by returning a 0,kFalse for a 
    requirement that fails.

   */
  //if(0)
  //return 0;
  
  return 1;
}

bool BeamPathLocator::ProcessFill(){

  /*if your analyzer list has passed the Process() 
portion then it will loop over all ProcessFill()s.  
This is where you should fill your histograms.
  */
  return 1;
}
  


bool BeamPathLocator::Terminate(){
  /*
    any memory cleanup unique to this analysis
    that NEEDS to happen
    

   */
  return 1;
  
}

bool BeamPathLocator::TerminateIfLast(){
  /*
    any cleanup(such as writing to the fgRootFile) that 
    should be reserved to the last analyzer in the list
   
    serious memory management decisions should be performed in the Terminate() function instead of this one.  This function will NOT be called if it is not the last analyzer in the list
   */

  return 1;
  
}

  /*
void LoadTemplateGates(){

    not a member of the BeamPathLocator class, this function serves to Load any gates that are unique to this analysis. The gates should be declared at the top of this file.

   */

}
