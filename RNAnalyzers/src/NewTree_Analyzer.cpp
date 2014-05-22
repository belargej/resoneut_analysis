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
#include "RN_Root.hpp"
#include "Si_Analyzer.hpp"

NewTree_Analyzer::NewTree_Analyzer()
{

}

bool NewTree_Analyzer::Begin(){

  if(!fgRootFile){
    std::cout<<"output file has not been created"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(!fgDataTree){
    std::cout<<"new tree has not been created"<<std::endl;   
    exit(EXIT_FAILURE);
  }
 

  RNROOT::gParameter_stack.AddBranches(fgDataTree);
  RNROOT::gModule_stack.AddBranches(fgDataTree);

  return 1;
}

bool NewTree_Analyzer::Process(){
  return 1;
}

bool NewTree_Analyzer::ProcessFill(){
  RNROOT::gNewTree->Fill();
  return 1;
}

void NewTree_Analyzer::Reset(){
}

bool NewTree_Analyzer::TerminateIfLast(){
  fgRootFile->Write();
  fgRootFile->Close();
  return 1;
}

bool NewTree_Analyzer::Terminate(){
  return 1;
}

#endif
