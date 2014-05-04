/****************************************************
//Class: Analyzer_Template
//
//Author:Sean Kuvin
//
After you have read through this template.
make a copy of it with a new name and create your
own Analyzer class.  To add it the libRNanalyzers package, add the file to the Makefile & myLinkdef.h file following the example.  You can also compile the class using AcLiC as long as your root environment is set up to load the RN_Root Libs(see rootlogon.C) and your include path is set properly.

**************************************************/
#include "../../include/RN_Root.hpp"
/*
  #include any other analyzers' .hpp files to use the parameters of that analyzer that have been declared extern
  when you add this analyzer to the analyzer list make sure to add the above analyzers to the analyzer list as well.  Since this class will rely on the calculations performed in those analyzers, it is necessary that they are added to the analyzer list BEFORE this one.
 */

////////////////////////////////////////////////
/*I recommend encapsulating your analysis in a namespace to help prevent against similar definitions
 */

//namespace template{

////////////////////////////////////////////////
/*
  Declare any root objects (histograms, cuts, etc) that
are going to be used in this analysis,
also any global parameters.  If these parameters are wanted in follow up analysis scripts make sure to declare these parameters as extern in the .hpp file
 */

/////////////////////////////////////////////////

Analyzer_Template::Analyzer_Template()
{
  
}


void Analyzer_Template::ResetGlobals(){
  /*any global parameters unique to this analysis
    which you want reset at the beginning of each 
    event entry should be reset here
  */

}

bool Analyzer_Template::Begin(){

  /*
    if you want to add your histograms to the global RNROOT::gRootFile uncomment the following block.  It checks if the RNROOT::gRootFile has been created and exits if it has not.
   */
  /*
  if(!RNROOT::gRootFile){
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

bool Analyzer_Template::Process(){
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

bool Analyzer_Template::ProcessFill(){

  /*if your analyzer list has passed the Process() 
portion then it will loop over all ProcessFill()s.  
This is where you should fill your histograms.
  */
  return 1;
}
  


bool Analyzer_Template::Terminate(){
  /*
    any memory cleanup unique to this analysis
    that NEEDS to happen
    

   */
  return 1;
  
}

bool Analyzer_Template::TerminateIfLast(){
  /*
    any cleanup(such as writing to the RNROOT::gRootFile) that 
    should be reserved to the last analyzer in the list
   
    serious memory management decisions should be performed in the Terminate() function instead of this one.  This function will NOT be called if it is not the last analyzer in the list
   */

  return 1;
  
}


void LoadTemplateGates(){
  /*
    not a member of the Analyzer_Template class, this function serves to Load any gates that are unique to this analysis. The gates should be declared at the top of this file.

   */

}
