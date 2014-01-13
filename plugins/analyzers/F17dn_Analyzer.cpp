/****************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "../../include/RN_Root.hpp"
#include "F17dn_Analyzer.hpp"

namespace _F17{

////////////////////////////////////////////////
/*
  Declare any root objects (histograms, cuts, etc) that
are going to be used in this analysis,
also any global parameters.  If these parameters are wanted in follow up analysis scripts make sure to declare these parameters as extern in the .hpp file
 */

/////////////////////////////////////////////////

F17dn_Analyzer::F17dn_Analyzer()
{
  
}


void F17dn_Analyzer::ResetGlobals(){
  /*any global parameters unique to this analysis
    which you want reset at the beginning of each 
    event entry should be reset here
  */

}

bool F17dn_Analyzer::Begin(){

  /*
    if you want to add your histograms to the global rootfile uncomment the following block.  It checks if the rootfile has been created and exits if it has not.
   */
  /*
  if(!rootfile){
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

bool F17dn_Analyzer::Process(){
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

bool F17dn_Analyzer::ProcessFill(){

  /*if your analyzer list has passed the Process() 
portion then it will loop over all ProcessFill()s.  
This is where you should fill your histograms.
  */
  return 1;
}
  


bool F17dn_Analyzer::Terminate(){
  /*
    any memory cleanup unique to this analysis
    that NEEDS to happen
    

   */
  return 1;
  
}

bool F17dn_Analyzer::TerminateIfLast(){
  /*
    any cleanup(such as writing to the rootfile) that 
    should be reserved to the last analyzer in the list
   
    serious memory management decisions should be performed in the Terminate() function instead of this one.  This function will NOT be called if it is not the last analyzer in the list
   */

  return 1;
  
}


void LoadTemplateGates(){
  /*
    not a member of the F17dn_Analyzer class, this function serves to Load any gates that are unique to this analysis. The gates should be declared at the top of this file.

   */

}
}
