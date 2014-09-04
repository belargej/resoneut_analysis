/***********************************************************/
//Class: NamedTree_Analyzer
//
//Author:Joe Belarge
//
// NamedTree_Analyzer class is to be inserted in the analyzer
// list for producing a next level tree in your root file. 
// It will be made of data that has been sorted out of the
// ADC,QDC,TDC format and into the SiA,SiB,Neut0... format.
// It can be added as another tree to a root file, and it will
// have a base structure similar to the NewTree_Analyzer class,
// so that a new set of analyzers can be designed to be used
// on this tree, and can then be used as wished.
/***********************************************************/

#ifndef _NAMEDTREEANALYZER_CXX
#define _NAMEDTREEANALYZER_CXX

#include "NamedTree_Analyzer.hpp"
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"


NamedTree_Analyzer::NamedTree_Analyzer()
{

}

bool NamedTree_Analyzer::Begin(){
  
  // If no new root file has been declared to be written, exit.
  if(!fgRootFile){
    std::cout << "> Root output file has not been created." << std::endl;
    exit(EXIT_FAILURE);
  }

  Energy_A = -100.0;
  Energy_B = -100.0;
  Energy_AB = -100.0;
  FrontChan_A = -100.0;
  FrontChan_B = -100.0;
  BackChan_A = -100.0;
  BackChan_B = -100.0;
  TotEn_IC = -100.0;
  dE_IC = -100.0;
  ThetaLab_A = -100.0;
  ThetaLab_B = -100.0;
  QVal_Theta_A = -100.0;
  QVal_Theta_B = -100.0;
  NeutLong_0= -100;
  NeutLong_1= -100;
  NeutLong_2= -100;
  NeutLong_3= -100;
  NeutLong_4= -100;
  NeutLong_5= -100;
  NeutLong_6= -100;
  NeutLong_7= -100;
  NeutLong_8= -100;
  NeutLong_9= -100;
  NeutShort_0= -100;
  NeutShort_1= -100;
  NeutShort_2= -100;
  NeutShort_3= -100;
  NeutShort_4= -100;
  NeutShort_5= -100;
  NeutShort_6= -100;
  NeutShort_7= -100;
  NeutShort_8= -100;
  NeutShort_9= -100;
  
  // Otherwise, proceed:
  fgRootFile->cd();
  fNamedTree = new TTree("NamedTree","NamedTree");

  // Add branches here:
  fNamedTree->Branch("Energy_A",&Energy_A,"Energy_A/D");
  fNamedTree->Branch("Energy_B",&Energy_B,"Energy_B/D");
  fNamedTree->Branch("Energy_AB",&Energy_AB,"Energy_AB/D");
  fNamedTree->Branch("A_X",&A_X,"A_X/D");
  fNamedTree->Branch("A_Y",&A_Y,"A_Y/D");
  fNamedTree->Branch("B_X",&B_X,"B_X/D");
  fNamedTree->Branch("B_Y",&B_Y,"B_Y/D");
  fNamedTree->Branch("FrontChan_A",&FrontChan_A,"FrontChan_A/D");
  fNamedTree->Branch("FrontChan_B_",&FrontChan_B,"FrontChan_B/D");
  fNamedTree->Branch("BackChan_A",&BackChan_A,"BackChan_A/D");
  fNamedTree->Branch("BackChan_B",&BackChan_B,"BackChan_B/D");
  fNamedTree->Branch("TotEn_IC",&TotEn_IC,"TotEn_IC/D");
  fNamedTree->Branch("dE_IC",&dE_IC,"dE_IC/D");
  fNamedTree->Branch("IC_X_Raw",&IC_X_Raw,"IC_X_Raw/D");
  fNamedTree->Branch("IC_Y_Raw",&IC_Y_Raw,"IC_Y_Raw/D");
  fNamedTree->Branch("IC_X",&IC_X,"IC_X/D");
  fNamedTree->Branch("IC_Y",&IC_Y,"IC_Y/D");
  fNamedTree->Branch("ThetaLab_A",&ThetaLab_A,"ThetaLab_A/D");
  fNamedTree->Branch("ThetaLab_B",&ThetaLab_B,"ThetaLab_B/D");
  fNamedTree->Branch("QVal_Theta_A",&QVal_Theta_A,"QVal_Theta_A/D");
  fNamedTree->Branch("QVal_Theta_B",&QVal_Theta_B,"QVal_Theta_B/D");
  fNamedTree->Branch("NeutLong_0",&NeutLong_0,"NeutLong_0/D");
  fNamedTree->Branch("NeutLong_1",&NeutLong_1,"NeutLong_1/D");
  fNamedTree->Branch("NeutLong_2",&NeutLong_2,"NeutLong_2/D");
  fNamedTree->Branch("NeutLong_3",&NeutLong_3,"NeutLong_3/D");
  fNamedTree->Branch("NeutLong_4",&NeutLong_4,"NeutLong_4/D");
  fNamedTree->Branch("NeutLong_5",&NeutLong_5,"NeutLong_5/D");
  fNamedTree->Branch("NeutLong_6",&NeutLong_6,"NeutLong_6/D");
  fNamedTree->Branch("NeutLong_7",&NeutLong_7,"NeutLong_7/D");
  fNamedTree->Branch("NeutLong_8",&NeutLong_8,"NeutLong_8/D");
  fNamedTree->Branch("NeutLong_9",&NeutLong_9,"NeutLong_9/D");
  fNamedTree->Branch("NeutShort_0",&NeutShort_0,"NeutShort_0/D");
  fNamedTree->Branch("NeutShort_1",&NeutShort_1,"NeutShort_1/D");
  fNamedTree->Branch("NeutShort_2",&NeutShort_2,"NeutShort_2/D");
  fNamedTree->Branch("NeutShort_3",&NeutShort_3,"NeutShort_3/D");
  fNamedTree->Branch("NeutShort_4",&NeutShort_4,"NeutShort_4/D");
  fNamedTree->Branch("NeutShort_5",&NeutShort_5,"NeutShort_5/D");
  fNamedTree->Branch("NeutShort_6",&NeutShort_6,"NeutShort_6/D");
  fNamedTree->Branch("NeutShort_7",&NeutShort_7,"NeutShort_7/D");
  fNamedTree->Branch("NeutShort_8",&NeutShort_8,"NeutShort_8/D");
  fNamedTree->Branch("NeutShort_9",&NeutShort_9,"NeutShort_9/D");
  

  ZeroCounter = 0;
  FillCounter = 0;
  
  std::cout << "> This is the NamedTree_Analyzer at work." << std::endl;
  std::cout << "> The Begin() portion of this is complete." << std::endl;

  return 1;
}

bool NamedTree_Analyzer::Process(){
  /*  std::cout << ">---------------------------" << std::endl;
  std::cout << "> The s2_A energy  : " << RNROOT::si_array.E_A() << std::endl;
  std::cout << "> s2_A back Channel: " << RNROOT::si_[0].back.Ch(0) << std::endl;
  std::cout << "> s2_A fr Channel  : " << RNROOT::si_[0].front.Ch(0) << std::endl;
  std::cout << "> The s2_B energy  : " << RNROOT::si_array.E_B() << std::endl;
  std::cout << "> The Sum energy   : " << RNROOT::si_array.E_AB() << std::endl;*/
  Energy_A = RNROOT::si_array.E_A();
  Energy_B = RNROOT::si_array.E_B();
  Energy_AB = RNROOT::si_array.E_AB();
  A_X = RNROOT::si_cluster_[0].fPos[0].X();
  A_Y = RNROOT::si_cluster_[0].fPos[0].Y();
  B_X = RNROOT::si_cluster_[1].fPos[0].X();
  B_Y = RNROOT::si_cluster_[1].fPos[0].Y();
  FrontChan_A = RNROOT::si_[0].front.Ch(0);
  FrontChan_B = RNROOT::si_[1].front.Ch(0);
  BackChan_A = RNROOT::si_[0].back.Ch(0);
  BackChan_B = RNROOT::si_[1].back.Ch(0);
  TotEn_IC = RNROOT::ic.ERaw() + RNROOT::ic.DERaw();
  dE_IC = RNROOT:: ic.DERaw();
  IC_X = RNROOT::ic.GetHitPos().X();
  IC_Y = RNROOT::ic.GetHitPos().Y();
  IC_X_Raw = RNROOT::ic.xgrid.Ch(0);
  IC_Y_Raw = RNROOT::ic.ygrid.Ch(0);
  ThetaLab_A = RNROOT::si_array.Theta_A();
  ThetaLab_B = RNROOT::si_array.Theta_B();
  if(RNROOT::si_array.E_AB()>0 && RNROOT::si_array.Theta_A()!=0)
    QVal_Theta_A = RNROOT::gReactionInfo.DecayQValueEstimate(RNROOT::si_array.E_AB(),RNROOT::si_array.Theta_A());
  if(RNROOT::si_array.E_AB()>0 && RNROOT::si_array.Theta_B()!=0)
    QVal_Theta_B = RNROOT::gReactionInfo.DecayQValueEstimate(RNROOT::si_array.E_AB(),RNROOT::si_array.Theta_B());


  NeutLong_0 = RNROOT::neut[0].QLong();
  NeutShort_0 = RNROOT::neut[0].QShort();
  NeutLong_1 = RNROOT::neut[1].QLong();
  NeutShort_1 = RNROOT::neut[1].QShort();
  NeutLong_2 = RNROOT::neut[2].QLong();
  NeutShort_2 = RNROOT::neut[2].QShort();
  NeutLong_3 = RNROOT::neut[3].QLong();
  NeutShort_3 = RNROOT::neut[3].QShort();
  NeutLong_4 = RNROOT::neut[4].QLong();
  NeutShort_4 = RNROOT::neut[4].QShort();
  NeutLong_5 = RNROOT::neut[5].QLong();
  NeutShort_5 = RNROOT::neut[5].QShort();
  NeutLong_6 = RNROOT::neut[6].QLong();
  NeutShort_6 = RNROOT::neut[6].QShort();
  NeutLong_7 = RNROOT::neut[7].QLong();
  NeutShort_7 = RNROOT::neut[7].QShort();
  NeutLong_8 = RNROOT::neut[8].QLong();
  NeutShort_8 = RNROOT::neut[8].QShort();
  NeutLong_9 = RNROOT::neut[9].QLong();
  NeutShort_9 = RNROOT::neut[9].QShort();
  
  return 1;
}

bool NamedTree_Analyzer::ProcessFill(){
  if(RNROOT::si_[0].front.Mult() == 0 || RNROOT::si_[1].front.Mult()==0){
    ZeroCounter++;
    return 0;
  } 

  if(!(RNROOT::ic.T()>0)){
    return 0;
  }

 
  if(RNROOT::si_.size()>1){
    fNamedTree->Fill();
    FillCounter++;
  }
  return 1;
  
}

void NamedTree_Analyzer::Reset(){
  Energy_A = -100.0;
  Energy_B = -100.0;
  Energy_AB = -100.0;
  FrontChan_A = -100.0;
  FrontChan_B = -100.0;
  BackChan_A = -100.0;
  BackChan_B = -100.0;
  TotEn_IC = -100.0;
  dE_IC = -100.0;
  ThetaLab_A = -100.0;
  ThetaLab_B = -100.0;
  QVal_Theta_A = -100.0;
  QVal_Theta_B = -100.0;
  NeutLong_0= -100;
  NeutLong_1= -100;
  NeutLong_2= -100;
  NeutLong_3= -100;
  NeutLong_4= -100;
  NeutLong_5= -100;
  NeutLong_6= -100;
  NeutLong_7= -100;
  NeutLong_8= -100;
  NeutLong_9= -100;
  NeutShort_0= -100;
  NeutShort_1= -100;
  NeutShort_2= -100;
  NeutShort_3= -100;
  NeutShort_4= -100;
  NeutShort_5= -100;
  NeutShort_6= -100;
  NeutShort_7= -100;
  NeutShort_8= -100;
  NeutShort_9= -100;
}

bool NamedTree_Analyzer::TerminateIfLast(){
  fgRootFile->Write();
  fgRootFile->Close();
  return 1;
}

bool NamedTree_Analyzer::Terminate(){

  std::cout << "> ZeroCounter = " << ZeroCounter << std::endl;
  std::cout << "> FillCounter = " << FillCounter << std::endl;
  return 1;

}

#endif
