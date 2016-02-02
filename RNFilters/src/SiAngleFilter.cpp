/***********************************************************/
//Class: SiAngleFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
///***********************************************************/

#ifndef _SiAngleFilter_CXX
#define _SiAngleFilter_CXX

#include "SiAngleFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  SiAngleFilter::SiAngleFilter():LowValue(0),			      
			       HighValue(4096), 
			       UseGate(0)
  {
    
  }
  
  void SiAngleFilter::Reset(){
  
  }

  
  bool SiAngleFilter::Begin(){   
  
    return 1;
    
  }  
  
  

  bool SiAngleFilter::Process(){
    
    if(UseGate){
      if(si_array.Theta_A()*RadToDeg()>LowValue && si_array.Theta_A()*RadToDeg()<HighValue)
	return 1;
      else
	return 0;
    }
    else{
      return 1;
    }
  }
  bool SiAngleFilter::ProcessFill(){
     
    return 1;
    
  }


  bool SiAngleFilter::Terminate(){
    return 1;
  }

  void SiAngleFilter::Clear(){
    
  }
  
  void SiAngleFilter::SetWindow(int Low, int High){
    UseGate = true;
    LowValue = Low;
    HighValue = High;
  }  


  
}


#endif
