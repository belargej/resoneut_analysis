/***********************************************************/
//Class: NewTree_Analyzer
//
//Author:Sean Kuvin
//
//NewTree_Analyzer class to be inserted in the analyzer list for 
//producing a new tree.  Branches are created for the original 
//DataTree format (to allow for immediate reprocessing of the
//new trees with the same analyzers) as some sorted parameters 
//the new trees are filtered down by imposing requirements on the
//analyzers which come before it in the analyzer list.
//for instance, if trigger::Analyzer is first and a RequireS1()
//is set, then NewTree_Analyzer will be filtered to require events
//that have a valid S1 time.
/***********************************************************/

#ifndef _NTANALYZER_CXX
#define _NTANALYZER_CXX

#include "NewTree_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "Si_Analyzer.hpp"
using namespace unpacker;


NewTree_Analyzer::NewTree_Analyzer()
{

}

bool NewTree_Analyzer::Begin(){

  if(!rootfile){
    std::cout<<"output file has not been created"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(!newtree){
    std::cout<<"new tree has not been created"<<std::endl;   
    exit(EXIT_FAILURE);
  }
  
 
  newtree->Branch("Event",&Event,"RunNum/I:flag/I:ScalerIDX/I"); 
  newtree->Branch("prot_E",&silicon::prot_E);
  newtree->Branch("prot_dE",&silicon::prot_dE);
  newtree->Branch("prot_Theta",&silicon::prot_theta);
  newtree->Branch("prot_RelAngle",&silicon::rel_angle);
  newtree->Branch("IC_TotalE",&ionchamber::IC_TotalE);
  newtree->Branch("IC_ELoss",&ionchamber::IC_ELoss);
  newtree->Branch("ADC1",&ADC1,"ADC1[32]/F");
  newtree->Branch("ADC2",&ADC2,"ADC2[32]/F");
  newtree->Branch("ADC3",&ADC3,"ADC3[32]/F");
  newtree->Branch("ADC4",&ADC4,"ADC4[32]/F");
  newtree->Branch("ADC5",&ADC5,"ADC5[32]/F");
  newtree->Branch("ADC6",&ADC6,"ADC6[32]/F");
  newtree->Branch("ADC7",&ADC7,"ADC7[32]/F");
  newtree->Branch("TDC1",&TDC1,"TDC1[32]/F");
  newtree->Branch("TDC2",&TDC2,"TDC2[32]/F");
  newtree->Branch("TDC3",&TDC3,"TDC3[32]/F");
  newtree->Branch("TDC4",&TDC4,"TDC4[32]/F");
  newtree->Branch("QDC1",&QDC1,"QDC1[32]/F");
  newtree->Branch("QDC2",&QDC2,"QDC2[32]/F");
  newtree->Branch("QDC3",&QDC2,"QDC3[32]/F");

  return 1;
}

bool NewTree_Analyzer::Process(){
  return 1;
}

bool NewTree_Analyzer::ProcessFill(){
  newtree->Fill();
  return 1;
}

void NewTree_Analyzer::ResetGlobals(){
}

bool NewTree_Analyzer::TerminateIfLast(){
  rootfile->Write();
  rootfile->Close();
  return 1;
}

bool NewTree_Analyzer::Terminate(){

}

#endif
