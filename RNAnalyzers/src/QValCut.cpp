/****************************************************
//Class: QValCut
//
//Author:Belarge

**************************************************/
#ifndef _QVALCUT_CXX
#define _QVALCUT_CXX

#include "QValCut.hpp"
#include "RN_Root.hpp"

#include "Si_Analyzer.hpp"
#include "IC_Analyzer.hpp"

namespace qcut{


  //Histograms
  TH1F *UnCutQVal;
  TH1F *

  
  QValCut::QValCut()
  {
  
  }


  void QValCut::Reset()(){

  }

  bool QValCut::Begin(){

    /*
      if you want to add your histograms to the global fgRootFile uncomment the following block.  It checks if the fgRootFile has been created and exits if it has not.
    */
    /*
      if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
      }
    */

    /*create your root file structure here and
      set your memory allocations for the root objects
      declared above.
    */


    return 1;
 
  }

  bool QValCut::Process(){
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

  bool QValCut::ProcessFill(){

    /*if your analyzer list has passed the Process() 
      portion then it will loop over all ProcessFill()s.  
      This is where you should fill your histograms.
    */
    return 1;
  }
  


  bool QValCut::Terminate(){
    /*
      any memory cleanup unique to this analysis
      that NEEDS to happen
    

    */
    return 1;
  
  }

  bool QValCut::TerminateIfLast(){
    /*
      any cleanup(such as writing to the fgRootFile) that 
      should be reserved to the last analyzer in the list
   
      serious memory management decisions should be performed in the Terminate() function instead of this one.  This function will NOT be called if it is not the last analyzer in the list
    */

    return 1;
  
  }


  void LoadTemplateGates(){
    /*
      not a member of the QValCut class, this function serves to Load any gates that are unique to this analysis. The gates should be declared at the top of this file.

    */

  }
}
