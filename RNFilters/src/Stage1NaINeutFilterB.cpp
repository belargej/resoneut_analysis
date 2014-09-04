/***********************************************************/
//Class: Stage1NaINeutFilterB
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1NaINeutFilterB_CXX
#define _Stage1NaINeutFilterB_CXX

#include "Stage1NaINeutFilterB.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1NaINeutFilterB::Stage1NaINeutFilterB()
  {
    
  }
  
  void Stage1NaINeutFilterB::Reset(){
  
  }

  
  bool Stage1NaINeutFilterB::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1NaINeutFilterB::Process(){
    
    
    //suppress ICds trigger bits
    if(triggerbit[1].Check()){
      return 0;
    }

    //require a valid IC DE signal
    if(!(ic.DE()>0)){
      return 0;
    }

    //suppress sib trigger bit
    if(triggerbit[0].Check()){
      return 0;
    }
    
    bool NaI = false;

    for(int i =0;i<NAI_NUM;i++){
      if(nai[i].fT[0]>0 && nai[i].fT[1]>0){
	NaI = true;
	return 1;
      }
    }

    /*if(NaI == true){
      for(int j = 0;j<NEUTNUM){
	if(neut[j].)
	  return 1;
      }
    }*/

    return 0;
  }
  bool Stage1NaINeutFilterB::ProcessFill(){
     
    return 1;
    
  }


  bool Stage1NaINeutFilterB::Terminate(){
    return 1;
  }

  void Stage1NaINeutFilterB::Clear(){
    
  }
  
  
}


#endif
