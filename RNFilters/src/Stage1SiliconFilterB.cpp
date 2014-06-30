/***********************************************************/
//Class: Stage1SiliconFilterB
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1SiliconFilterB_CXX
#define _Stage1SiliconFilterB_CXX

#include "Stage1SiliconFilterB.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1SiliconFilterB::Stage1SiliconFilterB()
  {
    
  }
  
  void Stage1SiliconFilterB::Reset(){
  
  }

  
  bool Stage1SiliconFilterB::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1SiliconFilterB::Process(){
    
    //suppress overflow bins
    if(si_[0].front.ERaw(0) > 4090 || si_[1].front.ERaw(0) > 4090){
      return 0;
    }
    
    //suppress high multiplicity events
    if(si_[0].front.Mult()>3 || si_[0].back.Mult()>3 || si_[1].front.Mult()>3 || si_[1].back.Mult()>3){
      return 0;
    }
  

    //require an IC DE segment signal
    if(!(ic.DERaw()>0)){
      return 0;
    }
  
    //suppress ICds trigger bits
    if(triggerbit[1].Check()){
      return 0;
    }
    
    //require sib trigger bit
    if(!(triggerbit[0].Check())){
      return 0;
    }


    return 1;
  }
  bool Stage1SiliconFilterB::ProcessFill(){
     
    return 1;
    
  }


  bool Stage1SiliconFilterB::Terminate(){
    return 1;
  }

  void Stage1SiliconFilterB::Clear(){
    
  }
  
  
}


#endif
