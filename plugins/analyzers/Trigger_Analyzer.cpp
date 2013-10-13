#include "Trigger_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "../../include/RN_Unpack2Root.hpp"

namespace trigger{
  int s1_bit(0);
  int icds_bit(0);


  Trigger_Analyzer::Trigger_Analyzer()
  {
    
  }
  
  void Trigger_Analyzer::Begin(){
    
  }
  
  
  void Trigger_Analyzer::Process(){
    s1_bit=0;
    icds_bit=0;
    
    s1_bit = triggerbit[0].Check();
    icds_bit = triggerbit[1].Check();
    
  }
  
  
  
  void Trigger_Analyzer::Terminate(){
    
    
  }

}
