/***********************************************************/
//Class: ICCalEDEFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
///***********************************************************/

#ifndef _ICCalEDEFilter_CXX
#define _ICCalEDEFilter_CXX

#include "ICCalEDEFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  ICCalEDEFilter::ICCalEDEFilter():LowValue(0),			      
			       HighValue(4096), 
			       UseGate(0)
  {
    
  }
  
  void ICCalEDEFilter::Reset(){
  
  }

  
  bool ICCalEDEFilter::Begin(){   
  
    return 1;
    
  }  
  
  

  bool ICCalEDEFilter::Process(){
    
    if(UseGate){
      std::cout << "ARE YOU USING THE CORRECT CALIBRATION???????" << std::endl;
      double ICCalEn = (ic.ERaw() + ic.DERaw())*0.0312251-17.047889;// Including Target Thickness
      ICCalEn = 47.746935*TMath::Power(TMath::E(),0.01024972*ICCalEn);// Including Target Thickness
      if((ICCalEn+si_array.E_AB())>LowValue && (ICCalEn+si_array.E_AB())<HighValue)
	return 1;
      else
	return 0;
    }
    else{
      return 1;
    }
  }
  bool ICCalEDEFilter::ProcessFill(){
     
    return 1;
    
  }


  bool ICCalEDEFilter::Terminate(){
    return 1;
  }

  void ICCalEDEFilter::Clear(){
    
  }
  
  void ICCalEDEFilter::SetWindow(int Low, int High){
    UseGate = true;
    LowValue = Low;
    HighValue = High;
  }  


  
}


#endif
