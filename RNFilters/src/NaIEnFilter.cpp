/***********************************************************/
//Class: NaIEnFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero NaI time.
///***********************************************************/

#ifndef _NaIEnFilter_CXX
#define _NaIEnFilter_CXX

#include "NaIEnFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  NaIEnFilter::NaIEnFilter():LowValue(0),
				 HighValue(4096),
				 UseGate(0),
				 YesNaIEn(0)
  {
    
  }
  
  void NaIEnFilter::Reset(){
  
  }

  
  bool NaIEnFilter::Begin(){   
    
    return 1;
  }  
  
  

  bool NaIEnFilter::Process(){
    
    //std::cout << " Use Gate ? " << UseGate << std::endl;
    if(UseGate){
      for(int  i = 0; i < 20; i++){
	if((nai[i].E1()>LowValue && nai[i].E1()<HighValue)||(nai[i].E2()>LowValue && nai[i].E2()<HighValue) )	
	  return 1;
	//else
	// return 0;
      }
      return 0;
    }
    else if(YesNaIEn){
      for(int i = 0; i < 20; i++){
	if((nai[i].E1()>0)||(nai[i].E2()>0)){
	  return 1;
	}
	//else
	// return 0;
      }
      return 0;
    }
    else{ // Want a zero for NaI time
      //std::cout << "NaI Time : " << nai_array.TRaw() << std::endl;
      for(int i = 0; i < 20; i++){
	if((nai[i].E1()>0)||(nai[i].E2()>0)){
	  return 0;
	}
      }
      return 1;
    }
  }
  bool NaIEnFilter::ProcessFill(){
     
    return 1;
    
  }


  bool NaIEnFilter::Terminate(){
    return 1;
  }

  void NaIEnFilter::Clear(){
    
  }

  void NaIEnFilter::SetWindow(int Low, int High){
    UseGate = true;
    YesNaIEn = false;
    LowValue = Low;
    HighValue = High;
  }
  
  
}


#endif
