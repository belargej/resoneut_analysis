/***********************************************************/
//Class: Stage1ICDSFilter
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1ICDSFilter_CXX
#define _Stage1ICDSFilter_CXX

#include "Stage1ICDSFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1ICDSFilter::Stage1ICDSFilter()
  {
    
  }
  
  void Stage1ICDSFilter::Reset(){
  
  }

  
  bool Stage1ICDSFilter::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1ICDSFilter::Process(){

    //require ICds trigger bits
    if(!triggerbit[1].Check()){
      return 0;
    }
    
    return 1;
  }
  bool Stage1ICDSFilter::ProcessFill(){
    
    return 1;
    
  }
  
  
  bool Stage1ICDSFilter::Terminate(){
    return 1;
  }
  
  void Stage1ICDSFilter::Clear(){
    
  }
  
  
}


#endif
