/***********************************************************/
//Class: Stage1NaINeutFilterA
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
///***********************************************************/

#ifndef _Stage1NaINeutFilterA_CXX
#define _Stage1NaINeutFilterA_CXX

#include "Stage1NaINeutFilterA.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace RNfilters{
  

  Stage1NaINeutFilterA::Stage1NaINeutFilterA()
  {
    
  }
  
  void Stage1NaINeutFilterA::Reset(){
  
  }

  
  bool Stage1NaINeutFilterA::Begin(){   
  
    return 1;
  }  
  
  

  bool Stage1NaINeutFilterA::Process(){
    
    //require multiplicity in both detectors
    /*if(si_[0].front.Mult() == 0 || si_[1].front.Mult()==0){
      return 0;
      } */

    // require neutron time and gamma time:
    if(!(Narray.T()>0)){
      return 0;
    }
    
    if(!(nai_array.T())){
      return 0;
    }

    //require an IC timing
    /*if(!(ic.T()>0)){
      return 0;
      }*/
    
    
    
    return 1;
  }
  bool Stage1NaINeutFilterA::ProcessFill(){
    
    return 1;
    
  }

  
  bool Stage1NaINeutFilterA::Terminate(){
    return 1;
  }

  void Stage1NaINeutFilterA::Clear(){
    
  }
  
  
}


#endif
