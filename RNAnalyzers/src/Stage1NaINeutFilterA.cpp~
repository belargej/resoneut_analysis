/***********************************************************/
//Class: Stage1SiliconFilterD
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1SiliconFilterD_CXX
#define _Stage1SiliconFilterD_CXX

#include "Stage1SiliconFilterD.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1SiliconFilterD::Stage1SiliconFilterD()
  {
    
  }
  
  void Stage1SiliconFilterD::Reset(){
  
  }

  
  bool Stage1SiliconFilterD::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1SiliconFilterD::Process(){
    
    //require multiplicity in both detectors
    if(si_[0].front.Mult() == 0 || si_[1].front.Mult()==0){
      return 0;
    } 
    
    //require an IC timing
    /*if(!(ic.T()>0)){
      return 0;
      }*/
    
    
    
    return 1;
  }
  bool Stage1SiliconFilterD::ProcessFill(){
    
    return 1;
    
  }

  
  bool Stage1SiliconFilterD::Terminate(){
    return 1;
  }

  void Stage1SiliconFilterD::Clear(){
    
  }
  
  
}


#endif
