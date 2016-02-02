/***********************************************************/
//Class: NaITimeFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero NaI time.
///***********************************************************/

#ifndef _NaITimeFilter_CXX
#define _NaITimeFilter_CXX

#include "NaITimeFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  NaITimeFilter::NaITimeFilter():LowValue(0),
				 HighValue(4096),
				 UseGate(0),
				 YesNaITime(0)
  {
    
  }
  
  void NaITimeFilter::Reset(){
  
  }

  
  bool NaITimeFilter::Begin(){   
    
    return 1;
  }  
  
  

  bool NaITimeFilter::Process(){
    
    //std::cout << " Use Gate ? " << UseGate << std::endl;
    if(UseGate){
      if(nai_array.TRaw()>LowValue && nai_array.TRaw()<HighValue)
	return 1;
      else
	return 0;
    }
    else if(YesNaITime){
      if((nai_array.TRaw()>0)){
	return 1;
      }
      else
	return 0;
    }
    else{ // Want a zero for NaI time
      //std::cout << "NaI Time : " << nai_array.TRaw() << std::endl;
      if((nai_array.TRaw()>0)){
	return 0;
      }
      
      return 1;
    }
  }
  bool NaITimeFilter::ProcessFill(){
     
    return 1;
    
  }


  bool NaITimeFilter::Terminate(){
    return 1;
  }

  void NaITimeFilter::Clear(){
    
  }

  void NaITimeFilter::SetWindow(int Low, int High){
    UseGate = true;
    YesNaITime = false;
    LowValue = Low;
    HighValue = High;
  }
  
  
}


#endif
