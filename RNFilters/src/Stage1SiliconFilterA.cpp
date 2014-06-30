/***********************************************************/
//Class: Stage1SiliconFilterA
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1SiliconFilterA_CXX
#define _Stage1SiliconFilterA_CXX

#include "Stage1SiliconFilterA.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1SiliconFilterA::Stage1SiliconFilterA()
  {
    
  }
  
  void Stage1SiliconFilterA::Reset(){
  
  }

  
  bool Stage1SiliconFilterA::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1SiliconFilterA::Process(){
    
    //suppress overflow bins
    if(si_[0].front.ERaw() > 4090 || si_[1].front.ERaw() > 4090){
      return 0;
    }
    
    //suppress high multiplicity events
    if(si_[0].front.Mult()>3 || si_[0].back.Mult()>3 || si_[1].front.Mult()>3 || si_[1].back.Mult()>3){
      return 0;
    }
    
    //suppress ICds trigger bits
    if(triggerbit[1].Check()){
      return 0;
    }

    //require a valid IC time
    if(!(ic.T()>0)){
      return 0;
    }

    //require sib trigger bit
    if(!triggerbit[0].Check()){
      return 0;
    }


    return 1;
  }
  bool Stage1SiliconFilterA::ProcessFill(){
     
    return 1;
    
  }


  bool Stage1SiliconFilterA::Terminate(){
    return 1;
  }

  void Stage1SiliconFilterA::Clear(){
    
  }
  
  
}


#endif
