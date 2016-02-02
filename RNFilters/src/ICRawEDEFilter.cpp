/***********************************************************/
//Class: ICRawEDEFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
///***********************************************************/

#ifndef _ICRawEDEFilter_CXX
#define _ICRawEDEFilter_CXX

#include "ICRawEDEFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  ICRawEDEFilter::ICRawEDEFilter():LowValue(0),			      
			       HighValue(4096), 
			       UseGate(0)
  {
    
  }
  
  void ICRawEDEFilter::Reset(){
  
  }

  
  bool ICRawEDEFilter::Begin(){   
  
    return 1;
    
  }  
  
  

  bool ICRawEDEFilter::Process(){
    
    if(UseGate){
      if((ic.ERaw() + ic.DERaw())>LowValue && (ic.ERaw() + ic.DERaw())<HighValue)
	return 1;
      else
	return 0;
    }
    else{
      return 1;
    }
  }
  bool ICRawEDEFilter::ProcessFill(){
     
    return 1;
    
  }


  bool ICRawEDEFilter::Terminate(){
    return 1;
  }

  void ICRawEDEFilter::Clear(){
    
  }
  
  void ICRawEDEFilter::SetWindow(int Low, int High){
    UseGate = true;
    LowValue = Low;
    HighValue = High;
  }  


  
}


#endif
