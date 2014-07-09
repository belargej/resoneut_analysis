/****************************************************
//Class: Mg24dn_Analyzer
//
//Author:Sean Kuvin
//


**************************************************/
#include "RN_Root.hpp"
#include "Mg24dn_Analyzer.hpp"
#include "Q_ValueAnalyzer.hpp"

using namespace RNROOT;

namespace _Mg24{
 
   Mg24dn_Analyzer::Mg24dn_Analyzer()
  {
    
  }
  

  void Mg24dn_Analyzer::Reset(){

  }
  
  bool Mg24dn_Analyzer::Begin(){
    fgRootFile->mkdir("Mg24Analysis");
    fgRootFile->cd("Mg24Analysis");

  return 1;
  }
  
  
  bool Mg24dn_Analyzer::Process(){
 
    return 1;
  }
  bool Mg24dn_Analyzer::ProcessFill(){
 
    return 1;
  }
  bool Mg24dn_Analyzer::Terminate(){
    
    return 1;
  
  }
  
 
  void Load_Mg24_dn_Gates(){
    
    
  }
}
