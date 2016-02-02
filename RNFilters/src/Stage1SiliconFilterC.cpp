/***********************************************************/
//Class: Stage1SiliconFilterC
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1SiliconFilterC_CXX
#define _Stage1SiliconFilterC_CXX

#include "Stage1SiliconFilterC.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1SiliconFilterC::Stage1SiliconFilterC()
  {
    
  }
  
  void Stage1SiliconFilterC::Reset(){
  
  }

  
  bool Stage1SiliconFilterC::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1SiliconFilterC::Process(){
    
    //suppress "overflow" bins
    if(si_[0].front.ERaw(0) > 4090 || si_[1].front.ERaw(0) > 4090){
      return 0;
    }
   
    //require multiplicity in both detectors
    if(si_[0].front.Mult() == 0 || si_[1].front.Mult()==0){
      return 0;
    }

    //suppress high multiplicity events
    if(si_[0].front.Mult()>4 || si_[0].back.Mult()>4 || si_[1].front.Mult()>4 || si_[1].back.Mult()>4){
      return 0;
    }
  

    //require an IC DE segment signal
    if(!(ic.DERaw()>0)){
      return 0;
    }
  
    return 1;
  }
  bool Stage1SiliconFilterC::ProcessFill(){
     
    return 1;
    
  }


  bool Stage1SiliconFilterC::Terminate(){
    return 1;
  }

  void Stage1SiliconFilterC::Clear(){
    
  }
  
  
}


#endif
