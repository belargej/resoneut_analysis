/***********************************************************/
//Class: SiHitFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
///***********************************************************/

#ifndef _SiHitFilter_CXX
#define _SiHitFilter_CXX

#include "SiHitFilter.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  SiHitFilter::SiHitFilter():UseSingleHit(0),
			     UseDoubleHit(0),
			     SiAThresh(0),
			     SiBThresh(0)
  {
    
  }
  
  void SiHitFilter::Reset(){
  
  }

  
  bool SiHitFilter::Begin(){   
  
    return 1;
  }  
  
  

  bool SiHitFilter::Process(){
    
    //std::cout << UseSingleHit << std::endl;
    //std::cout << UseDoubleHit << std::endl;
    if(UseSingleHit){
      if(si_array.E_A()>SiAThresh && si_array.E_B()>SiBThresh){
	if(si_[0].Back_E(1)>SiAThresh)
	  return 0;
	else if (si_[1].Back_E(1)>SiBThresh)
	  return 0;
	else
	  return 1;
      }
      else{
	return 0;
      }
    }
    else if(UseDoubleHit){
      if(si_array.E_A()>SiAThresh && si_array.E_B()>SiBThresh){
	if(si_[0].Back_E(1)>SiAThresh && si_[1].Back_E(1)>SiBThresh){
	  if(si_[0].Back_E(2)>SiAThresh)
	    return 0;
	  else if (si_[1].Back_E(2)>SiBThresh)
	    return 0;
	  else
	    return 1;
	}
      }
      else{
	return 0;
      }

    }
    else{ 
      //std::cout << " Si A En From Ground Level : F: " << si_[0].Front_E() << " , B: " 
      //	<< si_[0].Back_E(1) << std::endl;
      //std::cout << " Si A En From Top Level : " << si_array.E_A() << std::endl;
      //
      //std::cout << " Si B En From Ground Level : F: " << si_[1].Front_E() << " , B: " 
      //	<< si_[1].Back_E(1) << std::endl;
      //std::cout << " Si B En From Top Level : " << si_array.E_B() << std::endl;
      return 1;
    }


   

    return 1;
  }
  bool SiHitFilter::ProcessFill(){
     
    return 1;
    
  }


  bool SiHitFilter::Terminate(){
    return 1;
  }

  void SiHitFilter::Clear(){
    
  }

  
  
}


#endif